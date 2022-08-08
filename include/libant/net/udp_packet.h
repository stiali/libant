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

#ifndef LIBANT_INCLUDE_LIBANT_NET_UDP_PACKET_H_
#define LIBANT_INCLUDE_LIBANT_NET_UDP_PACKET_H_

#include <libant/net/detail/udp.h>

namespace ant {

struct UdpPacket {
    udphdr* Header{nullptr}; // pointer to the base address of the TCP packet
    uint32_t Length{0};      // length in bytes of the TCP packet, including the TCP header
};

struct ConstUdpPacket {
    const udphdr* Header{nullptr}; // pointer to the base address of the TCP packet
    uint32_t Length{0};            // length in bytes of the TCP packet, including the TCP header
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_UDP_PACKET_H_
