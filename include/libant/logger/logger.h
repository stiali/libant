/*
*
* LibAnt - A handy C++ library
* Copyright (C) 2021 Antigloss Huang (https://github.com/antigloss) All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef LIBANT_LOGGER_LOGGER_H_
#define LIBANT_LOGGER_LOGGER_H_

#include <atomic>
#ifdef _WIN32
#include <fstream>
#include <iostream>
#else
#include <unistd.h>
#endif
#include <memory>
#include <mutex>
#include <libant/logger/fmt/chrono.h>
#include <libant/logger/fmt/format.h>

namespace ant {

/**
 * Logger is a thread-safe logging facility which writes logs with different severity levels to files, console, or both.
 * Logs with different severity levels are written to different logfiles.
 * It's recommended to use ant::InitGlobalLogger to create a global Logger object, then use LOG_TRACE/LOG_INFO/LOG_WARN/... to write logs.
 * You can also create as many Logger objects as desired if in need. \n
 *
 * Features:
 *   \l Auto rotation: It'll create a new logfile whenever day changes or size of the current logfile exceeds the configured size limit.
 *   \l Log levels: 5 different levels are supported. Logs with different levels are written to different logfiles. By setting the Logger object to a higher log level, lower level logs will be filtered out.
 *   \l Log-through: Logs with higher severity level will be written to all the logfiles with lower severity level if configured to do so.
 *   \l Logs are not buffered, they are written to logfiles immediately with fileStream.write(log.c_str(), log.size()).
 *   \l Symlinks named `LOG_LEVEL`.log_filename_prefix.log will be created and link to the most current logfiles.
 */
class Logger {
public:
    /**
     * LogLevel is used to exclude logs with lower level.
     */
    enum LogLevel {
        LogLevelTrace,
        LogLevelInfo,
        LogLevelWarn,
        LogLevelError,
        LogLevelFatal, // Calls exit(-1) after log written.

        LogLevelCount
    };

    /**
     * LogDest controls where the logs are written.
     */
    enum LogDest {
        LogDestNone = 0,                            // Don't write logs.
        LogDestFile = 1,                            // Write logs to files.
        LogDestConsole = 2,                         // Write logs to console.
        LogDestBoth = LogDestFile | LogDestConsole, // Write logs both to files and console.
    };

    /**
     * ControlFlag controls how the logs are written. Use `|`(Or operator) to mix multiple flags.
     */
    enum ControlFlag {
        ControlFlagNone = 0,
        ControlFlagLogThrough = 0x01, // Controls if logs with higher level are written to lower level log files.
        ControlFlagLogDate = 0x02,    // Controls if a date string formatted as '20201201' is prepended to the logs.
        ControlFlagNoPrepends = 0x04, // Don't prepend any additional info to the logs.
        ControlFlagNoSymlinks = 0x08, // Don't create symlinks.
    };

    /**
     * Cfg contains options for creating a new Logger object.
     */
    class Cfg {
    public:
        friend class Logger;

    public:
        /**
         * Construct a Cfg object.
         *
         * @param dir Directory to hold the log files. If left empty, current working directory is used. Should you need to create multiple Logger objects, better to associate them with different directories.
         * @param filenamePrefix Name of a log file is formatted as `filenamePrefix.LogLevel.DateTime.log`.
         * @param fileMaxSizeInMB Limits the maximum size in MB for a single log file. 0 means unlimited.
         * @param enableThreadMutex Set it true if you want the Logger object to be thread-safe, false if the Logger object is used single-threaded.
         * @param level Don't write logs below `level`.
         * @param dest Where the logs are written.
         * @param controlFlags How the logs are written.
         */
        Cfg(const std::string& dir, const std::string& filenamePrefix, uint32_t fileMaxSizeInMB, bool enableThreadMutex = true, LogLevel level = LogLevelInfo,
            LogDest dest = LogDestFile, uint32_t controlFlags = ControlFlagLogThrough)
            : logDir_(dir)
            , logFilenamePrefix_(filenamePrefix)
            , logFileMaxSize_(fileMaxSizeInMB)
            , logLevel_(level)
            , logDest_(dest)
            , controlFlags_(controlFlags)
            , enableThreadMutex_(enableThreadMutex)
        {
        }

