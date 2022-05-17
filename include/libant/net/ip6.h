#ifndef LIBANT_INCLUDE_LIBANT_NET_IP6_H_
#define LIBANT_INCLUDE_LIBANT_NET_IP6_H_

#include <netinet/ip6.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>

namespace ant {

/**
 * LocateTcpHeader returns, if ipPacket is a TCP packet, a pointer to the base address of the TCP packet.
 * @param ipPacket
 * @param payloadLength
 * @return Pointer to the base address of a TCP packet, or nullptr if it's not a TCP packet.
 */
const tcphdr* LocateTcpHeader(const ip6_hdr* ipPacket, uint32_t payloadLength);

/**
 * LocateTcpHeader returns, if ipPacket is a TCP packet, a pointer to the base address of the TCP packet.
 * @param ipPacket
 * @param payloadLength
 * @return Pointer to the base address of a TCP packet, or nullptr if it's not a TCP packet.
 */
inline tcphdr* LocateTcpHeader(ip6_hdr* ipPacket, uint32_t payloadLength)
{
    return const_cast<tcphdr*>(LocateTcpHeader(const_cast<const ip6_hdr*>(ipPacket), payloadLength));
}

/**
 * LocateUdpHeader returns, if ipPacket is a UDP packet, a pointer to the base address of the UDP packet.
 * @param ipPacket
 * @param payloadLength
 * @return Pointer to the base address of a UDP packet, or nullptr if it's not a UDP packet.
 */
const udphdr* LocateUdpHeader(const ip6_hdr* ipPacket, uint32_t payloadLength);

/**
 * LocateUdpHeader returns, if ipPacket is a UDP packet, a pointer to the base address of the UDP packet.
 * @param ipPacket
 * @param payloadLength
 * @return Pointer to the base address of a UDP packet, or nullptr if it's not a UDP packet.
 */
inline udphdr* LocateUdpHeader(ip6_hdr* ipPacket, uint32_t payloadLength)
{
    return const_cast<udphdr*>(LocateUdpHeader(const_cast<const ip6_hdr*>(ipPacket), payloadLength));
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_IP6_H_
