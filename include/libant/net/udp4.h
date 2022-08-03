#ifndef LIBANT_INCLUDE_LIBANT_NET_UDP4_H_
#define LIBANT_INCLUDE_LIBANT_NET_UDP4_H_

#ifndef _WIN32
#include <netinet/ip.h>
#else
#include <libant/net/detail/ip.h>
#endif
#include <libant/net/udp_packet.h>

namespace ant {

/**
 * LocateUdpHeader returns, if ipPacket is a UDP packet, a pointer to the base address of the UDP packet.
 * @param ipPacket pointer to an IPv4 packet
 * @param ipPacketLen length of the IPv4 packet, including the IPv4 header
 * @return Pointer to the base address of a UDP packet, or nullptr if it's not a UDP packet.
 */
ConstUdpPacket LocateUdpHeader(const ip* ipPacket, uint32_t ipPacketLen);

/**
 * LocateUdpHeader returns, if ipPacket is a UDP packet, a pointer to the base address of the UDP packet.
 * @param ipPacket pointer to an IPv4 packet
 * @param ipPacketLen length of the IPv4 packet, including the IPv4 header
 * @return Pointer to the base address of a UDP packet, or nullptr if it's not a UDP packet.
 */
inline UdpPacket LocateUdpHeader(ip* ipPacket, uint32_t ipPacketLen)
{
    auto packet = LocateUdpHeader(const_cast<const ip*>(ipPacket), ipPacketLen);
    return UdpPacket{const_cast<udphdr*>(packet.Header), packet.Length};
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_UDP4_H_
