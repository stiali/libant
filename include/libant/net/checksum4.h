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

#ifndef LIBANT_INCLUDE_LIBANT_NET_CHECKSUM4_H_
#define LIBANT_INCLUDE_LIBANT_NET_CHECKSUM4_H_

#include <cstddef>
#include <libant/net/detail/ip.h>
#include <libant/net/detail/tcp.h>

namespace ant {

/**
 * Compute and set checksums (ip header checksum and icmp/udp/tcp checksum) for an IPv4 packet
 *
 * @param packet
 * @param packetLen length of the IP packet, including IP header
 *
 * @return true on success, false if the packet is invalid
 */
bool ComputeAndSetChecksum(ip* packet, const uint32_t packetLen);

/**
 * Compute and set checksum for a TCP packet over IPv4
 *
 * @param ipHeader
 * @param tcpHeader
 * @param tcpPacketLen length of the TCP packet, including TCP header
 *
 * @return true on success, false if the packet is invalid
 */
bool ComputeAndSetChecksum(const ip* ipHeader, tcphdr* tcpHeader, const uint32_t tcpPacketLen);

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_CHECKSUM4_H_
