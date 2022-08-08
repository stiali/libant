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

#ifndef LIBANT_INCLUDE_LIBANT_NET_UDP6_H_
#define LIBANT_INCLUDE_LIBANT_NET_UDP6_H_

#include <libant/net/detail/ip6.h>
#include <libant/net/udp_packet.h>

namespace ant {

/**
 * LocateUdpHeader returns, if ipPacket is a UDP packet, a pointer to the base address of the UDP packet.
 * @param ipPacket pointer to an IPv6 packet
 * @param ipPacketLen length of the IPv6 packet, including the IPv6 header
 * @return Pointer to the base address of a UDP packet, or nullptr if it's not a UDP packet.
 */
ConstUdpPacket LocateUdpHeader(const ip6_hdr* ipPacket, uint32_t ipPacketLen);

/**
 * LocateUdpHeader returns, if ipPacket is a UDP packet, a pointer to the base address of the UDP packet.
 * @param ipPacket pointer to an IPv6 packet
 * @param ipPacketLen length of the IPv6 packet, including the IPv6 header
 * @return Pointer to the base address of a UDP packet, or nullptr if it's not a UDP packet.
 */
inline UdpPacket LocateUdpHeader(ip6_hdr* ipPacket, uint32_t ipPacketLen)
{
    auto packet = LocateUdpHeader(const_cast<const ip6_hdr*>(ipPacket), ipPacketLen);
    return UdpPacket{const_cast<udphdr*>(packet.Header), packet.Length};
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_UDP6_H_
