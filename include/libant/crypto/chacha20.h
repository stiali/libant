/*
*
* LibAnt - A handy C++ library
* Copyright (C) 2021 Antigloss Huang (https://github.com/antigloss) All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

#ifndef LIBANT_CRYPTO_CHACHA20_H_
#define LIBANT_CRYPTO_CHACHA20_H_

#include <cstddef>
#include <cstdint>
#include <string>

namespace ant {

/**
 * ChaCha20 algorithm as described in RFC-7539
 */
class ChaCha20Cipher {
public:
    /**
     * Construct a ChaCha20 stream cipher as described in RFC-7539 with the given key, nonce and counter
     *
     * @param key pointer to an array of 32 bytes.
     * @param nonce pointer to an array of 12 bytes.
     * @param counter
     */
    ChaCha20Cipher(const uint8_t key[32], const uint8_t nonce[12], uint32_t counter);

    /**
     * Encrypt src into dst. dst could be the same address as src, in this case, it's encrypted inplace.
     *
     * @param dst could be the same address as `src`. If not, length must be the same as `src`.
     * @param src data to be encrypted.
     * @param len length of the data to be encrypted.
     */
    void Encrypt(void* dst, const void* src, size_t len);

    /**
     * Decrypt src into dst. dst could be the same address as src, in this case, it's decrypted inplace.
     *
     * @param dst could be the same address as `src`. If not, length must be the same as `src`.
     * @param src data to be encrypted.
     * @param len length of the data to be encrypted.
     */
    void Decrypt(void* dst, const void* src, size_t len)
    {
        Encrypt(dst, src, len);
    }

private:
    const uint32_t counter_;
    uint32_t input_[16];
};

/**
 * ChaCha20 algorithm as described in RFC-7539
 *
 * @param out could be the same address as `in`. If not, length must be the same as `in`.
 * @param in data to be encrypted.
 * @param len length of the data to be encrypted.
 * @param key pointer to an array of 32 bytes.
 * @param nonce pointer to an array of 12 bytes.
 * @param counter
 */
inline void ChaCha20(uint8_t* out, const uint8_t* in, size_t len, const uint8_t key[32], const uint8_t nonce[12], uint32_t counter)
{
    ChaCha20Cipher cc20(key, nonce, counter);
    cc20.Encrypt(out, in, len);
}

} // namespace ant

#endif //LIBANT_CRYPTO_CHACHA20_H_
