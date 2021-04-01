#ifndef LIBANT_SYSTEM_DAEMON_H_
#define LIBANT_SYSTEM_DAEMON_H_

namespace ant {

/**
 * Daemonize is for programs wishing to detach themselves from the controlling terminal and run in the background as system daemons.
 *
 * @param noChgDir set it true if you don't want to change the current working directory, or false if you want to change the cwd to /
 * @param noClose set it true if you don't want to close the standard file descriptors, or false if you want to redirect the standard input, output and error to /dev/null
 * @return true on success, false on failure.
 */
bool Daemonize(bool noChgDir, bool noClose);

} // namespace ant

#endif //LIBANT_SYSTEM_DAEMON_H_
