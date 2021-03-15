#include <sys/resource.h>

#include "resource.h"

namespace ant {

// 设置core文件最大允许大小
bool SetMaxCoreSize(uint64_t sizeInGB)
{
#ifndef _WIN32
    rlimit rlim;
    rlim.rlim_cur = sizeInGB * 1024 * 1024 * 1024; // max allowed core file size
    rlim.rlim_max = rlim.rlim_cur;
    return setrlimit(RLIMIT_CORE, &rlim) == 0;
#else
    return true;
#endif // !_WIN32
}

// 设置最大允许打开文件数
bool SetMaxOpenFiles(uint64_t maxFileNum)
{
#ifndef _WIN32
    rlimit rlim;
    /* max allowed open files */
    rlim.rlim_cur = maxFileNum;
    rlim.rlim_max = maxFileNum;
    return setrlimit(RLIMIT_NOFILE, &rlim) == 0;
#else
    return true;
#endif // !_WIN32
}

} // namespace ant