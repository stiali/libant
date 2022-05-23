#include <libant/utils/likely.h>
#include <libant/net/tcp6.h>

namespace ant {

ConstTcpPacket LocateTcpHeader(const ip6_hdr* ipPacket, uint32_t ipPacketLen)
{
    ConstTcpPacket packet;
    if (unlikely(ipPacketLen <= sizeof(ip6_hdr))) {
        return packet;
    }

    ipPacketLen -= sizeof(ip6_hdr);
    auto basePtr = reinterpret_cast<const uint8_t*>(ipPacket) + sizeof(ip6_hdr);
    auto nextHeader = ipPacket->ip6_nxt;
    for (;;) {
        switch (nextHeader) {
        case IPPROTO_TCP:
            if (ipPacketLen >= sizeof(tcphdr)) {
                packet.Header = reinterpret_cast<const tcphdr*>(basePtr);
                packet.Length = ipPacketLen;
            }
            return packet;
        case IPPROTO_HOPOPTS:
        case IPPROTO_ROUTING:
        case IPPROTO_DSTOPTS:
            if (ipPacketLen > 8) {
                auto ext = reinterpret_cast<const ip6_ext*>(basePtr);
                auto extLen = static_cast<uint32_t>(ext->ip6e_len) * 8 + 8;
                if (ipPacketLen > extLen) {
                    nextHeader = ext->ip6e_nxt;
                    basePtr += extLen;
                    ipPacketLen -= extLen;
                    continue;
                }
            }
            return packet;
        case IPPROTO_AH:
            if (ipPacketLen > 8) {
                auto ext = reinterpret_cast<const ip6_ext*>(basePtr);
                auto extLen = static_cast<uint32_t>(ext->ip6e_len) * 4 + 8;
                if (ipPacketLen > extLen) {
                    nextHeader = ext->ip6e_nxt;
                    basePtr += extLen;
                    ipPacketLen -= extLen;
                    continue;
                }
            }
            return packet;
        default:
            return packet;
        }
    }
}

} // namespace ant