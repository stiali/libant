#include <string>
#include <libant/utils/os.h>
#include <libant/checksum/checksum.h>

namespace ant {

uint64_t AddChecksum(uint64_t currentChecksum, const void* data, size_t dataLen)
{
#ifdef LIBANT_NO_UNALIGNED_ACCESS
    std::string tmpBuf;
    if (reinterpret_cast<uint64_t>(data) % 4) {
        tmpBuf.append(reinterpret_cast<const char*>(data), dataLen);
        data = tmpBuf.c_str();
    }
#endif
    // add data altogether
    auto data32 = reinterpret_cast<const uint32_t*>(data);
    while (dataLen > 3) {
        currentChecksum += *(data32++);
        dataLen -= 4;
    }
    auto data16 = reinterpret_cast<const uint16_t*>(data32);
    if (dataLen > 1) {
        currentChecksum += *(data16++);
        dataLen -= 2;
    }
    if (dataLen) {
        currentChecksum += *reinterpret_cast<const uint8_t*>(data16);
    }
    return currentChecksum;
}

} // namespace ant
