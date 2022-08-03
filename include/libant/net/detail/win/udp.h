#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define uh_sport source
#define uh_dport dest
#define uh_ulen len
#define uh_sum check

#if defined(_MSC_VER)
#pragma pack(push)
#pragma pack(1)
#else
#pragma pack(1)
#endif

struct udphdr {
    uint16_t uh_sport;
    uint16_t uh_dport;
    uint16_t uh_ulen;
    uint16_t uh_sum;
};

#if defined(_MSC_VER)
#pragma pack(pop)
#else
#pragma pack()
#endif

#define UDP_CORK 1
#define UDP_ENCAP 100
#define UDP_NO_CHECK6_TX 101
#define UDP_NO_CHECK6_RX 102
#define UDP_SEGMENT 103
#define UDP_GRO 104

#define UDP_ENCAP_ESPINUDP_NON_IKE 1
#define UDP_ENCAP_ESPINUDP 2
#define UDP_ENCAP_L2TPINUDP 3
#define UDP_ENCAP_GTP0 4
#define UDP_ENCAP_GTP1U 5
#define UDP_ENCAP_RXRPC 6

#define SOL_UDP 17

#ifdef __cplusplus
}
#endif
