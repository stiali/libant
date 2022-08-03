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
