// File: core/basictypes.h - Type definitions for the project.

#ifndef CORE_BASICTYPES_H_
#define CORE_BASICTYPES_H_

#include <stdint.h>
#include <cassert>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <string>

typedef signed char   schar;
typedef unsigned char uchar;
typedef int8_t        int8;
typedef int16_t       int16;
typedef int32_t       int32;
typedef int64_t       int64;
typedef uint8_t       uint8;
typedef uint16_t      uint16;
typedef uint32_t      uint32;
typedef uint64_t      uint64;

typedef unsigned long address_t;
typedef uint64 timestamp_t;
typedef uint64 thread_id_t;
#define INVALID_ADDRESS static_cast<address_t>(-1)
#define INVALID_TIMESTAMP static_cast<timestamp_t>(-1)
#define INVALID_THD_ID static_cast<thread_id_t>(-1)

// Disallow the copy constructor and operator= functions
#define DISALLOW_COPY_CONSTRUCTORS(TypeName)    \
  TypeName(const TypeName&);                    \
  void operator=(const TypeName&)

#endif


