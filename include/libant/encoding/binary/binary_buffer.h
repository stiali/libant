#ifndef LIBANT_INCLUDE_LIBANT_ENCODING_BINARY_BINARY_BUFFER_H_
#define LIBANT_INCLUDE_LIBANT_ENCODING_BINARY_BINARY_BUFFER_H_

#include <cassert>
#include <cstdint>
#include <cstring>

#include "endian.h"

namespace ant {

/**
 * BinaryBuffer can be used to encode data using the specified `endian`.
 * It supports efficient prepending.
 *
 * @tparam endian endian used to encode data into the binary stream
 */
template<Endian endian>
class BinaryBuffer {
public:
    /**
     * Creates a BinaryBuffer object to encode data into.
     *
     * @param reservedBytes    initial buffer capacity in bytes used to append data into the BinaryBuffer. Must be > 0.
     * @param prependableBytes max amount of data in bytes allowed to prepend into the BinaryBuffer. Must be <= 200.
     */
    BinaryBuffer(uint32_t reservedBytes = 128, uint8_t prependableBytes = 0)
        : prependableBytes_(prependableBytes)
    {
        assert(prependableBytes <= 200 && reservedBytes > 0);
        assert(static_cast<uint64_t>(prependableBytes) + reservedBytes <= 0xFFFFFFFF);
        auto buf = new char[prependableBytes + reservedBytes];
        head_ = buf + prependableBytes_;
        tail_ = head_;
        headCap_ = prependableBytes_;
        tailCap_ = reservedBytes;
    }

    /**
     * Destroy the BinaryBuffer and free the underlying memory
     */
    ~BinaryBuffer()
    {
        delete[](head_ - headCap_);
    }

    /**
     * Data returns a pointer to the first byte of the encoded data.
     *
     * @return pointer to the first byte of the encoded data
     */
    const void* Data() const
    {
        return head_;
    }

    /**
     * Size returns number of bytes encoded into the BinaryBuffer.
     *
     * @return size of the BinaryBuffer
     */
    uint32_t Size() const
    {
        return tail_ - head_;
    }

    /**
     * Clears the BinaryBuffer. Make sure Clear() is called before reusing a BinaryBuffer.
     */
    void Clear()
    {
        uint32_t size = Size();
        uint32_t prependedBytes = prependableBytes_ - headCap_;
        head_ += prependedBytes;
        tail_ = head_;
        headCap_ = prependableBytes_;
        tailCap_ = size - prependedBytes + tailCap_;
    }

    /**
     * Prepends an integer into the BinaryBuffer.
     *
     * @tparam T supported integer types are int8, uint8, int16, uint16, int32, uint32, int64, and uint64
     * @param val
     */
    template<typename T>
    void PrependInteger(T val)
    {
        uint32_t prependingBytes = sizeof val;
        assert(prependingBytes <= headCap_);
        head_ -= prependingBytes;
        if constexpr (endian == Endian::LittleEndian) {
            *reinterpret_cast<T*>(head_) = ant::HostToLittleEndian(val);
        } else {
            *reinterpret_cast<T*>(head_) = ant::HostToBigEndian(val);
        }
        headCap_ -= prependingBytes;
    }

    /**
     * Prepends a string into the BinaryBuffer.
     *
     * @param val
     */
    void PrependString(const std::string& val)
    {
        assert(val.size() <= headCap_);
        head_ -= val.size();
        memcpy(head_, val.c_str(), val.size());
        headCap_ -= val.size();
    }

    /**
     * Prepends string `s` of length `len` into the BinaryBuffer.
     *
     * @param s
     * @param len
     */
    void PrependString(const void* s, size_t len)
    {
        assert(len <= headCap_);
        head_ -= len;
        memcpy(head_, s, len);
        headCap_ -= len;
    }

    /**
     * Prepends length of the string and the string payload into the BinaryBuffer.
     *
     * @tparam StringLenType type used to encode length of the string
     * @param val
     */
    template<typename StringLenType>
    void PrependString(const std::string& val)
    {
        PrependString(val);
        PrependInteger(static_cast<StringLenType>(val.size()));
    }

    /**
     * Prepends `len` and `s` into the BinaryBuffer.
     *
     * @tparam StringLenType type used to encode `len`
     * @param s
     * @param len
     */
    template<typename StringLenType>
    void PrependString(const void* s, size_t len)
    {
        PrependString(s, len);
        PrependInteger(static_cast<StringLenType>(len));
    }

    /**
     * Appends an integer into the BinaryBuffer.
     *
     * @tparam T supported integer types are int8, uint8, int16, uint16, int32, uint32, int64, and uint64
     * @param val
     */
    template<typename T>
    void AppendInteger(T val)
    {
        uint32_t appendingBytes = sizeof val;
        grow(appendingBytes);
        if constexpr (endian == Endian::LittleEndian) {
            *reinterpret_cast<T*>(tail_) = ant::HostToLittleEndian(val);
        } else {
            *reinterpret_cast<T*>(tail_) = ant::HostToBigEndian(val);
        }
        tail_ += appendingBytes;
        tailCap_ -= appendingBytes;
    }

    /**
     * Appends a string into the BinaryBuffer.
     *
     * @param val
     */
    void AppendString(const std::string& val)
    {
        grow(val.size());
        memcpy(tail_, val.c_str(), val.size());
        tail_ += val.size();
        tailCap_ -= val.size();
    }

    /**
     * Appends string `s` of length `len` into the BinaryBuffer.
     *
     * @param s
     * @param len
     */
    void AppendString(const void* s, size_t len)
    {
        grow(len);
        memcpy(tail_, s, len);
        tail_ += len;
        tailCap_ -= len;
    }

    /**
     * Appends length of the string using type `StringLenType` first, then appends the string payload into the BinaryBuffer.
     *
     * @tparam StringLenType type used to encode length of the string
     * @param val
     */
    template<typename StringLenType>
    void AppendString(const std::string& val)
    {
        AppendInteger(static_cast<StringLenType>(val.size()));
        AppendString(val);
    }

    /**
     * Appends `len` using type `StringLenType` first, then appends `s` into the BinaryBuffer.
     *
     * @tparam StringLenType type used to encode `len`
     * @param s
     * @param len
     */
    template<typename StringLenType>
    void AppendString(const void* s, size_t len)
    {
        AppendInteger(static_cast<StringLenType>(len));
        AppendString(s, len);
    }

private:
    void grow(uint32_t appendingBytes)
    {
        if (tailCap_ >= appendingBytes) {
            return;
        }

        auto size = Size();
        const uint64_t curSize = static_cast<uint64_t>(headCap_) + size + tailCap_;
        const uint64_t newSize = ((curSize + appendingBytes) / 1024 + 1) * 1024;
        assert(newSize <= 0xFFFFFFFF);
        auto buf = new char[newSize];
        memcpy(buf + headCap_, head_, size);
        delete[](head_ - headCap_);
        head_ = buf + headCap_;
        tail_ = head_ + size;
        tailCap_ += (newSize - curSize);
    }

private:
    const uint8_t prependableBytes_;
    uint8_t headCap_;
    uint32_t tailCap_;
    char* head_;
    char* tail_;
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_ENCODING_BINARY_BINARY_BUFFER_H_
