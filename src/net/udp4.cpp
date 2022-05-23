#include <libant/utils/likely.h>
#include <libant/net/udp4.h>

namespace ant {

ConstUdpPacket LocateUdpHeader(const ip* ipPacket, uint32_t ipPacketLen)
{
    ConstUdpPacket packet;
    if (unlikely(ipPacketLen <= sizeof(ip) || ipPacket->ip_p != IPPROTO_UDP)) {
        return packet;
    }

    const auto ipHeaderLen = (static_cast<uint32_t>(ipPacket->ip_hl) << 2);
    if (ipPacketLen >= ipHeaderLen + sizeof(udphdr)) {
        packet.Header = reinterpret_cast<const udphdr*>(reinterpret_cast<const char*>(ipPacket) + ipHeaderLen);
        packet.Length = ipPacketLen - ipHeaderLen;
    }
    return packet;
}

} // namespace ant
