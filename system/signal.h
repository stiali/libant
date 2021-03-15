#ifndef LIBANT_SYSTEM_SIGNAL_H_
#define LIBANT_SYSTEM_SIGNAL_H_

namespace ant {

/**
 * Ignore the given signal
 * @param sigNum signal number
 * @return true on success, false on failure
 */
bool IgnoreSignal(int sigNum);

/**
 * Block all signals for the current thread.
 * @return true on success, false on failure
 */
bool ThreadBlockAllSignals();

} // namespace ant

#endif //LIBANT_SYSTEM_SIGNAL_H_
