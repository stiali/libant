#ifndef LIBANT_INCLUDE_LIBANT_UTILS_LIKELY_H_
#define LIBANT_INCLUDE_LIBANT_UTILS_LIKELY_H_

#ifndef _WIN32

#define likely(cond_) __builtin_expect(!!(cond_), 1)
#define unlikely(cond_) __builtin_expect(!!(cond_), 0)

#else

#define likely(cond_) cond_
#define unlikely(cond_) cond_

#endif

#endif //LIBANT_INCLUDE_LIBANT_UTILS_LIKELY_H_
