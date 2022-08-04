#ifndef LIBANT_INCLUDE_LIBANT_TIMER_TIMER_H_
#define LIBANT_INCLUDE_LIBANT_TIMER_TIMER_H_

#include <functional>
#include <map>

namespace ant {

class TimerManager {
public:
    void Update(time_t curTimeMS)
    {
        for (auto iter = timers_.begin(); iter != timers_.end(); iter = timers_.erase(iter)) {
            if (iter->first > curTimeMS) {
                break;
            }
            iter->second(curTimeMS);
        }
    }

    void AddTimer(time_t expiringTimeMS, std::function<void(time_t curTimeMS)> cb)
    {
        timers_.emplace(std::make_pair(expiringTimeMS, std::move(cb)));
    }

    void Clear()
    {
        timers_.clear();
    }

public:
    // <ExpiringTime, Callback>
    std::multimap<time_t, std::function<void(time_t curTimeMS)>> timers_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_TIMER_TIMER_H_
