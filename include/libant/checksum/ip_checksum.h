#ifndef LIBANT_INCLUDE_LIBANT_CHECKSUM_IP_CHECKSUM_H_
#define LIBANT_INCLUDE_LIBANT_CHECKSUM_IP_CHECKSUM_H_

#include <cstddef>

struct ip;

namespace ant {

/**
 * Compute and set checksums (ip header checksum and icmp/udp/tcp checksum) for an IPv4 packet
 *
 * @param packet
 * @param packetLen
 *
 * @return true on success, false if the packet is invalid
 */
bool ComputeAndSetIPv4Checksum(ip* packet, const size_t packetLen);

/**
 * Compute and set checksum for a TCP packet
 *
 * @param packet
 * @param packetLen
 * @param ipHeaderLen
 *
 * @return true on success, false if the packet is invalid
 */
bool ComputeAndSetTCPv4Checksum(ip* packet, const size_t packetLen, const size_t ipHeaderLen);

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_CHECKSUM_IP_CHECKSUM_H_
