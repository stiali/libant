#ifndef LIBANT_INCLUDE_LIBANT_HASH_HASH_H_
#define LIBANT_INCLUDE_LIBANT_HASH_HASH_H_

#include <string>

namespace ant {

/**
 * BKDR Hash Function
 */
inline uint64_t BKDRHash(const void* const data, const size_t dataLen)
{
    uint64_t val = 0;
    auto p = reinterpret_cast<const unsigned char*>(data);
    for (size_t i = 0; i != dataLen; ++i) {
        val = val * 131 + static_cast<uint64_t>(p[i]);
    }
    return val;
}

/**
 * R5 Hash Function
 */
inline uint32_t R5Hash(const char* p)
{
    uint32_t h = 0;
    while (*p) {
        h = h * 11 + (*p << 4) + (*p >> 4);
        p++;
    }
    return h;
}

/**
 * R5 Hash Function
 */
inline uint32_t R5Hash(const std::string& s)
{
    uint32_t h = 0;
    for (std::string::size_type i = 0; i != s.size(); ++i) {
        h = h * 11 + (s[i] << 4) + (s[i] >> 4);
    }
    return h;
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_HASH_HASH_H_
