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

#ifndef LIBANT_INCLUDE_LIBANT_HASH_HASH_H_
#define LIBANT_INCLUDE_LIBANT_HASH_HASH_H_

#include <string>

namespace ant {

/**
 * BKDR Hash Function
 */
inline uint64_t BKDRHash(const void* const data, const size_t dataLen)
{
    uint64_t val = 0;
    auto p = reinterpret_cast<const unsigned char*>(data);
    for (size_t i = 0; i != dataLen; ++i) {
        val = val * 131 + static_cast<uint64_t>(p[i]);
    }
    return val;
}

/**
 * R5 Hash Function
 */
inline uint32_t R5Hash(const char* p)
{
    uint32_t h = 0;
    while (*p) {
        h = h * 11 + (*p << 4) + (*p >> 4);
        p++;
    }
    return h;
}

/**
 * R5 Hash Function
 */
inline uint32_t R5Hash(const std::string& s)
{
    uint32_t h = 0;
    for (std::string::size_type i = 0; i != s.size(); ++i) {
        h = h * 11 + (s[i] << 4) + (s[i] >> 4);
    }
    return h;
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_HASH_HASH_H_
