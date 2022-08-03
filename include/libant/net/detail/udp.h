#ifndef LIBANT_INCLUDE_LIBANT_NET_DETAIL_UDP_H_
#define LIBANT_INCLUDE_LIBANT_NET_DETAIL_UDP_H_

#ifndef _WIN32
#include <netinet/udp.h>
#else
#include "win/udp.h"
#endif

#endif //LIBANT_INCLUDE_LIBANT_NET_DETAIL_UDP_H_
