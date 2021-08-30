#ifndef LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_
#define LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_

#include <cstddef>

namespace ant {

/**
 * Add checksum of `data` to `currentChecksum`. Total data length of the added data should not exceed 120000 bytes.
 *
 * @param currentChecksum
 * @param data
 * @param dataLen
 * @return checksum of `data` plus `currentChecksum`
 */
uint32_t AddChecksum16(uint32_t currentChecksum, const void* data, size_t dataLen);

/**
 * fold the 32-bit checksum into a 16-bit checksum
 *
 * @param currentChecksum
 * @return the folded 16-bit checksum
 */
inline uint16_t FinishChecksum16(uint32_t currentChecksum)
{
    // fold the 32-bit checksum into a 16-bit checksum
    while (currentChecksum > 0xFFFF) {
        currentChecksum = (currentChecksum >> 16) + (currentChecksum & 0xFFFF);
    }
    return static_cast<uint16_t>(currentChecksum);
}

/**
 * Checksum16 computes a 16-bit checksum of `data`
 *
 * @param data
 * @param dataLen cannot be longer then 120000 bytes
 *
 * @return a 16-bit checksum of `data`
 */
inline uint16_t Checksum16(const void* data, size_t dataLen)
{
    return FinishChecksum16(AddChecksum16(0, data, dataLen));
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_
