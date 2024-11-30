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

#ifdef __cplusplus
#include <string>
std::string joystick_get_hat_name(int hat_value);
#endif //__cplusplus
#endif //__SDL_JOYSTICK_H
