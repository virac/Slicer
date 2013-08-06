#ifndef __CONFIG_H__
#define __CONFIG_H__

#if defined(_WIN32) || defined(__WIN32__)
#define SYSTEM_WINDOWS
#elif defined(linux) || defined(__linux)
#define SYSTEM_LINUX
#else
#error Unknown system.
#endif

#define SEC_TO_USEC                 1000000.
#define MSEC_TO_USEC                1000.
#define SEC_TO_MSEC                 1000.
#define PI                          3.14159265
#define PI_OVER_2                   1.57079633
#define TWO_TO_32                   4294967296
#define ONE_OVER_2_TO_32_PLUS_1     2.3283064359965952029459655278022e-10
#define ONE_OVER_2_TO_32_PLUS_2     2.3283064354544941170818665450989e-10
#define EPSILON						1e-6


#include <climits>

namespace Kernal {
#define UINT8_MAX 0xFF
#define INT8_MAX UINT8_MAX>>1
// 8 bits integer types
#if UCHAR_MAX == 0xFF
typedef signed char int8;
typedef unsigned char uint8;
#else
#error No 8 bits integer type for this platform
#endif

#define UINT16_MAX 0xFFFF
#define INT16_MAX UINT16_MAX>>1
// 16 bits integer types
#if USHRT_MAX == 0xFFFF
typedef signed short int16;
typedef unsigned short uint16;
#elif UINT_MAX == 0xFFFF
typedef signed int int16;
typedef unsigned int uint16;
#elif ULONG_MAX == 0xFFFF
typedef signed long int16;
typedef unsigned long uint16;
#elif ULLONG_MAX == 0xFFFF
typedef signed long long int16;
typedef unsigned long long uint16;
#else
#error No 16 bits integer type for this platform
#endif

#define UINT32_MAX 0xFFFFFFFF
#define INT32_MAX UINT32_MAX>>1
// 32 bits integer types
#if USHRT_MAX == 0xFFFFFFFF
typedef signed short int32;
typedef unsigned short uint32;
#elif UINT_MAX == 0xFFFFFFFF
typedef signed int int32;
typedef unsigned int uint32;
#elif ULONG_MAX == 0xFFFFFFFF
typedef signed long int32;
typedef unsigned long uint32;
#elif ULLONG_MAX == 0xFFFFFFFF
typedef signed long long int32;
typedef unsigned long long uint32;
#else
#error No 32 bits integer type for this platform
#endif

#define UINT64_MAX 0xFFFFFFFFFFFFFFFF
#define INT64_MAX UINT64_MAX>>1
// 64 bits integer types
#if USHRT_MAX == 0xFFFFFFFFFFFFFFFF
typedef signed short int64;
typedef unsigned short uint64;
#elif UINT_MAX == 0xFFFFFFFFFFFFFFFF
typedef signed int int64;
typedef unsigned int uint64;
#elif ULONG_MAX == 0xFFFFFFFFFFFFFFFF
typedef signed long int64;
typedef unsigned long uint64;
#elif ULLONG_MAX == 0xFFFFFFFFFFFFFFFF
typedef signed long long int64;
typedef unsigned long long uint64;
#else
#error No 32 bits integer type for this platform
#endif
}

#endif  // __CONFIG_H__