    private:
        std::string logDir_;
        std::string logFilenamePrefix_;
        uint64_t logFileMaxSize_; // in MB
        LogLevel logLevel_;
        LogDest logDest_;
        uint32_t controlFlags_;
        bool enableThreadMutex_;
    };

public:
    /**
     * Construct a Logger object.
     *
     * @param cfg
     */
    Logger(const Cfg& cfg);

    /**
     * Change log level of the Logger object at runtime. Thread-safe.
     * @param level
     */
    void SetLogLevel(LogLevel level)
    {
        logLevel_ = level;
    }

    /**
     * Change log destination of the Logger object at runtime. Thread-safe.
     * @param dest
     */
    void SetLogDest(LogDest dest)
    {
        logDest_ = dest;
    }

    /**
     * Writes a log. Use FMT_STRING() on the format string to enable static argument count checking. \n
     * Eg: Log(ant::Logger::LogLevelInfo, FMT_STRING("User login. uid={} ip={}"), 12321, "127.0.0.1")
     *
     * @tparam S
     * @tparam Args
     *
     * @param level
     * @param format
     * @param args
     */
    template<typename S, typename... Args>
    void Log(LogLevel level, const S& format, Args&&... args)
    {
        LogLevel lowestLevel = logLevel_;
        LogDest dest = logDest_;
        if (lowestLevel > level || dest == LogDestNone) {
            return;
        }

        std::string* buf;
        if (multiThreaded_) {
            buf = &thrBuf_;
        } else {
            buf = &buf_;
        }

        auto backInserter = std::back_insert_iterator<std::string>(*buf);
        auto tNow = std::chrono::system_clock::now();
        auto curTm = std::chrono::duration_cast<std::chrono::microseconds>(tNow.time_since_epoch()).count();
        auto tmNow = fmt::localtime(curTm / 1000000);
        if (!(controlFlags_ & ControlFlagNoPrepends)) {
            if (!(controlFlags_ & ControlFlagLogDate)) {
                fmt::format_to(backInserter, "{}{:%H:%M:%S} ", levelInitials_[level], tmNow);
            } else {
                fmt::format_to(backInserter, "{}{:%Y%m%d %H:%M:%S} ", levelInitials_[level], tmNow);
            }
        }
        fmt::format_to(backInserter, format, std::forward<Args>(args)...);
        buf->append("\n");

        if (logDest_ & LogDestFile) {
            auto microSeconds = static_cast<uint32_t>(curTm % 1000000);
            if (controlFlags_ & ControlFlagLogThrough) {
                for (int lv = level; lv >= lowestLevel; --lv) {
                    loggers_[lv]->Log(tmNow, microSeconds, *buf);
                }
            } else {
                loggers_[level]->Log(tmNow, microSeconds, *buf);
            }
        }

        if (logDest_ & LogDestConsole) {
            if (multiThreaded_) {
                lock_.lock();
            }

#ifdef _WIN32
            std::cout.write(buf->c_str(), buf->size());
#else
            std::ignore = write(STDOUT_FILENO, buf->c_str(), buf->size());
#endif

            if (multiThreaded_) {
                lock_.unlock();
            }
        }

        buf->clear();
    }

private:
    class Impl {
    public:
        Impl(const Logger* parent, const std::string& filenamePrefix, LogLevel level, bool enableMutex);

#ifndef _WIN32
        ~Impl()
        {
            close(outFD_);
        }
#endif

        void Log(const tm& tmNow, uint32_t microSeconds, const std::string& content)
        {
            if (enableMutex_) {
                lock_.lock();
            }

            if (!log(tmNow, microSeconds, content)) {
                log(tmNow, microSeconds, content); // try one more time
            }

            if (enableMutex_) {
                lock_.unlock();
            }
        }

    private:
        bool log(const tm& tmNow, uint32_t microSeconds, const std::string& content);

    private:
        const Logger* parent_;
        const LogLevel level_;
        const bool enableMutex_;
        const std::string symlink_;

