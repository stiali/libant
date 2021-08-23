#include <cassert>
#include <libant/checksum/checksum.h>

namespace ant {

/**
 * Checksum16 computes a 16-bit checksum of `data`
 *
 * @param data
 * @param dataLen cannot be longer then 60000 bytes
 *
 * @return a 16-bit checksum of `data`
 */
uint16_t Checksum16(const void* data, size_t dataLen)
{
    assert(dataLen <= 60000);

    uint32_t checksum = 0;

    // add data altogether
    auto data16 = reinterpret_cast<const uint16_t*>(data);
    while (dataLen > 1) {
        checksum += *(data16++);
        dataLen -= 2;
    }
    if (dataLen) {
        checksum += *reinterpret_cast<const uint8_t*>(data16);
    }
    // fold the 32-bit checksum into a 16-bit checksum
    while (checksum > 0xFFFF) {
        checksum = (checksum >> 16) + (checksum & 0xFFFF);
    }

    return static_cast<uint16_t>(checksum);
}

} // namespace ant
