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

#ifndef LIBANT_INCLUDE_LIBANT_NET_IP4_H_
#define LIBANT_INCLUDE_LIBANT_NET_IP4_H_

#include <libant/net/detail/ip.h>

namespace ant {

/**
 * GetSourcePort returns source port in network endian of `packet` if `packet` is a UDP or TCP packet
 *
 * @param packet IPv4 packet
 * @param packetLen packet length
 * @return source port in network endian if `packet` is a UDP or TCP packet, 0 otherwise
 */
uint16_t GetSourcePort(const struct ip* packet, uint32_t packetLen);

/**
 * GetDestinationPort returns destination port in network endian of `packet` if `packet` is a UDP or TCP packet
 *
 * @param packet IPv4 packet
 * @param packetLen packet length
 * @return destination port in network endian if `packet` is a UDP or TCP packet, 0 otherwise
 */
uint16_t GetDestinationPort(const struct ip* packet, uint32_t packetLen);

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_IP4_H_
