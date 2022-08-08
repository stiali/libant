/*
*
* LibAnt - A handy C++ library
* Copyright (C) 2021 Antigloss Huang (https://github.com/antigloss) All rights reserved.
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program.  If not, see <http://www.gnu.org/licenses/>.
*
*/

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
