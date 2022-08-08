/*
*
* LibAnt - A handy C++ library
* Copyright (C) 2022 Antigloss Huang (https://github.com/antigloss) All rights reserved.
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
