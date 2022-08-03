#ifndef LIBANT_INCLUDE_LIBANT_NET_DETAIL_TCP_H_
#define LIBANT_INCLUDE_LIBANT_NET_DETAIL_TCP_H_

#ifndef _WIN32
#include <netinet/tcp.h>
#else
#include "win/tcp.h"
#endif

#endif //LIBANT_INCLUDE_LIBANT_NET_DETAIL_TCP_H_
