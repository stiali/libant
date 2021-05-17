#include <libant/system/signal.h>
#include <libant/file_utils/file_purger.h>

using namespace std;
namespace fs = std::filesystem;

namespace ant {

void FilePurger::run()
{
    ThreadBlockAllSignals();

    unordered_map<string, PurgingRule> rules;
    unique_lock<std::mutex> lock(cvMtx_);
    while (!stop_) {
        rulesMtx_.lock();
        rules = rules_;
        rulesMtx_.unlock();

        auto tNow = fs::file_time_type::clock::now();
        for (const auto& rule : rules) {
            purgeOldFiles(tNow, rule.first, rule.second);
        }

        cv_.wait_for(lock, chrono::seconds(300));
    }
}

void FilePurger::purgeOldFiles(const fs::file_time_type& tNow, const std::string& dir, const PurgingRule& rule)
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

        time_t elapsed = chrono::duration_cast<chrono::seconds>(tNow - lastWriteTime).count();
        if (fs::is_regular_file(entry, ec)) {
            if (elapsed > rule.inactiveTime_) {
                fs::remove(entry, ec);
            }
        } else if (fs::is_directory(entry, ec)) {
            if (rule.purgeRecursively_) {
                purgeOldFiles(tNow, entry.path().string(), rule);
            }

            if (rule.purgeEmptyDirectory_ && elapsed > rule.inactiveTime_) {
                fs::remove(entry, ec); // remove a file or an empty directory
            }
        }
    }
}

} // namespace ant