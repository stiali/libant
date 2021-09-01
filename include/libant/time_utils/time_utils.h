#ifndef LIBANT_INCLUDE_LIBANT_TIME_UTILS_TIME_UTILS_H_
#define LIBANT_INCLUDE_LIBANT_TIME_UTILS_TIME_UTILS_H_

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

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_TIME_UTILS_TIME_UTILS_H_
