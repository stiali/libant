#ifndef LIBANT_INCLUDE_LIBANT_NET_DETAIL_TCP_H_
#define LIBANT_INCLUDE_LIBANT_NET_DETAIL_TCP_H_

#ifndef _WIN32
#include <netinet/tcp.h>
#else
#pragma warning(push)
#pragma warning(disable: 4201)

#include "win/tcp.h"

#pragma warning(pop)
#endif

#endif //LIBANT_INCLUDE_LIBANT_NET_DETAIL_TCP_H_
