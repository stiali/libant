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
std::string ToString(const std::vector<T>& v)
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
inline std::string ToString<char*>(const std::vector<char*>& v)
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
inline std::string ToString<const char*>(const std::vector<const char*>& v)
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
inline std::string ToString<std::string>(const std::vector<std::string>& v)
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

/**
 * SplitString splits the given string `src` by `sep`
 *
 * @param src
 * @param sep
 * @param sepLen
 * @return vector of the splitted strings
 */
std::vector<std::string> SplitString(const char* src, const char* sep, size_t sepLen);

/**
 * SplitString splits the given string `src` by `sep`
 *
 * @param src
 * @param sep
 * @return vector of the splitted strings
 */
inline std::vector<std::string> SplitString(const std::string& src, const std::string& sep)
{
    return SplitString(src.c_str(), sep.c_str(), sep.size());
}

/**
 * SplitString splits the given string `src` by `sep`
 *
 * @param src
 * @param sep
 * @return vector of the splitted strings
 */
std::vector<std::string> SplitString(const std::string& src, char sep);

/**
 * TrimStringLeft
 * @param s
 */
inline void TrimStringLeft(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](char ch) { return !std::isspace(ch); }));
}

/**
 * TrimStringRight
 * @param s
 */
inline void TrimStringRight(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](char ch) { return !std::isspace(ch); }).base(), s.end());
}

/**
 * TrimString
 * @param s
 */
inline void TrimString(std::string& s)
{
    TrimStringLeft(s);
    TrimStringRight(s);
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_STRING_UTILS_STRING_UTILS_H_
