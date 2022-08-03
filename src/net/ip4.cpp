#ifndef _WIN32
#include <netinet/tcp.h>
#include <netinet/udp.h>
#else
#include <libant/net/detail/tcp.h>
#include <libant/net/detail/udp.h>
#endif
#include <libant/utils/likely.h>
#include <libant/net/ip4.h>

namespace ant {

uint16_t GetSourcePort(const struct ip* packet, uint32_t packetLen)
{
    if (unlikely(packetLen <= sizeof(ip))) {
        return 0;
    }

    const auto ipHeaderLen = (static_cast<uint32_t>(packet->ip_hl) << 2);
    if (unlikely(packetLen <= ipHeaderLen)) {
        return 0;
    }

    switch (packet->ip_p) {
    case IPPROTO_UDP:
        if (packetLen >= ipHeaderLen + sizeof(udphdr)) {
            return reinterpret_cast<const udphdr*>(reinterpret_cast<const char*>(packet) + ipHeaderLen)->uh_sport;
        }
        return 0;
    case IPPROTO_TCP:
        if (packetLen >= ipHeaderLen + sizeof(tcphdr)) {
            return reinterpret_cast<const tcphdr*>(reinterpret_cast<const char*>(packet) + ipHeaderLen)->th_sport;
        }
        return 0;
    default:
        return 0;
    }
}

uint16_t GetDestinationPort(const struct ip* packet, uint32_t packetLen)
{
    if (unlikely(packetLen <= sizeof(ip))) {
        return 0;
    }

    const auto ipHeaderLen = (static_cast<uint32_t>(packet->ip_hl) << 2);
    if (unlikely(packetLen <= ipHeaderLen)) {
        return 0;
    }

    switch (packet->ip_p) {
    case IPPROTO_UDP:
        if (packetLen >= ipHeaderLen + sizeof(udphdr)) {
            return reinterpret_cast<const udphdr*>(reinterpret_cast<const char*>(packet) + ipHeaderLen)->uh_dport;
        }
        return 0;
    case IPPROTO_TCP:
        if (packetLen >= ipHeaderLen + sizeof(tcphdr)) {
            return reinterpret_cast<const tcphdr*>(reinterpret_cast<const char*>(packet) + ipHeaderLen)->th_dport;
        }
        return 0;
    default:
        return 0;
    }
}

} // namespace ant