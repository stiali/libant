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

#ifndef LIBANT_CRYPTO_XXTEA_H_
#define LIBANT_CRYPTO_XXTEA_H_

#include <cstring>
#include <string>
#include <cstdint>

namespace ant {

/**
 * Corrected Block TEA
 */
class XXTeaCipher {
public:
    /**
     * Construct an XXTeaCipher object with the given key.
     *
     * @param key
     */
    XXTeaCipher(uint32_t (&key)[4])
    {
        memcpy(key_, key, kKeySize);
    }

    /**
     * Construct an XXTeaCipher object with the given key.
     *
     * @param key must not exceed 16 bytes
     */
    XXTeaCipher(const std::string& key)
    {
        if (key.size() < kKeySize) {
            memcpy(key_, key.data(), key.size());
            memset(&reinterpret_cast<char*>(key_)[key.size()], 0, kKeySize - key.size());
        } else {
            memcpy(key_, key.data(), kKeySize);
        }
    }

    /**
     * Encrypt the given data and return the result as a std::string.
     *
     * @param data
     * @param dataLen
     * @return the encrypted result
     */
    std::string Encrypt(const void* data, uint32_t dataLen)
    {
        std::string inOut;
        inOut.reserve(dataLen + 8);
        inOut.append(reinterpret_cast<const char*>(data), dataLen);
        FastEncrypt(inOut);
        return inOut;
    }

    /**
     * Encrypt the given data and return the result as a std::string.
     *
     * @param data
     * @return the encrypted result
     */
    std::string Encrypt(const std::string& data)
    {
        std::string inOut;
        inOut.reserve(data.size() + 8);
        inOut.append(data);
        FastEncrypt(inOut);
        return inOut;
    }

    /**
     * Encrypt the given data inplace.
     *
     * @param inOut data to be encrypted and place to hold the encrypted result
     */
    void FastEncrypt(std::string& inOut)
    {
        uint32_t inLen = static_cast<uint32_t>(inOut.size());
        uint32_t remainder = inLen % 4;
        uint32_t lenIdx;
        if (remainder != 0) {
            lenIdx = (inLen + 4 - remainder) / 4;
            inOut.resize(inOut.size() + 8 - remainder);
        } else {
            lenIdx = inLen / 4;
            inOut.resize(inOut.size() + 4);
        }

        uint32_t* data = reinterpret_cast<uint32_t*>(&inOut[0]);
        data[lenIdx] = inLen;
        encrypt(data, lenIdx + 1);
    }

    /**
     * Decrypt the given data.
     * @param data to be decrypted
     * @param dataLen
     * @param out hold the decrypted result
     * @return true on success, false on failure
     */
    bool Decrypt(const void* data, uint32_t dataLen, std::string& out)
    {
        std::string inOut(reinterpret_cast<const char*>(data), dataLen);
        if (FastDecrypt(inOut)) {
            out = std::move(inOut);
            return true;
        }
        return false;
    }

    /**
     * Decrypt the given data.
     * @param data to be decrypted
     * @param out hold the decrypted result
     * @return true on success, false on failure
     */
    bool Decrypt(const std::string& data, std::string& out)
    {
        std::string inOut(data);
        if (FastDecrypt(inOut)) {
            out = std::move(inOut);
            return true;
        }
        return false;
    }

    /**
     * Decrypt the given data inplace.
     * @param inOut data to be decrypted and place to hold the decrypted result
     * @return true on success, false on failure
     */
    bool FastDecrypt(std::string& inOut)
    {
        uint32_t* data = reinterpret_cast<uint32_t*>(&inOut[0]);
        uint32_t dataLen = static_cast<uint32_t>(inOut.size());
        uint32_t dataNum = dataLen / 4;
        decrypt(data, dataNum);

        uint32_t decryptedLen = data[dataNum - 1];
        if (decryptedLen <= dataLen - 4 && decryptedLen >= dataLen - 7) {
            inOut.resize(decryptedLen);
            return true;
        }
        return false;
    }

private:
    void encrypt(uint32_t* data, uint32_t len);
    void decrypt(uint32_t* data, uint32_t len);

private:
    constexpr static size_t kKeySize = 16;

private:
    uint32_t key_[4];
};

} // namespace ant

#endif // LIBANT_CRYPTO_XXTEA_H_
