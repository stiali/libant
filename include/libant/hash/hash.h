#ifndef LIBANT_INCLUDE_LIBANT_HASH_HASH_H_
#define LIBANT_INCLUDE_LIBANT_HASH_HASH_H_

#include <cstdint>

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

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_HASH_HASH_H_
