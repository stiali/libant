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

#ifndef LIBANT_INCLUDE_LIBANT_NET_ADDR6_H_
#define LIBANT_INCLUDE_LIBANT_NET_ADDR6_H_

#include <cstring>
#include <functional>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace std {

// make in6_addr usable for std::unordered_map and std::unordered_set
template<>
struct hash<in6_addr> {
    size_t operator()(const in6_addr& k) const
    {
        return *reinterpret_cast<const size_t*>(k.s6_addr + 8);
    }
};

} // namespace std

inline bool operator==(const in6_addr& lhs, const in6_addr& rhs)
{
    return memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
}

namespace ant {

inline std::string ToString(const in6_addr& addr)
{
    char buf[INET6_ADDRSTRLEN];
    if (inet_ntop(AF_INET6, &addr, buf, sizeof(buf))) {
        return std::string(buf);
    }
    return std::string();
}

inline bool IsV4Mapped(const in6_addr& addr)
{
    return ((addr.s6_addr[0] == 0) && (addr.s6_addr[1] == 0) && (addr.s6_addr[2] == 0) && (addr.s6_addr[3] == 0) && (addr.s6_addr[4] == 0) &&
            (addr.s6_addr[5] == 0) && (addr.s6_addr[6] == 0) && (addr.s6_addr[7] == 0) && (addr.s6_addr[8] == 0) && (addr.s6_addr[9] == 0) &&
            (addr.s6_addr[10] == 0xff) && (addr.s6_addr[11] == 0xff));
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_ADDR6_H_
