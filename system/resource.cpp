#include <sys/resource.h>

#include "resource.h"

namespace ant {

bool SetMaxCoreFileSize(uint64_t sizeInGB)
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

bool SetMaxOpenFiles(uint64_t maxFileNum)
{
#ifndef _WIN32
    rlimit rlim;
    rlim.rlim_cur = maxFileNum; // max allowed open files
    rlim.rlim_max = maxFileNum;
    return setrlimit(RLIMIT_NOFILE, &rlim) == 0;
#else
    return true;
#endif // !_WIN32
}

} // namespace ant