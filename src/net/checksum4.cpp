#include <netinet/ip_icmp.h>
#include <netinet/udp.h>

#include <libant/utils/likely.h>
#include <libant/checksum/checksum.h>
#include <libant/net/checksum4.h>

namespace ant {

bool ComputeAndSetChecksum(ip* packet, const size_t packetLen)
{
    if (unlikely(packetLen < sizeof(ip))) {
        return false;
    }

    const auto ipHeaderLen = static_cast<size_t>(packet->ip_hl) << 2;
    const size_t ipPacketLen = ntohs(packet->ip_len);
    if (unlikely(packetLen < ipHeaderLen || packetLen != ipPacketLen)) {
        return false;
    }

    packet->ip_sum = 0;
    packet->ip_sum = ~Checksum16(packet, ipHeaderLen);

    bool ret = false;
    uint8_t* base = reinterpret_cast<uint8_t*>(packet);

    switch (packet->ip_p) {
    case IPPROTO_ICMP:
        if (packetLen >= ipHeaderLen + sizeof(icmp)) {
            auto header = reinterpret_cast<icmp*>(base + ipHeaderLen);
            header->icmp_cksum = 0;
            header->icmp_cksum = ~Checksum16(packet, packetLen);
            ret = true;
        }
        break;
    case IPPROTO_UDP:
        if (packetLen >= ipHeaderLen + sizeof(udphdr)) {
            auto header = reinterpret_cast<udphdr*>(base + ipHeaderLen);
            header->uh_sum = 0;
            ret = true;
        }
        break;
    case IPPROTO_TCP:
        ret = ComputeAndSetChecksum(packet, reinterpret_cast<tcphdr*>(base + ipHeaderLen), packetLen - ipHeaderLen);
        break;
    default:
        break;
    }

    return ret;
}

bool ComputeAndSetChecksum(const ip* ipHeader, tcphdr* tcpHeader, const size_t tcpPacketLen)
{
    if (unlikely(tcpPacketLen < sizeof(tcphdr))) {
        return false;
    }

    uint32_t sum = static_cast<uint16_t>(ipHeader->ip_src.s_addr >> 16);
    sum += static_cast<uint16_t>(ipHeader->ip_src.s_addr);
    sum += static_cast<uint16_t>(ipHeader->ip_dst.s_addr >> 16);
    sum += static_cast<uint16_t>(ipHeader->ip_dst.s_addr);
    sum += htons(IPPROTO_TCP);
    sum += htons(tcpPacketLen);
    tcpHeader->th_sum = 0;
    tcpHeader->th_sum = ~FinishChecksum16(AddChecksum16(sum, tcpHeader, tcpPacketLen));
    return true;
}

} // namespace ant