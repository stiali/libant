#ifndef LIBANT_INCLUDE_LIBANT_NET_UDP6_H_
#define LIBANT_INCLUDE_LIBANT_NET_UDP6_H_

#include <netinet/ip6.h>
#include <netinet/udp.h>

namespace ant {

struct UdpPacket {
    udphdr* Header;  // pointer to the base address of the TCP packet
    uint32_t Length; // length in bytes of the TCP packet, including the TCP header
};

struct ConstUdpPacket {
    const udphdr* Header; // pointer to the base address of the TCP packet
    uint32_t Length;      // length in bytes of the TCP packet, including the TCP header
};

/**
 * LocateUdpHeader returns, if ipPacket is a UDP packet, a pointer to the base address of the UDP packet.
 * @param ipPacket pointer to an IPv6 packet
 * @param payloadLength length of the payload, not including the IPv6 header
 * @return Pointer to the base address of a UDP packet, or nullptr if it's not a UDP packet.
 */
ConstUdpPacket LocateUdpHeader(const ip6_hdr* ipPacket, uint32_t payloadLength);

/**
 * LocateUdpHeader returns, if ipPacket is a UDP packet, a pointer to the base address of the UDP packet.
 * @param ipPacket pointer to an IPv6 packet
 * @param payloadLength length of the payload, not including the IPv6 header
 * @return Pointer to the base address of a UDP packet, or nullptr if it's not a UDP packet.
 */
inline UdpPacket LocateUdpHeader(ip6_hdr* ipPacket, uint32_t payloadLength)
{
    auto packet = LocateUdpHeader(const_cast<const ip6_hdr*>(ipPacket), payloadLength);
    return UdpPacket{const_cast<udphdr*>(packet.Header), packet.Length};
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_UDP6_H_
