#ifndef LIBANT_INCLUDE_LIBANT_NET_CHECKSUM4_H_
#define LIBANT_INCLUDE_LIBANT_NET_CHECKSUM4_H_

#include <cstddef>
#include <netinet/ip.h>
#include <netinet/tcp.h>

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
