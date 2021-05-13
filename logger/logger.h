#ifndef LIBANT_LOGGER_LOGGER_H_
#define LIBANT_LOGGER_LOGGER_H_

#include <atomic>
#include <fstream>
#include <iostream>
#include <memory>
#include <mutex>
#include "fmt/chrono.h"
#include "fmt/format.h"

namespace ant {

class Logger {
public:
    enum LogLevel {
        LogLevelTrace,
        LogLevelInfo,
        LogLevelWarn,
        LogLevelError,
        LogLevelFatal, // Calls exit(-1) after log written.

        LogLevelCount
    };

    enum LogDest {
        LogDestNone = 0,                            // Don't write logs.
        LogDestFile = 1,                            // Write logs to files.
        LogDestConsole = 2,                         // Write logs to console.
        LogDestBoth = LogDestFile | LogDestConsole, // Write logs both to files and console.
    };

    enum ControlFlag {
        ControlFlagLogThrough = 1, // Controls if logs with higher level are written to lower level log files.
        ControlFlagLogDate = 2,    // Controls if a date string formatted as '20201201' is prepended to the logs.
    };

    class Cfg {
    public:
        friend class Logger;

    public:
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
    Logger(const Cfg& cfg)
        : logDir_(cfg.logDir_)
        , logPathPrefix_(!cfg.logDir_.empty() ? cfg.logDir_ + "/" + cfg.logFilenamePrefix_ : cfg.logFilenamePrefix_)
        , logFileMaxSize_(cfg.logFileMaxSize_ ? cfg.logFileMaxSize_ * 1024 * 1024 : 0xFFFFFFFFFF000000)
        , controlFlags_(cfg.controlFlags_)
        , multiThreaded_(cfg.enableThreadMutex_)
        , logLevel_(cfg.logLevel_)
        , logDest_(cfg.logDest_)
        , loggers_{{this, LogLevelTrace, cfg.enableThreadMutex_},
                   {this, LogLevelInfo, cfg.enableThreadMutex_},
                   {this, LogLevelWarn, cfg.enableThreadMutex_},
                   {this, LogLevelError, cfg.enableThreadMutex_},
                   {this, LogLevelFatal, cfg.enableThreadMutex_}}
    {
    }

    void SetLogLevel(LogLevel level)
    {
        logLevel_ = level;
    }

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
        if (!(controlFlags_ & ControlFlagLogDate)) {
            fmt::format_to(backInserter, "{}{:%H:%M:%S} ", levelInitials_[level], tmNow);
        } else {
            fmt::format_to(backInserter, "{}{:%Y%m%d %H:%M:%S} ", levelInitials_[level], tmNow);
        }
        fmt::format_to(backInserter, format, std::forward<Args>(args)...);
        buf->append("\n");

        if (logDest_ & LogDestFile) {
            auto microSeconds = curTm % 1000000;
            if (controlFlags_ & ControlFlagLogThrough) {
                for (int lv = level; lv >= lowestLevel; --lv) {
                    loggers_[lv].Log(tmNow, microSeconds, *buf);
                }
            } else {
                loggers_[level].Log(tmNow, microSeconds, *buf);
            }
        }

        if (logDest_ & LogDestConsole) {
            if (multiThreaded_) {
                lock_.lock();
            }

            std::cout.write(buf->c_str(), buf->size());

            if (multiThreaded_) {
                lock_.unlock();
            }
        }

        buf->clear();
    }

private:
    class Impl {
    public:
        Impl(const Logger* parent, LogLevel level, bool enableMutex)
            : parent_(parent)
            , level_(level)
            , enableMutex_(enableMutex)
        {
        }

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

        std::mutex lock_; // protects the following variables
        std::ofstream out_;
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
    Impl loggers_[LogLevelCount];
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

#define LOGGER_WRITE(level_, fmt_, ...) \
    ant::detail::gLogger->Log(level_, FMT_STRING("{}:{}] " fmt_), strrchr("/" __FILE__, '/') + 1, __LINE__, ##__VA_ARGS__)

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
