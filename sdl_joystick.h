#ifndef __SDL_JOYSTICK_H
#define __SDL_JOYSTICK_H

#include "sdl_common.h"

// this contains fragments of SDL_joystick.h
typedef int32_t SDL_JoystickID;
typedef void SDL_Joystick;

/**
 *  \name Hat positions
 */
/* @{ */
#define SDL_HAT_CENTERED    0x00
#define SDL_HAT_UP          0x01
#define SDL_HAT_RIGHT       0x02
#define SDL_HAT_DOWN        0x04
#define SDL_HAT_LEFT        0x08
#define SDL_HAT_RIGHTUP     (SDL_HAT_RIGHT|SDL_HAT_UP)
#define SDL_HAT_RIGHTDOWN   (SDL_HAT_RIGHT|SDL_HAT_DOWN)
#define SDL_HAT_LEFTUP      (SDL_HAT_LEFT|SDL_HAT_UP)
#define SDL_HAT_LEFTDOWN    (SDL_HAT_LEFT|SDL_HAT_DOWN)
/* @} */

#define SDL_PROP_JOYSTICK_CAP_MONO_LED_BOOLEAN          "SDL.joystick.cap.mono_led"

typedef enum
{
    SDL_JOYSTICK_TYPE_UNKNOWN,
    SDL_JOYSTICK_TYPE_GAMEPAD,
    SDL_JOYSTICK_TYPE_WHEEL,
    SDL_JOYSTICK_TYPE_ARCADE_STICK,
    SDL_JOYSTICK_TYPE_FLIGHT_STICK,
    SDL_JOYSTICK_TYPE_DANCE_PAD,
    SDL_JOYSTICK_TYPE_GUITAR,
    SDL_JOYSTICK_TYPE_DRUM_KIT,
    SDL_JOYSTICK_TYPE_ARCADE_PAD,
    SDL_JOYSTICK_TYPE_THROTTLE
} SDL_JoystickType;

#ifdef __cplusplus
#include <string>
std::string joystick_get_hat_name(int hat_value);
std::string joystick_get_type_name(SDL_JoystickType type);
#endif //__cplusplus
#endif //__SDL_JOYSTICK_H
