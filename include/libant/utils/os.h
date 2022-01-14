#ifndef LIBANT_INCLUDE_LIBANT_UTILS_OS_H_
#define LIBANT_INCLUDE_LIBANT_UTILS_OS_H_

#if defined(__WINDOWS__) || defined(_WIN32) || defined(__WIN32__) || defined(_WIN64) || defined(__TOS_WIN__)

#define LIBANT_OS_WINDOWS

#elif defined(__ANDROID__)

#define LIBANT_OS_ANDROID

#elif defined(__APPLE__) && defined(__MACH__) && defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)

#define LIBANT_OS_IOS

#elif defined(linux) || defined(__linux) || defined(__linux__) || defined(__gnu_linux__)

#define LIBANT_OS_LINUX

#elif defined(macintosh) || defined(Macintosh) || (defined(__APPLE__) && defined(__MACH__))

#define LIBANT_OS_MACOS

#elif defined(unix) || defined(__unix) || defined(_XOPEN_SOURCE) || defined(_POSIX_SOURCE)

#define LIBANT_OS_UNIX

#endif

#endif //LIBANT_INCLUDE_LIBANT_UTILS_OS_H_
