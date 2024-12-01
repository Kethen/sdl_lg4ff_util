#ifndef __SDL_BINDER_H
#define __SDL_BINDER_H

#include "sdl_common.h"
#include "sdl_events.h"
#include "sdl_joystick.h"

// from SDL.h

/**
 *  \name SDL_INIT_*
 *
 *  These are the flags which may be passed to SDL_Init().  You should
 *  specify the subsystems which you will be using in your application.
 */
/* @{ */
#define SDL_INIT_TIMER          0x00000001u
#define SDL_INIT_AUDIO          0x00000010u
#define SDL_INIT_VIDEO          0x00000020u  /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
#define SDL_INIT_JOYSTICK       0x00000200u  /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
#define SDL_INIT_HAPTIC         0x00001000u
#define SDL_INIT_GAMECONTROLLER 0x00002000u  /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
#define SDL_INIT_EVENTS         0x00004000u
#define SDL_INIT_SENSOR         0x00008000u
#define SDL_INIT_NOPARACHUTE    0x00100000u  /**< compatibility; this flag is ignored. */
#define SDL_INIT_EVERYTHING ( \
                SDL_INIT_TIMER | SDL_INIT_AUDIO | SDL_INIT_VIDEO | SDL_INIT_EVENTS | \
                SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC | SDL_INIT_GAMECONTROLLER | SDL_INIT_SENSOR \
            )
/* @} */



#ifdef __cplusplus
extern "C"{
#endif
	extern int (*SDL_Init)(Uint32 flags);
	extern int (*SDL_JoystickEventState)(int state);
	extern int (*SDL_WaitEvent)(SDL_Event * event);
	extern int (*SDL_WaitEventTimeout)(SDL_Event * event, int timeout);
	extern SDL_Joystick *(*SDL_JoystickOpen)(int device_index);
	extern SDL_JoystickID (*SDL_JoystickInstanceID)(SDL_Joystick *joystick);
	extern Uint16 (*SDL_JoystickGetVendor)(SDL_Joystick *joystick);
	extern Uint16 (*SDL_JoystickGetProduct)(SDL_Joystick *joystick);
	extern const char *(*SDL_JoystickPath)(SDL_Joystick *joystick);
	extern int (*SDL_JoystickNumHats)(SDL_Joystick *joystick);
	extern int (*SDL_JoystickNumButtons)(SDL_Joystick *joystick);
	extern int (*SDL_JoystickNumAxes)(SDL_Joystick *joystick);
	extern void (*SDL_JoystickClose)(SDL_Joystick *joystick);
	extern int (*SDL_JoystickSetLED)(SDL_Joystick *joystick, Uint8 red, Uint8 green, Uint8 blue);
	extern int (*SDL_JoystickSendEffect)(SDL_Joystick *joystick, const void *data, int size);

	void init_sdl_bindings();
#ifdef __cplusplus
}
#endif

#endif //__SDL_BINDER_H
