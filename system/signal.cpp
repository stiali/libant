#include "signal.h"

namespace ant {

bool ThreadBlockAllSignals()
{
#ifndef _WIN32
    sigset_t mask;
    return (sigfillset(&mask) == 0) && (pthread_sigmask(SIG_BLOCK, &mask, nullptr) == 0);
#else
    return true;
#endif // !_WIN32
}

} // namespace ant