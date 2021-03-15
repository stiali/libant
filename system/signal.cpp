#include <csignal>

#include "signal.h"

namespace ant {

bool IgnoreSignal(int sigNum)
{
    // The only portable use of signal() is to set a signal's disposition to SIG_DFL or SIG_IGN
    return signal(sigNum, SIG_IGN) != SIG_ERR;
}

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