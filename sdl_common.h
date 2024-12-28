#ifndef __SDL_COMMON_H
#define __SDL_COMMON_H
#include <stdint.h>
#include <stdbool.h>

// fragments of sdl types

typedef uint64_t Uint64;
typedef uint32_t Uint32;
typedef uint16_t Uint16;
typedef uint8_t Uint8;
typedef int64_t Sint64;
typedef int32_t Sint32;
typedef int16_t Sint16;
typedef int8_t Sint8;

typedef struct SDL_GUID {
    Uint8 data[16];
} SDL_GUID;

typedef Uint32 SDL_PropertiesID;

#endif //__SDL_COMMON_H
