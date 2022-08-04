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

#if defined(i386) || defined(__i386) || defined(__i386__) || defined(__386) || defined(__IA32__) || defined(_M_IX86) || defined(_X86_) || defined(__X86__) || \
    defined(__I86__) || defined(__INTEL__) || defined(__THW_INTEL__) || defined(__x86_64) || defined(__x86_64__) || defined(__amd64) || defined(__amd64__) || \
    defined(_M_X64) || defined(_M_AMD64) || defined(__itanium__) || defined(__ia64) || defined(__ia64__) || defined(_IA64) || defined(__IA64__) ||            \
    defined(_M_IA64) || defined(__aarch64__) || defined(__ARM_ARCH_ISA_A64) || defined(_M_ARM64)
#else
#define LIBANT_NO_UNALIGNED_ACCESS
#endif


#endif //LIBANT_INCLUDE_LIBANT_UTILS_OS_H_
