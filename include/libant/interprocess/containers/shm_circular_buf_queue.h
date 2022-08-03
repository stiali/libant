// Copyright (C) 2001-2013 Free Software Foundation, Inc.
//
// This file is part of the GNU ISO C++ Library.  This library is free
// software; you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the
// Free Software Foundation; either version 3, or (at your option)
// any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// Under Section 7 of GPL version 3, you are granted additional
// permissions described in the GCC Runtime Library Exception, version
// 3.1, as published by the Free Software Foundation.

// You should have received a copy of the GNU General Public License and
// a copy of the GCC Runtime Library Exception along with this program;
// see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see
// <http://www.gnu.org/licenses/>.

/*
 *
 * Copyright (C) 2015
 * Antigloss Huang (https://github.com/antigloss)
 *
 * Permission to use, copy, modify, distribute and sell this software
 * and its documentation for any purpose is hereby granted without fee,
 * provided that the above copyright notice appear in all copies and
 * that both that copyright notice and this permission notice appear
 * in supporting documentation.  Hewlett-Packard Company makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied warranty.
 *
 */

/**
 * @brief Shared memory circular buffer queue.
 * @platform Linux and Windows.
 * @note Tested heavily only under x86_x64 GNU/Linux.
 * @author Antigloss Huang (https://github.com/antigloss)
 */

#ifndef LIBANT_INCLUDE_LIBANT_INTERPROCESS_CONTAINERS_SHM_CIRCULAR_BUF_QUEUE_H_
#define LIBANT_INCLUDE_LIBANT_INTERPROCESS_CONTAINERS_SHM_CIRCULAR_BUF_QUEUE_H_

