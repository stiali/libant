#ifndef LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_
#define LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_

#include <cstdint>

namespace ant {

/**
 * Checksum16 computes a 16-bit checksum of `data`
 *
 * @param data
 * @param dataLen cannot be longer then 60000 bytes
 *
 * @return a 16-bit checksum of `data`
 */
uint16_t Checksum16(const void* data, size_t dataLen);

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_CHECKSUM_CHECKSUM_H_
