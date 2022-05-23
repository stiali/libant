#ifndef LIBANT_INCLUDE_LIBANT_NET_UDP_PACKET_H_
#define LIBANT_INCLUDE_LIBANT_NET_UDP_PACKET_H_

#include <netinet/udp.h>

namespace ant {

struct UdpPacket {
    udphdr* Header{nullptr}; // pointer to the base address of the TCP packet
    uint32_t Length{0};      // length in bytes of the TCP packet, including the TCP header
};

struct ConstUdpPacket {
    const udphdr* Header{nullptr}; // pointer to the base address of the TCP packet
    uint32_t Length{0};            // length in bytes of the TCP packet, including the TCP header
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_UDP_PACKET_H_
