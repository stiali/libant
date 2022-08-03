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
