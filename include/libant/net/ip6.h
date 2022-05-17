#ifndef LIBANT_INCLUDE_LIBANT_NET_IP6_H_
#define LIBANT_INCLUDE_LIBANT_NET_IP6_H_

#include <netinet/ip6.h>

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

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_IP6_H_
