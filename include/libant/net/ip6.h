#ifndef LIBANT_INCLUDE_LIBANT_NET_IP6_H_
#define LIBANT_INCLUDE_LIBANT_NET_IP6_H_

#ifndef _WIN32
#include <netinet/ip6.h>
#else
#include <libant/net/detail/ip6.h>
#endif

namespace ant {

/**
 * GetSourcePort returns source port in network endian of `packet` if `packet` is a UDP or TCP packet
 *
 * @param packet IPv6 packet
 * @param packetLen packet length
 * @return source port in network endian if `packet` is a UDP or TCP packet, 0 otherwise
 */
uint16_t GetSourcePort(const ip6_hdr* packet, uint32_t packetLen);

/**
 * GetDestinationPort returns destination port in network endian of `packet` if `packet` is a UDP or TCP packet
 *
 * @param packet IPv6 packet
 * @param packetLen packet length
 * @return destination port in network endian if `packet` is a UDP or TCP packet, 0 otherwise
 */
uint16_t GetDestinationPort(const ip6_hdr* packet, uint32_t packetLen);

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_IP6_H_
