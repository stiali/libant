#ifndef LIBANT_SYSTEM_RESOURCE_H_
#define LIBANT_SYSTEM_RESOURCE_H_

#include <cstdint>

namespace ant {

/**
 * Set maximum core file size.
 * @param sizeInGB
 * @return true on success, false on failure
 */
bool SetMaxCoreDumpSize(uint64_t sizeInGB);

/**
 * Set maximum number of open file descriptors.
 * @param maxFileNum
 * @return true on success, false on failure
 */
bool SetMaxOpenFiles(uint64_t maxFileNum);

} // namespace ant

#endif // LIBANT_SYSTEM_RESOURCE_H_
