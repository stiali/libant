#ifndef LIBANT_INCLUDE_LIBANT_NET_DETAIL_IP6_H_
#define LIBANT_INCLUDE_LIBANT_NET_DETAIL_IP6_H_

#ifndef _WIN32
#include <netinet/ip6.h>
#else
#pragma warning(push)
#pragma warning(disable: 4200)

#include "win/ip6.h"

#pragma warning(pop)
#endif

#endif //LIBANT_INCLUDE_LIBANT_NET_DETAIL_IP6_H_
