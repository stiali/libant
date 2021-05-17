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
    for (;;) {
        rulesMtx_.lock();
        rules = rules_;
        rulesMtx_.unlock();

        time_t tNow = time(nullptr);
        for (const auto& rule : rules) {
            purgeOldFiles(tNow, rule.first, rule.second);
        }

        this_thread::sleep_for(chrono::seconds(300));
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
        time_t lastUpdTime = chrono::duration_cast<chrono::seconds>(fs::last_write_time(entry).time_since_epoch()).count();
        if (fs::is_regular_file(entry, ec)) {
            if (tNow - lastUpdTime > rule.inactiveTime_) {
                fs::remove(entry, ec);
            }
        } else if (fs::is_directory(entry, ec)) {
            if (rule.purgeRecursively_) {
                purgeOldFiles(tNow, entry.path().string(), rule);
            }

            if (rule.purgeEmptyDirectory_ && tNow - lastUpdTime > rule.inactiveTime_ && fs::is_empty(entry, ec)) {
                fs::remove(entry, ec);
            }
        }
    }
}

} // namespace ant