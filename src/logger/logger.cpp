#include <filesystem>

#ifndef _WIN32
#include <fcntl.h>
#endif

#include <libant/logger/logger.h>

using namespace std;

namespace ant {

thread_local std::string Logger::thrBuf_;

namespace detail {

std::unique_ptr<Logger> gLogger;

}

//==========================================================================================
// Logger Public Methods
//==========================================================================================

Logger::Logger(const Cfg& cfg)
    : logDir_(cfg.logDir_)
    , logPathPrefix_(!cfg.logDir_.empty() ? cfg.logDir_ + filesystem::path::preferred_separator + cfg.logFilenamePrefix_ : cfg.logFilenamePrefix_)
    , logFileMaxSize_(cfg.logFileMaxSize_ ? cfg.logFileMaxSize_ * 1024 * 1024 : 0xFFFFFFFFFF000000)
    , controlFlags_(cfg.controlFlags_)
    , multiThreaded_(cfg.enableThreadMutex_)
    , logLevel_(cfg.logLevel_)
    , logDest_(cfg.logDest_)
    , loggers_{std::make_unique<Impl>(this, cfg.logFilenamePrefix_, LogLevelTrace, cfg.enableThreadMutex_),
               std::make_unique<Impl>(this, cfg.logFilenamePrefix_, LogLevelInfo, cfg.enableThreadMutex_),
               std::make_unique<Impl>(this, cfg.logFilenamePrefix_, LogLevelWarn, cfg.enableThreadMutex_),
               std::make_unique<Impl>(this, cfg.logFilenamePrefix_, LogLevelError, cfg.enableThreadMutex_),
               std::make_unique<Impl>(this, cfg.logFilenamePrefix_, LogLevelFatal, cfg.enableThreadMutex_)}
{
}

//==========================================================================================

static const string sLogLevelNames[Logger::LogLevelCount] = {"TRACE", "INFO", "WARN", "ERROR", "FATAL"};

//==========================================================================================
// Logger::Impl Public Methods
//==========================================================================================

Logger::Impl::Impl(const Logger* parent, const std::string& filenamePrefix, LogLevel level, bool enableMutex)
    : parent_(parent)
    , level_(level)
    , enableMutex_(enableMutex)
    , symlink_((!parent_->logDir_.empty() ? parent_->logDir_ + filesystem::path::preferred_separator : "") + sLogLevelNames[level] + "." + filenamePrefix +
               ".log")
{
}

//==========================================================================================
// Logger::Impl Private Methods
//==========================================================================================

bool Logger::Impl::log(const tm& tmNow, uint32_t microSeconds, const string& content)
{
#ifdef _WIN32
    if (curFileSize_ >= parent_->logFileMaxSize_ || curDay_ != tmNow.tm_yday || !out_) {
        out_.close();
#else
    if (curFileSize_ >= parent_->logFileMaxSize_ || curDay_ != tmNow.tm_yday || (outFD_ == -1)) {
        close(outFD_);
        outFD_ = -1;
#endif
        error_code ec;
        if (!parent_->logDir_.empty()) {
            filesystem::create_directories(parent_->logDir_, ec);
            if (ec) {
                return false;
            }
        }

        auto filename = fmt::format("{}.{}.{:%Y%m%d%H%M%S}{:06d}.log", parent_->logPathPrefix_, sLogLevelNames[level_], tmNow, microSeconds);
#ifdef _WIN32
        out_.open(filename, ofstream::app);
        if (!out_) {
            return false;
        }
#else
        outFD_ = open(filename.c_str(), O_WRONLY | O_CREAT | O_APPEND, 0644);
        if (outFD_ == -1) {
            return false;
        }
#endif

        filesystem::remove(symlink_, ec);
        if (!(parent_->controlFlags_ & ControlFlagNoSymlinks)) {
            filesystem::create_symlink(filesystem::path(filename).filename(), symlink_, ec);
        }

        curDay_ = tmNow.tm_yday;
        curFileSize_ = 0;
    }

#ifdef _WIN32
    if (out_.write(content.c_str(), content.size())) {
        out_.flush();
#else
    if (write(outFD_, content.c_str(), content.size()) >= 0) {
#endif
        curFileSize_ += content.size();
        return true;
    }

    close(outFD_);
    outFD_ = -1;
    return false;
}

} // namespace ant