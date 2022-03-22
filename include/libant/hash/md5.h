#ifndef LIBANT_INCLUDE_LIBANT_HASH_MD_5_H_
#define LIBANT_INCLUDE_LIBANT_HASH_MD_5_H_

#include <string>

namespace ant {

/**
 * MD5File computes MD5 checksum from a given file
 * @param filepath file to be computed
 * @param md5 computed md5 checksum
 * @return true on success, false on failure
 */
bool MD5File(const std::string& filepath, std::string& md5);

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_HASH_MD_5_H_