        std::mutex lock_; // protects the following variables
#ifdef _WIN32
        std::ofstream out_;
#else
        int outFD_{-1};
#endif
        int curDay_{-1};
        uint64_t curFileSize_{0};
    };

private:
    const std::string logDir_;
    const std::string logPathPrefix_;
    const uint64_t logFileMaxSize_;
    const uint32_t controlFlags_;
    const bool multiThreaded_;

    std::mutex lock_; // protects cout

    std::atomic<LogLevel> logLevel_;
    std::atomic<LogDest> logDest_;
    std::unique_ptr<Impl> loggers_[LogLevelCount];
    std::string buf_;
    static thread_local std::string thrBuf_;
    static constexpr char levelInitials_[LogLevelCount] = {'T', 'I', 'W', 'E', 'F'};
};

namespace detail {

extern std::unique_ptr<Logger> gLogger;

}

/**
 * Initialize the global Logger object. Should be call once and only once. \n
 * Usage: LOG_INFO("User login. uid={} ip={}", 12321, "127.0.0.1")
 *
 * @param cfg
 */
inline void InitGlobalLogger(const Logger::Cfg& cfg)
{
    detail::gLogger = std::make_unique<Logger>(cfg);
}

/**
 * Change log level of the global Logger object at runtime. Thread-safe.
 * @param level
 */
inline void SetGlobalLoggerLogLevel(Logger::LogLevel level)
{
    detail::gLogger->SetLogLevel(level);
}

/**
 * Change log destination of the global Logger object at runtime. Thread-safe.
 * @param dest
 */
inline void SetGlobalLoggerLogDest(Logger::LogDest dest)
{
    detail::gLogger->SetLogDest(dest);
}

} // namespace ant

#if defined(_WIN64) || defined(_WIN32)
#define LOGGER_WRITE(level_, fmt_, ...) ant::detail::gLogger->Log(level_, FMT_STRING("{}:{}] " fmt_), strrchr("\\" __FILE__, '\\') + 1, __LINE__, ##__VA_ARGS__)
#else
#define LOGGER_WRITE(level_, fmt_, ...) ant::detail::gLogger->Log(level_, FMT_STRING("{}:{}] " fmt_), strrchr("/" __FILE__, '/') + 1, __LINE__, ##__VA_ARGS__)
#endif

#ifndef DISABLE_LOG_TRACE
/**
 * Writes trace level log. Could be disable at compile time by defining DISABLE_LOG_TRACE
 */
#define LOG_TRACE(fmt_, ...) LOGGER_WRITE(ant::Logger::LogLevelTrace, fmt_, ##__VA_ARGS__)
#else
#define LOG_TRACE(...)
#endif

#ifndef DISABLE_LOG_INFO
/**
 * Writes info level log. Could be disable at compile time by defining DISABLE_LOG_INFO
 */
#define LOG_INFO(fmt_, ...) LOGGER_WRITE(ant::Logger::LogLevelInfo, fmt_, ##__VA_ARGS__)
#else
#define LOG_INFO(...)
#endif

#ifndef DISABLE_LOG_WARN
/**
 * Writes warn level log. Could be disable at compile time by defining DISABLE_LOG_WARN
 */
#define LOG_WARN(fmt_, ...) LOGGER_WRITE(ant::Logger::LogLevelWarn, fmt_, ##__VA_ARGS__)
#else
#define LOG_WARN(...)
#endif

#ifndef DISABLE_LOG_ERROR
/**
 * Writes error level log. Could be disable at compile time by defining DISABLE_LOG_ERROR
 */
#define LOG_ERROR(fmt_, ...) LOGGER_WRITE(ant::Logger::LogLevelError, fmt_, ##__VA_ARGS__)
#else
#define LOG_ERROR(...)
#endif

/**
 * Writes fatal level log, then calls exit(-1) to terminate the process.
 */
#define LOG_FATAL(fmt_, ...)                                       \
    LOGGER_WRITE(ant::Logger::LogLevelFatal, fmt_, ##__VA_ARGS__); \
    exit(-1)

#endif //LIBANT_LOGGER_LOGGER_H_
