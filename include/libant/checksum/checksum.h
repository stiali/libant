#ifndef LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_
#define LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_

#include <cstddef>
#include <cstdint>

namespace ant {

/**
 * Add checksum of `data` to `currentChecksum`.
 *
 * @param currentChecksum
 * @param data
 * @param dataLen
 * @return checksum of `data` plus `currentChecksum`
 */
uint64_t AddChecksum(uint64_t currentChecksum, const void* data, size_t dataLen);

/**
 * fold the 64-bit checksum into a 32-bit checksum
 *
 * @param currentChecksum
 * @return the folded 32-bit checksum
 */
inline uint32_t FinishChecksum32(uint64_t currentChecksum)
{
    // fold the 64-bit checksum into a 32-bit checksum
    while (currentChecksum > 0xFFFFFFFF) {
        currentChecksum = (currentChecksum >> 32) + (currentChecksum & 0xFFFFFFFF);
    }
    return static_cast<uint32_t>(currentChecksum);
}

/**
 * fold the 64-bit checksum into a 16-bit checksum
 *
 * @param currentChecksum
 * @return the folded 16-bit checksum
 */
inline uint16_t FinishChecksum16(uint64_t currentChecksum)
{
    // fold the 64-bit checksum into a 16-bit checksum
    while (currentChecksum > 0xFFFFFFFF) {
        currentChecksum = (currentChecksum >> 32) + (currentChecksum & 0xFFFFFFFF);
    }
    while (currentChecksum > 0xFFFF) {
        currentChecksum = (currentChecksum >> 16) + (currentChecksum & 0xFFFF);
    }
    return static_cast<uint16_t>(currentChecksum);
}

/**
 * Checksum32 computes a 32-bit checksum of `data`
 *
 * @param data
 * @param dataLen
 *
 * @return a 32-bit checksum of `data`
 */
inline uint32_t Checksum32(const void* data, size_t dataLen)
{
    return FinishChecksum32(AddChecksum(0, data, dataLen));
}

/**
 * Checksum16 computes a 16-bit checksum of `data`
 *
 * @param data
 * @param dataLen
 *
 * @return a 16-bit checksum of `data`
 */
inline uint16_t Checksum16(const void* data, size_t dataLen)
{
    return FinishChecksum16(AddChecksum(0, data, dataLen));
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_
