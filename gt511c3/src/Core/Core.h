#pragma once

#pragma warning (disable : 4251)

#define BIT(x) (1 << x)

#define BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

#define PI 3.141592653589793f
#define TAU 2.0f * PI
#define EPSILON 0.001f
