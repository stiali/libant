#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <netinet/tcp.h>
#include <libant/checksum/checksum.h>
#include <libant/checksum/ip_checksum.h>

namespace ant {

bool ComputeAndSetIPv4Checksum(ip* packet, const size_t packetLen)
{
    const auto ipHeaderLen = static_cast<size_t>(packet->ip_hl) << 2;
    if (ipHeaderLen > packetLen) {
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
        ret = ComputeAndSetTCPv4Checksum(packet, packetLen, ipHeaderLen);
        break;
    default:
        break;
    }

    return ret;
}

bool ComputeAndSetTCPv4Checksum(ip* packet, const size_t packetLen, const size_t ipHeaderLen)
{
    const size_t ipPacketLen = ntohs(packet->ip_len);
    if (packetLen >= ipPacketLen && packetLen >= ipHeaderLen + sizeof(tcphdr)) {
        uint16_t tcpLen = ipPacketLen - ipHeaderLen;
        uint32_t sum = static_cast<uint16_t>(packet->ip_src.s_addr >> 16);
        sum += static_cast<uint16_t>(packet->ip_src.s_addr);
        sum += static_cast<uint16_t>(packet->ip_dst.s_addr >> 16);
        sum += static_cast<uint16_t>(packet->ip_dst.s_addr);
        sum += htons(IPPROTO_TCP);
        sum += htons(tcpLen);

        auto tcpHeader = reinterpret_cast<tcphdr*>(reinterpret_cast<uint8_t*>(packet) + ipHeaderLen);
        tcpHeader->th_sum = 0;
        tcpHeader->th_sum = ~FinishChecksum16(AddChecksum16(sum, tcpHeader, tcpLen));
        return true;
    }
    return false;
}

} // namespace ant