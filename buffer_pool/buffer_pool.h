#ifndef LIBANT_BUFFER_POOL_H
#define LIBANT_BUFFER_POOL_H

#include <memory>
#include <vector>

namespace ant {

template<typename BufferType, typename BufferSizeType, BufferSizeType (BufferType::*BufferCapacityFunc)() const noexcept,
         void (BufferType::*BufferClearFunc)() noexcept>
class BufferPool : public std::enable_shared_from_this<BufferPool<BufferType, BufferSizeType, BufferCapacityFunc, BufferClearFunc>> {
public:
    using PoolPtr = std::shared_ptr<BufferPool>;
    using BufferPtr = std::shared_ptr<BufferType>;

public:
    static PoolPtr CreateBufferPool(uint64_t maxPooledByteSize)
    {
        return PoolPtr(new BufferPool(maxPooledByteSize));
    }

    ~BufferPool()
    {
        for (auto buf : pooledBuffers_) {
            delete buf;
        }
    }

    BufferPtr GetBuffer()
    {
        BufferType* buf;
        if (pooledBuffers_.size() > 0) {
            buf = pooledBuffers_.back();
            pooledBuffers_.pop_back();
            curPooledSize_ -= (buf->*BufferCapacityFunc)();
            (buf->*BufferClearFunc)();
        } else {
            buf = new BufferType;
        }
        return BufferPtr(buf, [weak = BufferPool::weak_from_this()](auto buf) {
            if (auto self = weak.lock(); self != nullptr) {
                self->freeBuffer(buf);
            } else {
                delete buf;
            }
        });
    }

private:
    BufferPool(uint64_t maxPooledSize)
        : maxPooledSize_(maxPooledSize)
        , curPooledSize_(0)
    {
    }

    void freeBuffer(BufferType* buf)
    {
        auto newSize = curPooledSize_ + (buf->*BufferCapacityFunc)();
        if (newSize <= maxPooledSize_) {
            pooledBuffers_.emplace_back(buf);
            curPooledSize_ = newSize;
        } else {
            delete buf;
        }
    }

private:
    const BufferSizeType maxPooledSize_; // Max pooled size in bytes
    BufferSizeType curPooledSize_;       // Currently pooled size in bytes
    std::vector<BufferType*> pooledBuffers_;
};

} // namespace ant

#endif //LIBANT_BUFFER_POOL_H
