#ifndef LIBANT_INCLUDE_LIBANT_NET_ADDR6_H_
#define LIBANT_INCLUDE_LIBANT_NET_ADDR6_H_

#include <cstring>
#include <functional>
#include <string>
#include <arpa/inet.h>
#include <netinet/in.h>

namespace std {

// make in6_addr usable for std::unordered_map and std::unordered_set
template<>
struct hash<in6_addr> {
    size_t operator()(const in6_addr& k) const
    {
        return *reinterpret_cast<const size_t*>(k.s6_addr + 8);
    }
};

} // namespace std

inline bool operator==(const in6_addr& lhs, const in6_addr& rhs)
{
    return memcmp(&lhs, &rhs, sizeof(lhs)) == 0;
}

namespace ant {

inline std::string ToString(const in6_addr& addr)
{
    char buf[INET6_ADDRSTRLEN];
    if (inet_ntop(AF_INET6, &addr, buf, sizeof(buf))) {
        return std::string(buf);
    }
    return std::string();
}

inline bool IsV4Mapped(const in6_addr& addr)
{
    return ((addr.s6_addr[0] == 0) && (addr.s6_addr[1] == 0) && (addr.s6_addr[2] == 0) && (addr.s6_addr[3] == 0) && (addr.s6_addr[4] == 0) &&
            (addr.s6_addr[5] == 0) && (addr.s6_addr[6] == 0) && (addr.s6_addr[7] == 0) && (addr.s6_addr[8] == 0) && (addr.s6_addr[9] == 0) &&
            (addr.s6_addr[10] == 0xff) && (addr.s6_addr[11] == 0xff));
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_ADDR6_H_
