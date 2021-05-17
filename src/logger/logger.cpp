#include <filesystem>

#include <libant/logger/logger.h>

using namespace std;

namespace ant {

thread_local std::string Logger::thrBuf_;

namespace detail {

std::unique_ptr<Logger> gLogger;

}

//==========================================================================================
// Logger::Impl Private Methods
//==========================================================================================
static const string sLogLevelNames[Logger::LogLevelCount] = {"TRACE", "INFO", "WARN", "ERROR", "FATAL"};

bool Logger::Impl::log(const tm& tmNow, uint32_t microSeconds, const string& content)
{
    if (curFileSize_ >= parent_->logFileMaxSize_ || curDay_ != tmNow.tm_yday || !out_) {
        out_.close();

        error_code ec;
        filesystem::create_directories(parent_->logDir_, ec);
        if (ec) {
            return false;
        }

        auto filename = fmt::format("{}.{}.{:%Y%m%d%H%M%S}{:06d}.log", parent_->logPathPrefix_, sLogLevelNames[level_], tmNow, microSeconds);
        out_.open(filename, ofstream::app);
        if (!out_) {
            return false;
        }

        curDay_ = tmNow.tm_yday;
        curFileSize_ = 0;
    }

    if (out_.write(content.c_str(), content.size())) {
        curFileSize_ += content.size();
        return true;
    }

    return false;
}

} // namespace ant