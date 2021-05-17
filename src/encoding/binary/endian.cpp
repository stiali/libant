#include <libant/encoding/binary/endian.h>

namespace ant {

#ifndef __linux__

const uint64_t value = 1;
Endian SystemEndian = *reinterpret_cast<const uint8_t*>(&value) ? Endian::LittleEndian : Endian::BigEndian;

#endif

} // namespace ant