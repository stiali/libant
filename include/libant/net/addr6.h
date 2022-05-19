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

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_NET_ADDR6_H_
