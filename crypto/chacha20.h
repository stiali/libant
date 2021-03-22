#ifndef LIBANT_CRYPTO_CHACHA20_H_
#define LIBANT_CRYPTO_CHACHA20_H_

#include <cstddef>
#include <cstdint>

namespace ant {

/**
 * ChaCha20 algorithm as described in RFC-7539
 *
 * @param out could be the same address as `in`. If not, length must be the same as `in`.
 * @param in data to be encrypted.
 * @param inLen length of the data to be encrypted.
 * @param key pointer to an array of 32 bytes.
 * @param nonce pointer to an array of 12 bytes.
 * @param counter
 */
void ChaCha20(uint8_t* out, const uint8_t* in, size_t inLen, const uint8_t key[32], const uint8_t nonce[12], uint32_t counter);

} // namespace ant

#endif //LIBANT_CRYPTO_CHACHA20_H_
