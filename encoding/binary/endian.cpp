#include "endian.h"

namespace ant {

const uint64_t value = 1;
Endian SystemEndian = *reinterpret_cast<const uint8_t*>(&value) ? Endian::LittleEndian : Endian::BigEndian;

} // namespace ant