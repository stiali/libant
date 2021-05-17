#ifndef LIBANT_SYSTEM_SIGNAL_H_
#define LIBANT_SYSTEM_SIGNAL_H_

#include <csignal>

namespace ant {

/**
 * Ignore the given signal
 * @param sigNum signal number
 * @return true on success, false on failure
 */
inline bool IgnoreSignal(int sigNum)
{
    // The only portable use of signal() is to set a signal's disposition to SIG_DFL or SIG_IGN
    return signal(sigNum, SIG_IGN) != SIG_ERR;
}

/**
 * Block all signals for the current thread.
 * @return true on success, false on failure
 */
bool ThreadBlockAllSignals();

} // namespace ant

#endif //LIBANT_SYSTEM_SIGNAL_H_
