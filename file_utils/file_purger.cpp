#include <filesystem>

#ifndef _WIN32
#include <csignal>
#endif

#include "file_purger.h"

using namespace std;
namespace fs = std::filesystem;

namespace ant {

void FilePurger::run()
{
#ifndef _WIN32
    // block all signals
    sigset_t mask;
    if (sigfillset(&mask) == 0) {
        pthread_sigmask(SIG_BLOCK, &mask, nullptr);
    }
#endif // !_WIN32

    unordered_map<string, PurgingRule> rules;
    unique_lock<std::mutex> lock(cvMtx_);
    while (!stop_) {
        rulesMtx_.lock();
        rules = rules_;
        rulesMtx_.unlock();

        time_t tNow = time(nullptr);
        for (const auto& rule : rules) {
            purgeOldFiles(tNow, rule.first, rule.second);
        }

        cv_.wait_for(lock, chrono::seconds(300));
    }
}

void FilePurger::purgeOldFiles(time_t tNow, const std::string& dir, const PurgingRule& rule)
{
    fs::path p(dir);

    error_code ec;
    auto dirIter = fs::directory_iterator(p, ec);
    if (ec) {
        return;
    }

    for (const auto& entry : dirIter) {
        auto lastWriteTime = fs::last_write_time(entry, ec);
        if (ec) {
            continue;
        }

        time_t lastUpdTime = chrono::duration_cast<chrono::seconds>(lastWriteTime.time_since_epoch()).count();
        if (fs::is_regular_file(entry, ec)) {
            if (tNow - lastUpdTime > rule.inactiveTime_) {
                fs::remove(entry, ec);
            }
        } else if (fs::is_directory(entry, ec)) {
            if (rule.purgeRecursively_) {
                purgeOldFiles(tNow, entry.path().string(), rule);
            }

            if (rule.purgeEmptyDirectory_ && tNow - lastUpdTime > rule.inactiveTime_) {
                fs::remove(entry, ec); // remove a file or an empty directory
            }
        }
    }
}

} // namespace ant