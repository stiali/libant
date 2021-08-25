#ifndef LIBANT_ENCODING_BINARY_BINARY_WRITER_H_
#define LIBANT_ENCODING_BINARY_BINARY_WRITER_H_

#include <string>

#include "endian.h"

namespace ant {

/**
 * Encode data into a binary stream using the specified `endian`.
 *
 * @tparam endian endian used to encode data into the binary stream
 */
template<Endian endian>
class BinaryWriter {
public:
    using size_type = std::string::size_type;

public:
    /**
     * Construct a BinaryWriter object using `stream`.
     *
     * @param stream
     */
    BinaryWriter(std::string& stream)
        : buf_(stream)
    {
    }

    /**
     * Writes an integer into the binary stream.
     *
     * @tparam T supported integer types are int8, uint8, int16, uint16, int32, uint32, int64, and uint64
     * @param val
     */
    template<typename T>
    void WriteInteger(T val)
    {
        auto pos = buf_.size();
        buf_.resize(pos + sizeof val);
        if constexpr (endian == Endian::LittleEndian) {
            *reinterpret_cast<T*>(&buf_[pos]) = ant::HostToLittleEndian(val);
        } else {
            *reinterpret_cast<T*>(&buf_[pos]) = ant::HostToBigEndian(val);
        }
    }

    /**
     * Writes a string into the binary stream.
     *
     * @param val
     */
    void WriteString(const std::string& val)
    {
        buf_.append(val);
    }

    /**
     * Writes string `s` of length `len` into the binary stream.
     *
     * @param s
     * @param len
     */
    void WriteString(const char* s, size_t len)
    {
        buf_.append(s, len);
    }

    /**
     * Writes length of the string of type `StringLenType` first, then writes the string payload into the binary stream.
     *
     * @tparam StringLenType type of the length of the string encoded before the string payload
     * @param val
     */
    template<typename StringLenType>
    void WriteString(const std::string& val)
    {
        WriteInteger(static_cast<StringLenType>(val.size()));
        WriteString(val);
    }

    /**
     * Appends n bytes of random data to the underlying string.
     */
    void Append(size_t n)
    {
        buf_.resize(buf_.size() + n);
    }

    /**
     * Return current size in bytes of the underlying string.
     */
    size_type Size() const
    {
        return buf_.size();
    }

private:
    std::string& buf_;
};

} // namespace ant

#endif //LIBANT_ENCODING_BINARY_BINARY_WRITER_H_
