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

#ifndef LIBANT_INCLUDE_LIBANT_STRING_UTILS_STRING_UTILS_H_
#define LIBANT_INCLUDE_LIBANT_STRING_UTILS_STRING_UTILS_H_

#include <algorithm>
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

/**
 * ToUpper
 * @param s
 */
inline void ToUpper(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), toupper);
}

/**
 * ToUpperCopy
 * @param s
 * @return
 */
inline std::string ToUpperCopy(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), toupper);
    return s;
}

/**
 * ToLower
 * @param s
 */
inline void ToLower(std::string& s)
{
    std::transform(s.begin(), s.end(), s.begin(), tolower);
}

/**
 * ToLowerCopy
 * @param s
 * @return
 */
inline std::string ToLowerCopy(std::string s)
{
    std::transform(s.begin(), s.end(), s.begin(), tolower);
    return s;
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_STRING_UTILS_STRING_UTILS_H_
