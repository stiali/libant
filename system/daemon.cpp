#include "daemon.h"

namespace ant {

bool Daemonize(bool noChgDir, bool noClose)
{
#ifndef _WIN32
    // Call umask to set the file mode creation mask to 0.
    // The file mode creation mask that's inherited could be set to deny certain permissions.
    // If the daemon process is going to create files, it may want to set specific permissions.
    // For example, if it specifically creates files with group-read and group-write enabled,
    // a file mode creation mask that turns off either of these permissions would undo its efforts.
    umask(0);

    // Become a session leader to lose controlling TTY
    // Call fork and have the parent exit. This does several things.
    // First, if the daemon was started as a simple shell command,
    // having the parent terminate makes the shell think that the command is done.
    // Second, the child inherits the process group ID of the parent but gets a new process ID,
    // so we're guaranteed that the child is not a process group leader.
    // This is a prerequisite for the call to setsid that is done next.
    auto pid = fork();
    if (pid < 0) { // fork failed!
        return false;
    } else if (pid != 0) { // exit parent process
        exit(0);
    }
    // Call setsid to create a new session. The process
    //    (a) becomes a session leader of a new session,
    //    (b) becomes the process group leader of a new process group, and
    //    (c) has no controlling terminal.
    setsid();

    // Ensure future opens won't allocate controlling TTYs.
    // Under System V based systems, some people recommend calling fork again at this point
    // and having the parent terminate. The second child continues as the daemon.
    // This guarantees that the daemon is not a session leader, which prevents it from acquiring
    // a controlling terminal under the System V rules. Alternatively, to avoid acquiring
    // a controlling terminal, be sure to specify O_NOCTTY whenever opening a terminal device.
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGHUP, &sa, nullptr) < 0) {
        return false;
    }
    if ((pid = fork()) < 0) { // fork failed!
        return false;
    } else if (pid != 0) { // exit parent process
        exit(0);
    }

    // Change the current working directory to the root directory.
    // The current working directory inherited from the parent could be on a mounted file system.
    // Since daemons normally exist until the system is rebooted, if the daemon stays on a mounted file system,
    // that file system cannot be unmounted. Alternatively, some daemons might change the current working directory
    // to some specific location, where they will do all their work. For example,
    // line printer spooling daemons often change to their spool directory.
    if (!noChgDir && chdir("/") < 0) {
        return false;
    }

    if (!noClose) {
        // Redirects standard input, standard output and standard error to /dev/null.
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        auto fd = open("/dev/null", O_RDWR);
        auto fdin = dup2(fd, STDIN_FILENO);
        auto fdout = dup2(fd, STDOUT_FILENO);
        auto fderr = dup2(fd, STDERR_FILENO);
        if (fdin != STDIN_FILENO || fdout != STDOUT_FILENO || fderr != STDERR_FILENO) {
            return false;
        }
    }
#endif // !_WIN32
    return true;
}

} // namespace ant