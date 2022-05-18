#ifndef LIBANT_INCLUDE_LIBANT_NET_CHECKSUM6_H_
#define LIBANT_INCLUDE_LIBANT_NET_CHECKSUM6_H_

#include <cstddef>
#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

namespace ant {

/**
 * Compute and set checksum for a TCP packet over IPv6
 *
 * @param ipHeader
 * @param tcpHeader
 * @param tcpPacketLen length of the TCP packet, including TCP header
 *
 * @return true on success, false if the packet is invalid
 */
bool ComputeAndSetChecksum(const ip6_hdr* ipHeader, tcphdr* tcpHeader, const size_t tcpPacketLen);

/**
 * Compute and set checksum for a UDP packet over IPv6
 *
 * @param ipHeader
 * @param udpHeader
 * @param udpPacketLen length of the UDP packet, including UDP header
 *
 * @return true on success, false if the packet is invalid
 */
bool ComputeAndSetChecksum(const ip6_hdr* ipHeader, udphdr* udpHeader, const size_t udpPacketLen);

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_CHECKSUM6_H_
