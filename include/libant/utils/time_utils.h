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

#ifndef LIBANT_INCLUDE_LIBANT_TIME_UTILS_TIME_UTILS_H_
#define LIBANT_INCLUDE_LIBANT_TIME_UTILS_TIME_UTILS_H_

#include <ctime>
#include <chrono>

namespace ant {

template<typename Duration>
time_t ToTimestamp(const std::chrono::time_point<std::chrono::steady_clock>& t)
{
    return std::chrono::duration_cast<Duration>(t.time_since_epoch()).count();
}

template<typename Duration>
time_t ToTimestamp(const std::chrono::time_point<std::chrono::system_clock>& t)
{
    return std::chrono::duration_cast<Duration>(t.time_since_epoch()).count();
}

inline time_t ToUnixTime(int year, int mon, int day, int hour, int min, int sec)
{
    tm tm;
    tm.tm_isdst = 0;
    tm.tm_year = year - 1900;
    tm.tm_mon = mon - 1;
    tm.tm_mday = day;
    tm.tm_hour = hour;
    tm.tm_min = min;
    tm.tm_sec = sec;
    return mktime(&tm);
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_TIME_UTILS_TIME_UTILS_H_
