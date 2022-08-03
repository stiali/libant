#ifndef _WIN32
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#else
#include <libant/net/detail/in.h>
#include <libant/net/detail/tcp.h>
#include <libant/net/detail/udp.h>
#endif
#include <libant/utils/likely.h>
#include <libant/net/ip6.h>

namespace ant {

uint16_t GetSourcePort(const ip6_hdr* packet, uint32_t packetLen)
{
    if (unlikely(packetLen <= sizeof(ip6_hdr))) {
        return 0;
    }

    packetLen -= sizeof(ip6_hdr);
    auto basePtr = reinterpret_cast<const uint8_t*>(packet) + sizeof(ip6_hdr);
    auto nextHeader = packet->ip6_nxt;
    for (;;) {
        switch (nextHeader) {
        case IPPROTO_TCP:
            if (packetLen >= sizeof(tcphdr)) {
                return reinterpret_cast<const tcphdr*>(basePtr)->th_sport;
            }
            return 0;
        case IPPROTO_UDP:
            if (packetLen >= sizeof(udphdr)) {
                return reinterpret_cast<const udphdr*>(basePtr)->uh_sport;
            }
            return 0;
        case IPPROTO_HOPOPTS:
        case IPPROTO_ROUTING:
        case IPPROTO_DSTOPTS:
            if (packetLen > 8) {
                auto ext = reinterpret_cast<const ip6_ext*>(basePtr);
                auto extLen = static_cast<uint32_t>(ext->ip6e_len) * 8 + 8;
                if (packetLen > extLen) {
                    nextHeader = ext->ip6e_nxt;
                    basePtr += extLen;
                    packetLen -= extLen;
                    continue;
                }
            }
            return 0;
        case IPPROTO_AH:
            if (packetLen > 8) {
                auto ext = reinterpret_cast<const ip6_ext*>(basePtr);
                auto extLen = static_cast<uint32_t>(ext->ip6e_len) * 4 + 8;
                if (packetLen > extLen) {
                    nextHeader = ext->ip6e_nxt;
                    basePtr += extLen;
                    packetLen -= extLen;
                    continue;
                }
            }
            return 0;
        default:
            return 0;
        }
    }
}

uint16_t GetDestinationPort(const ip6_hdr* packet, uint32_t packetLen)
{
    if (unlikely(packetLen <= sizeof(ip6_hdr))) {
        return 0;
    }

    packetLen -= sizeof(ip6_hdr);
    auto basePtr = reinterpret_cast<const uint8_t*>(packet) + sizeof(ip6_hdr);
    auto nextHeader = packet->ip6_nxt;
    for (;;) {
        switch (nextHeader) {
        case IPPROTO_TCP:
            if (packetLen >= sizeof(tcphdr)) {
                return reinterpret_cast<const tcphdr*>(basePtr)->th_dport;
            }
            return 0;
        case IPPROTO_UDP:
            if (packetLen >= sizeof(udphdr)) {
                return reinterpret_cast<const udphdr*>(basePtr)->uh_dport;
            }
            return 0;
        case IPPROTO_HOPOPTS:
        case IPPROTO_ROUTING:
        case IPPROTO_DSTOPTS:
            if (packetLen > 8) {
                auto ext = reinterpret_cast<const ip6_ext*>(basePtr);
                auto extLen = static_cast<uint32_t>(ext->ip6e_len) * 8 + 8;
                if (packetLen > extLen) {
                    nextHeader = ext->ip6e_nxt;
                    basePtr += extLen;
                    packetLen -= extLen;
                    continue;
                }
            }
            return 0;
        case IPPROTO_AH:
            if (packetLen > 8) {
                auto ext = reinterpret_cast<const ip6_ext*>(basePtr);
                auto extLen = static_cast<uint32_t>(ext->ip6e_len) * 4 + 8;
                if (packetLen > extLen) {
                    nextHeader = ext->ip6e_nxt;
                    basePtr += extLen;
                    packetLen -= extLen;
                    continue;
                }
            }
            return 0;
        default:
            return 0;
        }
    }
}

} // namespace ant