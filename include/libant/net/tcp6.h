#ifndef LIBANT_INCLUDE_LIBANT_NET_TCP6_H_
#define LIBANT_INCLUDE_LIBANT_NET_TCP6_H_

#include <libant/net/detail/ip6.h>
#include <libant/net/detail/tcp.h>

namespace ant {

struct TcpPacket {
    tcphdr* Header{nullptr}; // pointer to the base address of the TCP packet
    uint32_t Length{0};      // length in bytes of the TCP packet, including the TCP header
};

struct ConstTcpPacket {
    const tcphdr* Header{nullptr}; // pointer to the base address of the TCP packet
    uint32_t Length{0};            // length in bytes of the TCP packet, including the TCP header
};

/**
 * LocateTcpHeader returns, if ipPacket is a TCP packet, a pointer to the base address of the TCP packet.
 * @param ipPacket pointer to an IPv6 packet
 * @param ipPacketLen length of the IPv6 packet, including the IPv6 header
 * @return Pointer to the base address of a TCP packet, or nullptr if it's not a TCP packet.
 */
ConstTcpPacket LocateTcpHeader(const ip6_hdr* ipPacket, uint32_t ipPacketLen);

/**
 * LocateTcpHeader returns, if ipPacket is a TCP packet, a pointer to the base address of the TCP packet.
 * @param ipPacket pointer to an IPv6 packet
 * @param ipPacketLen length of the IPv6 packet, including the IPv6 header
 * @return Pointer to the base address of a TCP packet, or nullptr if it's not a TCP packet.
 */
inline TcpPacket LocateTcpHeader(ip6_hdr* ipPacket, uint32_t ipPacketLen)
{
    auto packet = LocateTcpHeader(const_cast<const ip6_hdr*>(ipPacket), ipPacketLen);
    return TcpPacket{const_cast<tcphdr*>(packet.Header), packet.Length};
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_TCP6_H_
