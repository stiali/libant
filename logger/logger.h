#ifndef LIBANT_LOGGER_LOGGER_H_
#define LIBANT_LOGGER_LOGGER_H_

#include <atomic>
#include <fstream>
#include <iostream>
#include <mutex>
#include "fmt/format.h"

namespace ant {

class Logger {
public:
    enum LogLevel {
        LogLevelTrace,
        LogLevelInfo,
        LogLevelWarn,
        LogLevelError,
        LogLevelFatal, // Call exit(-1) after log is written.

        LogLevelCount
    };

    enum LogDest {
        LogDestNone = 0,                            // Don't write logs.
        LogDestFile = 1,                            // Write logs to files.
        LogDestConsole = 2,                         // Write logs to console.
        LogDestBoth = LogDestFile | LogDestConsole, // Write logs both to files and console.
    };

    enum ControlFlag {
        ControlFlagLogThrough = 1,  // Controls if logs with higher level are written to lower level log files.
        ControlFlagLogFuncName = 2, // Controls if function name is prepended to the logs.
        ControlFlagLogLineNum = 4,  // Controls if filename and line number are prepended to the logs.
        ControlFlagLogDate = 8,     // Controls if a date string formatted as '20201201' is prepended to the logs.
    };

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
    // Variables not allowed to be changed at runtime go here
    const std::string logDir_;
    const std::string logPathPrefix_;
    const uint64_t logFileMaxSize_;
    const uint32_t controlFlags_;

    // Variables allowed to be changed at runtime go here
    std::atomic<LogLevel> logLevel_;
    std::atomic<LogDest> logDest_;
};

} // namespace ant

#endif //LIBANT_LOGGER_LOGGER_H_
