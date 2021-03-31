#ifndef LIBANT_ENCODING_BINARY_ENDIAN_H_
#define LIBANT_ENCODING_BINARY_ENDIAN_H_

#include <cstdint>

#ifdef _WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

namespace ant {

inline int8_t BigEndianToHost(int8_t v)
{
    return v;
}

inline uint8_t BigEndianToHost(uint8_t v)
{
    return v;
}

inline int16_t BigEndianToHost(int16_t v)
{
    return ntohs(v);
}

inline uint16_t BigEndianToHost(uint16_t v)
{
    return ntohs(v);
}

inline int32_t BigEndianToHost(int32_t v)
{
    return ntohl(v);
}

inline uint32_t BigEndianToHost(uint32_t v)
{
    return ntohl(v);
}

inline int64_t BigEndianToHost(int64_t v)
{
#ifdef __linux__
    return be64toh(v);
#else
    return ntohll(v);
#endif
}

inline uint64_t BigEndianToHost(uint64_t v)
{
#ifdef __linux__
    return be64toh(v);
#else
    return ntohll(v);
#endif
}

inline int8_t HostToBigEndian(int8_t v)
{
    return v;
}

inline uint8_t HostToBigEndian(uint8_t v)
{
    return v;
}

inline int16_t HostToBigEndian(int16_t v)
{
    return htons(v);
}

inline uint16_t HostToBigEndian(uint16_t v)
{
    return htons(v);
}

inline int32_t HostToBigEndian(int32_t v)
{
    return htonl(v);
}

inline uint32_t HostToBigEndian(uint32_t v)
{
    return htonl(v);
}

inline int64_t HostToBigEndian(int64_t v)
{
#ifdef __linux__
    return htobe64(v);
#else
    return htonll(v);
#endif
}

inline uint64_t HostToBigEndian(uint64_t v)
{
#ifdef __linux__
    return htobe64(v);
#else
    return htonll(v);
#endif
}

} // namespace ant

#endif //LIBANT_ENCODING_BINARY_ENDIAN_H_
