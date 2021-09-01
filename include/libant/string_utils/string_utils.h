#ifndef LIBANT_INCLUDE_LIBANT_STRING_UTILS_STRING_UTILS_H_
#define LIBANT_INCLUDE_LIBANT_STRING_UTILS_STRING_UTILS_H_

#include <string>
#include <vector>

namespace ant {

/**
 * Converts a vector to string formatted as `[v1, v2, v3...]`
 *
 * @tparam T
 * @param v
 * @return
 */
template<typename T>
std::string to_string(const std::vector<T>& v)
{
    std::string s = "[";
    for (const auto& vv : v) {
        s.append(std::to_string(vv)).append(" ");
    }
    if (s.size() > 1) {
        s[s.size() - 1] = ']';
    } else {
        s.append("]");
    }
    return s;
}

template<>
inline std::string to_string<char*>(const std::vector<char*>& v)
{
    std::string s = "[";
    for (const auto& vv : v) {
        s.append("\"").append(vv).append("\" ");
    }
    if (s.size() > 1) {
        s[s.size() - 1] = ']';
    } else {
        s.append("]");
    }
    return s;
}

template<>
inline std::string to_string<const char*>(const std::vector<const char*>& v)
{
    std::string s = "[";
    for (const auto& vv : v) {
        s.append("\"").append(vv).append("\" ");
    }
    if (s.size() > 1) {
        s[s.size() - 1] = ']';
    } else {
        s.append("]");
    }
    return s;
}

template<>
inline std::string to_string<std::string>(const std::vector<std::string>& v)
{
    std::string s = "[";
    for (const auto& vv : v) {
        s.append("\"").append(vv).append("\" ");
    }
    if (s.size() > 1) {
        s[s.size() - 1] = ']';
    } else {
        s.append("]");
    }
    return s;
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_STRING_UTILS_STRING_UTILS_H_
