#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _WIN32
#define __LITTLE_ENDIAN 1
#define __BIG_ENDIAN 1

#define __BYTE_ORDER __LITTLE_ENDIAN
#endif

#ifdef __cplusplus
}
#endif