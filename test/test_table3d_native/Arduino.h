#pragma once

#include <stdint.h>
#include <string.h>

typedef uint8_t byte;

#ifndef HIGH
#define HIGH 0x1
#endif

#ifndef LOW
#define LOW  0x0
#endif

#ifndef PSTR
#define PSTR(x) (x)
#endif

#ifndef strcpy_P
#define strcpy_P(dest, src) strcpy((dest), (src))
#endif
