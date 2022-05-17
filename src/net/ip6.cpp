#include <libant/net/ip6.h>

namespace ant {

enum {
    kNextHeaderHopByHop = 0,
    kNextHeaderTCP = 6,
    kNextHeaderUDP = 17,
    kNextHeaderIPv6 = 41,
    kNextHeaderRouting = 43,
    kNextHeaderFragment = 44,
    kNextHeaderRSVP = 46,
    kNextHeaderSecurity = 50,
    kNextHeaderAuth = 51,
    kNextHeaderICMP6 = 58,
    kNextHeaderNone = 59,
    kNextHeaderDestOpts = 60,
};

ConstTcpPacket LocateTcpHeader(const ip6_hdr* ipPacket, uint32_t payloadLength)
{
    ConstTcpPacket packet{nullptr, 0};

    auto basePtr = reinterpret_cast<const uint8_t*>(ipPacket) + sizeof(ip6_hdr);
    auto nextHeader = ipPacket->ip6_nxt;
    for (;;) {
        switch (nextHeader) {
        case kNextHeaderTCP:
            if (payloadLength > sizeof(tcphdr)) {
                packet.Header = reinterpret_cast<const tcphdr*>(basePtr);
                packet.Length = payloadLength;
            }
            break;
        case kNextHeaderHopByHop:
        case kNextHeaderRouting:
        case kNextHeaderDestOpts:
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
        case kNextHeaderAuth:
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

ConstUdpPacket LocateUdpHeader(const ip6_hdr* ipPacket, uint32_t payloadLength)
{
    ConstUdpPacket packet{nullptr, 0};

    auto basePtr = reinterpret_cast<const uint8_t*>(ipPacket) + sizeof(ip6_hdr);
    auto nextHeader = ipPacket->ip6_nxt;
    for (;;) {
        switch (nextHeader) {
        case kNextHeaderUDP:
            if (payloadLength > sizeof(udphdr)) {
                packet.Header = reinterpret_cast<const udphdr*>(basePtr);
                packet.Length = payloadLength;
            }
            break;
        case kNextHeaderHopByHop:
        case kNextHeaderRouting:
        case kNextHeaderDestOpts:
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
        case kNextHeaderAuth:
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