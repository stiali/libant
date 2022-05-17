#include <libant/net/udp6.h>

namespace ant {

ConstUdpPacket LocateUdpHeader(const ip6_hdr* ipPacket, uint32_t payloadLength)
{
    ConstUdpPacket packet{nullptr, 0};

    auto basePtr = reinterpret_cast<const uint8_t*>(ipPacket) + sizeof(ip6_hdr);
    auto nextHeader = ipPacket->ip6_nxt;
    for (;;) {
        switch (nextHeader) {
        case IPPROTO_UDP:
            if (payloadLength > sizeof(udphdr)) {
                packet.Header = reinterpret_cast<const udphdr*>(basePtr);
                packet.Length = payloadLength;
            }
            break;
        case IPPROTO_HOPOPTS:
        case IPPROTO_ROUTING:
        case IPPROTO_DSTOPTS:
            if (payloadLength > 8) {
                auto ext = reinterpret_cast<const ip6_ext*>(basePtr);
                auto extLen = static_cast<uint32_t>(ext->ip6e_len) * 8 + 8;
                if (payloadLength > extLen) {
                    nextHeader = ext->ip6e_nxt;
                    basePtr += extLen;
                    payloadLength -= extLen;
                    continue;
                }
            }
            break;
        case IPPROTO_AH:
            if (payloadLength > 8) {
                auto ext = reinterpret_cast<const ip6_ext*>(basePtr);
                auto extLen = static_cast<uint32_t>(ext->ip6e_len) * 4 + 8;
                if (payloadLength > extLen) {
                    nextHeader = ext->ip6e_nxt;
                    basePtr += extLen;
                    payloadLength -= extLen;
                    continue;
                }
            }
            break;
        default:
            break;
        }
    }

    return packet;
}

} // namespace ant