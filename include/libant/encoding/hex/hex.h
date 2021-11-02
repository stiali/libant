#ifndef LIBANT_INCLUDE_LIBANT_ENCODING_HEX_HEX_H_
#define LIBANT_INCLUDE_LIBANT_ENCODING_HEX_HEX_H_

#include <cassert>
#include <cctype>
#include <cstdint>
#include <string>

namespace ant {

static const char __hexMap__[] = "0123456789ABCDEF";
static const char __hexMapLower__[] = "0123456789abcdef";

/**
 * @brief Converts the hex number combined of `first` and `second` to an integer of 8 bits.
 *        If first=='A', second=='B', then the result is 171.
 */
inline uint8_t HexCharsToByte(char first, char second)
{
    first = toupper(first);
    second = toupper(second);

    uint8_t digit = (first >= 'A' ? (first - 'A') + 10 : (first - '0')) * 16;
    digit += (second >= 'A' ? (second - 'A') + 10 : (second - '0'));

    return digit;
}

/**
 * @brief Converts an integer of 8 bits to a hex string in upper case.
 *
 * @param val integer value to be converted to a hex string
 * @param dst memory to hold the converted hex string. Must be equal or greater than 2 bytes.
 */
inline void ByteToHexString(uint8_t val, void* dst)
{
    char* dest = reinterpret_cast<char*>(dst);

    dest[0] = __hexMap__[val / 16];
    dest[1] = __hexMap__[val % 16];
}

/**
 * @brief Converts an integer of 8 bits to a hex string in upper case.
 *
 * @param val integer value to be converted to a hex string
 *
 * @return the converted hex string in upper case.
 */
inline std::string ByteToHexString(uint8_t val)
{
    std::string ret;
    ret.resize(2);
    ByteToHexString(val, ret.data());
    return ret;
}

/**
 * @brief Converts an integer of 8 bits to a hex string in lower case.
 *
 * @param val integer value to be converted to a hex string
 * @param dst memory to hold the converted hex string. Must be equal or greater than 2 bytes.
 */
inline void ByteToLowerHexString(uint8_t val, void* dst)
{
    char* dest = reinterpret_cast<char*>(dst);

    dest[0] = __hexMapLower__[val / 16];
    dest[1] = __hexMapLower__[val % 16];
}

/**
 * @brief Converts an integer of 8 bits to a hex string in lower case.
 *
 * @param val integer value to be converted to a hex string
 *
 * @return the converted hex string in upper case.
 */
inline std::string ByteToLowerHexString(uint8_t val)
{
    std::string ret;
    ret.resize(2);
    ByteToLowerHexString(val, ret.data());
    return ret;
}

/**
 * @brief Converts the given binary data to a hex string in upper case.
 *
 * @param src binary data to be converted
 * @param srcLen length of the binary data
 * @param dst memory to hold the converted hex string. Must be equal or greater than (srcLen * 2 + 1) bytes.
 */
inline void ToHexString(const void* src, size_t srcLen, void* dst)
{
    const uint8_t* source = reinterpret_cast<const uint8_t*>(src);
    char* dest = reinterpret_cast<char*>(dst);

    for (size_t i = 0; i != srcLen; ++i) {
        ByteToHexString(source[i], &dest[i * 2]);
    }
    dest[srcLen * 2] = '\0';
}

/**
 * @brief Converts the given binary data to a hex string in upper case.
 *
 * @param src binary data to be converted
 * @param srcLen length of the binary data
 *
 * @return the converted hex string
 */
inline std::string ToHexString(const void* src, size_t srcLen)
{
    std::string dest;
    dest.resize(srcLen * 2);
    const uint8_t* source = reinterpret_cast<const uint8_t*>(src);

    for (size_t i = 0; i != srcLen; ++i) {
        ByteToHexString(source[i], &dest[i * 2]);
    }

    return dest;
}

/**
 * @brief Converts the given binary data to a hex string in lower case.
 *
 * @param src binary data to be converted
 * @param srcLen length of the binary data
 * @param dst memory to hold the converted hex string. Must be equal or greater than (srcLen * 2 + 1) bytes.
 */
inline void ToLowerHexString(const void* src, size_t srcLen, void* dst)
{
    const uint8_t* source = reinterpret_cast<const uint8_t*>(src);
    char* dest = reinterpret_cast<char*>(dst);

    for (size_t i = 0; i != srcLen; ++i) {
        ByteToLowerHexString(source[i], &dest[i * 2]);
    }
    dest[srcLen * 2] = '\0';
}

/**
 * @brief Converts the given binary data to a hex string in lower case.
 *
 * @param src binary data to be converted
 * @param srcLen length of the binary data
 *
 * @return the converted hex string
 */
inline std::string ToLowerHexString(const void* src, size_t srcLen)
{
    std::string dest;
    dest.resize(srcLen * 2);
    const uint8_t* source = reinterpret_cast<const uint8_t*>(src);

    for (size_t i = 0; i != srcLen; ++i) {
        ByteToLowerHexString(source[i], &dest[i * 2]);
    }

    return dest;
}

/**
 * @brief Converts a hex string to a binary string.
 *
 * @param src hex string to be converted.
 * @param srcLen length of the hex string. Length of the hex string must be multiple of 2.
 * @param dst memory to hold the converted binary string. Must be equal to greater than (srcLen / 2) bytes.
 *
 */
inline void HexStringToBinary(const void* src, size_t srcLen, void* dst)
{
    assert(srcLen % 2 == 0);

    const char* source = reinterpret_cast<const char*>(src);
    uint8_t* dest = reinterpret_cast<uint8_t*>(dst);

    size_t i;
    for (i = 0; i != srcLen; i += 2) {
        dest[i / 2] = HexCharsToByte(source[i], source[i + 1]);
    }
}

/**
 * @brief Converts a hex string to a binary string.
 *
 * @param src hex string to be converted.
 * @param srcLen length of the hex string. Length of the hex string must be multiple of 2.
 * @param dst memory to hold the converted binary string. Must be equal to greater than (srcLen / 2) bytes.
 *
 * @return the converted binary string
 */
inline std::string HexStringToBinary(const void* src, size_t srcLen)
{
    std::string dst;
    dst.resize(srcLen / 2);
    HexStringToBinary(src, srcLen, dst.data());
    return dst;
}

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_ENCODING_HEX_HEX_H_
