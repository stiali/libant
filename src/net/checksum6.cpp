#include <libant/utils/likely.h>
#include <libant/checksum/checksum.h>
#include <libant/net/checksum6.h>

namespace ant {

bool ComputeAndSetChecksum(const ip6_hdr* ipHeader, tcphdr* tcpHeader, const size_t tcpPacketLen)
{
    if (unlikely(tcpPacketLen < sizeof(tcphdr))) {
        return false;
    }

    uint64_t sum = 0;
    auto addr = reinterpret_cast<const uint32_t*>(ipHeader->ip6_src.s6_addr);
    for (int i = 0; i != 4; ++i) {
        sum += addr[i];
    }

    addr = reinterpret_cast<const uint32_t*>(ipHeader->ip6_dst.s6_addr);
    for (int i = 0; i != 4; ++i) {
        sum += addr[i];
    }

    sum += htonl(tcpPacketLen);
    sum += htonl(IPPROTO_TCP);
    tcpHeader->th_sum = 0;
    tcpHeader->th_sum = ~FinishChecksum16(AddChecksum32(sum, tcpHeader, tcpPacketLen));
    return true;
}

} // namespace ant