#include <cstdint>
#include <string>

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace ant {

/**
 * @brief A lock-free shared memory circular queue dedicated to work well
 * 			with only one producer and one consumer, no matter they are
 * 			within the same/different thread/process.
 */
class ShmCircularBufQueue {
public:
    /**
	 * @brief Default constructor. Only do object initialization here,
	 * 			shared memory is not allocated.
	 * 			To make the ShmCircularBufQueue object work properly
	 * 			as a circular queue, call Create or Attach.
	 * @see Create, Attach
	 */
    ShmCircularBufQueue()
    {
        cq_ = 0;
#ifdef WIN32
        mapfile_ = 0;
#endif
    }

    /**
	 * @brief Detaches from the circular queue. The shared memory allocated is not remove
	 * 			from the system and can be attached again. To remove the shared memory
	 * 			from the system, call Destroy().
	 * @note Under Windows, the shared memory will be remove after program termination.
	 */
    ~ShmCircularBufQueue()
    {
        Detach();
    }

    /**
	 * @brief Allocates shared memory for ShmCircularBufQueue.
	 * 			Failed if the named shared memory already exists.
	 * @param name Name of the shared memory, must be less than 64 bytes.
	 * 				Failed if the named shared memory already exists.
	 * @param cqSize Size of the circular queue, must be 100 bytes greater
	 * 					than `dataMaxSz` and less than 2,000,000,000 bytes.
	 * @param dataMaxSz Max size in bytes allowed for data pushed into the circular queue.
	 * @return true on success, false on failure.
	 * @see Destroy, Attach
	 */
    bool Create(const std::string& name, uint32_t cqSize, uint32_t dataMaxSz);

    /**
	 * @brief Destroy circular queue and remove the shared memory from the system.
	 * @return true on success, false on failure.
	 * @see Create
	 */
    bool Destroy();

    /**
	 * @brief Attaches to an already created shared memory circular queue.
	 * @param name Name of the shared memory.
	 * @return true on success, false on failure.
	 * @note You must make sure that Create() had returned successfully
	 *			before calling this function.
	 * @see Create, Detach
	 */
    bool Attach(const std::string& name);

    /**
	 * @brief Detaches from the circular queue. The shared memory allocated is not removed
	 * 			from the system and can be attached again. To remove the shared memory
	 * 			from the system, call Destroy().
	 * @return true on success, false on failure.
	 * @note Under Windows, the shared memory will be remove after program termination.
	 * @see Attach, Destroy
	 */
    bool Detach();

    /**
	 * @brief Pops a data element from the circular queue.
	 * 			If success, `*data` will point to the popped data.
	 * 			You DON'T need to free/delete `*data`.
	 * @param data If success, `*data` will point to the popped data.
	 * @return Length of the popped data on success, 0 if the queue is Empty.
	 * @see Push
	 */
    uint32_t Pop(void** data);

    /**
	 * @brief Pushed a data element into the circular queue.
	 * @param data data to be pushed into the circular queue.
	 * @param len Length of the data to be pushed.
	 * @return true on success, false on failure (the queue is full).
	 * @see Pop
	 */
    bool Push(const void* data, uint32_t len);

    /**
	 * @brief Returns true if the ShmCircularBufQueue is Empty.
	 * @return true if Empty, false otherwise.
	 */
    bool Empty() const
    {
        return (cq_->Head == cq_->Tail);
    }

private:
    /*! 64: Max size in bytes for name of the shared memory, including '\0' */
    static const uint32_t kShmNameSz = 64;
    /* shared memory circular queue max size */
    static const uint32_t kShmCircularQueueMaxSz = 2000000000;

private:
    /**
	 * @struct ShmCQ
	 * @brief Head of the circular queue
	 */
    struct ShmCQ {
        /*! Offset for Head of the circular queue */
        volatile uint32_t Head;
        /*! Offset for Tail of the circular queue */
        volatile uint32_t Tail;
        /*! Size of the allocated shared memory in bytes */
        uint32_t ShmSize;
        /*! Max size in bytes allowed for data pushed into the circular queue */
        uint32_t ElemMaxSize;
        /*! Name of the shared memory */
        char Name[kShmNameSz];
    };

#pragma pack(1)

    struct ShmBlock {
        uint32_t Len;
        uint8_t Data[];
    };

#pragma pack()

private:
    // forbid copy and assignment
    ShmCircularBufQueue(const ShmCircularBufQueue&) = delete;
    ShmCircularBufQueue& operator=(const ShmCircularBufQueue&) = delete;

    ShmBlock* head() const
    {
        return reinterpret_cast<ShmBlock*>(reinterpret_cast<char*>(cq_) + cq_->Head);
    }

    ShmBlock* tail() const
    {
        return reinterpret_cast<ShmBlock*>(reinterpret_cast<char*>(cq_) + cq_->Tail);
    }

    bool pushWait(uint32_t len) const
    {
        uint32_t tail = cq_->Tail;
        for (int cnt = 0; cnt != 10; ++cnt) {
            uint32_t head = cq_->Head;
            // q->ElemMaxSize is added just to prevent overwriting
            // the buffer that might be referred to currently
            if ((head <= tail) || (head >= (tail + len + cq_->ElemMaxSize))) {
                return true;
            }
#ifndef WIN32
            usleep(5);
#else
            Sleep(1);
#endif
        }

        return false;
    }

    bool tailAlignWait() const
    {
        uint32_t tail = cq_->Tail;
        for (int cnt = 0; cnt != 10; ++cnt) {
            uint32_t head = cq_->Head;
            if ((head > sizeof(ShmCQ)) && (head <= tail)) {
                return true;
            }
#ifndef WIN32
            usleep(5);
#else
            Sleep(1);
#endif
        }

        return false;
    }

    void alignHead()
    {
        uint32_t head = cq_->Head;
        if (head < cq_->Tail) {
            return;
        }

        ShmBlock* pad = this->head();
        if (((cq_->ShmSize - head) < sizeof(ShmBlock)) || (pad->Len == 0xFFFFFFFF)) {
            cq_->Head = sizeof(ShmCQ);
        }
    }

    bool alignTail(uint32_t len);

private:
    ShmCQ* cq_;
#ifdef WIN32
    HANDLE mapfile_;
#endif
};

} // namespace ant

#endif //LIBANT_INCLUDE_LIBANT_INTERPROCESS_CONTAINERS_SHM_CIRCULAR_BUF_QUEUE_H_
