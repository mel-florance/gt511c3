#pragma once

#pragma warning (disable : 4251)

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define PI 3.141592653589793f
#define TAU 2.0f * PI
#define EPSILON 0.001f

typedef __int16 int16;
typedef unsigned __int16 uint16;

typedef __int32 int32;
typedef unsigned __int32 uint32;

typedef __int64 int64;
typedef unsigned __int64 uint64;
