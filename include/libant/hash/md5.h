#ifndef LIBANT_INCLUDE_LIBANT_HASH_MD_5_H_
#define LIBANT_INCLUDE_LIBANT_HASH_MD_5_H_

#include <string>

namespace ant {

/**
 * MD5File computes MD5 checksum from a given file
 * @param filepath file to be computed
 * @return MD5 checksum in lowercase hex string at success, empty string at failure
 */
std::string MD5File(const std::string& filepath);

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_HASH_MD_5_H_
