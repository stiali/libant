#include <cassert>
#include <cerrno>
#include <cstring>

#ifndef WIN32

#include <fcntl.h> /* For O_* constants */
#include <sys/mman.h>
#include <sys/stat.h>

#endif

#include <libant/interprocess/containers/shm_circular_buf_queue.h>

using namespace std;

namespace ant {

bool ShmCircularBufQueue::Create(const string& name, uint32_t cqSize, uint32_t dataMaxSz)
{
    assert((cqSize < kShmCircularQueueMaxSz) && (cqSize >= (dataMaxSz + sizeof(ShmBlock))));

    if (name.size() >= kShmNameSz) {
        errno = ENAMETOOLONG;
        return false;
    }

    uint32_t size = cqSize + sizeof(ShmCQ);

#ifndef WIN32
    int shmfd = shm_open(name.c_str(), O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
    if (shmfd == -1) {
        return false;
    }

    int ret = ftruncate(shmfd, size);
    if (ret == -1) {
        close(shmfd);
        shm_unlink(name.c_str());
        return false;
    }

    ShmCQ* cq = reinterpret_cast<ShmCQ*>(mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0));
    close(shmfd);
    if (cq == MAP_FAILED) {
        shm_unlink(name.c_str());
        return false;
    }
#else
    mapfile_ = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, size, name.c_str());
    if (!mapfile_) {
        return false;
    }

    ShmCQ* cq = reinterpret_cast<ShmCQ*>(MapViewOfFile(mapfile_, FILE_MAP_ALL_ACCESS, 0, 0, 0));
    if (!cq) {
        CloseHandle(mapfile_);
        mapfile_ = 0;
        return false;
    }
#endif

    cq_ = cq;
    // init shared-memory circular queue
    cq_->Head = sizeof(ShmCQ);
    cq_->Tail = sizeof(ShmCQ);
    cq_->ShmSize = size;
    cq_->ElemMaxSize = dataMaxSz + sizeof(ShmBlock);
    strcpy(cq_->Name, name.c_str());

    return true;
}

bool ShmCircularBufQueue::Destroy()
{
#ifndef WIN32
    return ((shm_unlink(cq_->Name) == 0) && Detach());
#else
    // no easy way to remove shared memory under windows system
    return (CloseHandle(mapfile_) && detach());
#endif
}

bool ShmCircularBufQueue::Attach(const string& name)
{
#ifndef WIN32
    int shmfd = shm_open(name.c_str(), O_RDWR, 0);
    if (shmfd == -1) {
        return false;
    }

    // this call of mmap is used to get cq->ShmSize only
    ShmCQ* cq = reinterpret_cast<ShmCQ*>(mmap(0, sizeof(ShmCQ), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0));
    if (cq == MAP_FAILED) {
        close(shmfd);
        return false;
    }

    // used to mmap shmfd again
    uint32_t sz = cq->ShmSize;
    // unmap cq
    munmap(cq, sizeof(ShmCQ));
    // mmap again with the real length of ShmCQ
    cq = reinterpret_cast<ShmCQ*>(mmap(0, sz, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0));
    close(shmfd);
    if (cq == MAP_FAILED) {
        return false;
    }

    cq_ = cq;
    return true;
#else
    HANDLE hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, name.c_str());
    if (!hMapFile) {
        return false;
    }

    ShmCQ* cq = reinterpret_cast<ShmCQ*>(MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, 0));
    CloseHandle(hMapFile);
    if (cq) {
        cq_ = cq;
        return true;
    }

    return false;
#endif
}

bool ShmCircularBufQueue::Detach()
{
    if (cq_) {
#ifndef WIN32
        return (munmap(cq_, cq_->ShmSize) == 0);
#else
        return UnmapViewOfFile(cq_);
#endif
    }
    return true;
}

uint32_t ShmCircularBufQueue::Pop(void** data)
{
    // queue is empty
    if (Empty()) {
        return 0;
    }
    alignHead();
    // queue is empty
    if (Empty()) {
        return 0;
    }

    ShmBlock* curMB = head();
    assert(curMB->Len <= cq_->ElemMaxSize);
    *data = curMB->Data;
    cq_->Head += curMB->Len;
    return curMB->Len - sizeof(ShmBlock);
}

bool ShmCircularBufQueue::Push(const void* data, uint32_t len)
{
    assert((len > 0) && (len <= cq_->ElemMaxSize - sizeof(ShmBlock)));

    uint32_t elemLen = len + sizeof(ShmBlock);
    if (alignTail(elemLen)) {
        ShmBlock* nextMB = tail();
        nextMB->Len = elemLen;
        memcpy(nextMB->Data, data, len);
        cq_->Tail += elemLen;
        return true;
    }

    return false;
}

bool ShmCircularBufQueue::alignTail(uint32_t len)
{
    uint32_t tailPos = cq_->Tail;
    uint32_t surplus = cq_->ShmSize - tailPos;
    if (surplus >= len) {
        return pushWait(len);
    }

    if (tailAlignWait()) {
        if (surplus >= sizeof(ShmBlock)) {
            ShmBlock* pad = tail();
            pad->Len = 0xFFFFFFFF;
        }
        cq_->Tail = sizeof(ShmCQ);
        return pushWait(len);
    }

    return false;
}

} // namespace ant