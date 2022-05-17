#ifndef LIBANT_INCLUDE_LIBANT_NET_TCP6_H_
#define LIBANT_INCLUDE_LIBANT_NET_TCP6_H_

#include <netinet/tcp.h>
#include <libant/net/ip6.h>

namespace ant {

struct TcpPacket {
    tcphdr* Header;  // pointer to the base address of the TCP packet
    uint32_t Length; // length in bytes of the TCP packet, including the TCP header
};

struct ConstTcpPacket {
    const tcphdr* Header; // pointer to the base address of the TCP packet
    uint32_t Length;      // length in bytes of the TCP packet, including the TCP header
};

/**
 * LocateTcpHeader returns, if ipPacket is a TCP packet, a pointer to the base address of the TCP packet.
 * @param ipPacket pointer to an IPv6 packet
 * @param payloadLength length of the payload, not including the IPv6 header
 * @return Pointer to the base address of a TCP packet, or nullptr if it's not a TCP packet.
 */
ConstTcpPacket LocateTcpHeader(const ip6_hdr* ipPacket, uint32_t payloadLength);

/**
 * LocateTcpHeader returns, if ipPacket is a TCP packet, a pointer to the base address of the TCP packet.
 * @param ipPacket pointer to an IPv6 packet
 * @param payloadLength length of the payload, not including the IPv6 header
 * @return Pointer to the base address of a TCP packet, or nullptr if it's not a TCP packet.
 */
TcpPacket LocateTcpHeader(ip6_hdr* ipPacket, uint32_t payloadLength)
{
    auto packet = LocateTcpHeader(const_cast<const ip6_hdr*>(ipPacket), payloadLength);
    return TcpPacket{const_cast<tcphdr*>(packet.Header), packet.Length};
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_TCP6_H_
