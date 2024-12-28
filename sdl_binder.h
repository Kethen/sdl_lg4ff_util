#ifndef __SDL_BINDER_H
#define __SDL_BINDER_H

#include "sdl_common.h"
#include "sdl_events.h"
#include "sdl_joystick.h"
#include "sdl_haptic.h"

// SDL_init.h
typedef Uint32 SDL_InitFlags;
#define SDL_INIT_AUDIO      0x00000010u /**< `SDL_INIT_AUDIO` implies `SDL_INIT_EVENTS` */
#define SDL_INIT_VIDEO      0x00000020u /**< `SDL_INIT_VIDEO` implies `SDL_INIT_EVENTS`, should be initialized on the main thread */
#define SDL_INIT_JOYSTICK   0x00000200u /**< `SDL_INIT_JOYSTICK` implies `SDL_INIT_EVENTS`, should be initialized on the same thread as SDL_INIT_VIDEO on Windows if you don't set SDL_HINT_JOYSTICK_THREAD */
#define SDL_INIT_HAPTIC     0x00001000u
#define SDL_INIT_GAMEPAD    0x00002000u /**< `SDL_INIT_GAMEPAD` implies `SDL_INIT_JOYSTICK` */
#define SDL_INIT_EVENTS     0x00004000u
#define SDL_INIT_SENSOR     0x00008000u /**< `SDL_INIT_SENSOR` implies `SDL_INIT_EVENTS` */
#define SDL_INIT_CAMERA     0x00010000u /**< `SDL_INIT_CAMERA` implies `SDL_INIT_EVENTS` */


#ifdef __cplusplus
extern "C"{
#endif

extern bool (*SDL_Init)(Uint32 flags);
extern void (*SDL_SetJoystickEventsEnabled)(bool enabled);
extern bool (*SDL_WaitEvent)(SDL_Event * event);
extern SDL_Joystick *(*SDL_OpenJoystick)(int device_index);
extern SDL_JoystickID (*SDL_GetJoystickID)(SDL_Joystick *joystick);
extern Uint16 (*SDL_GetJoystickVendor)(SDL_Joystick *joystick);
extern Uint16 (*SDL_GetJoystickProduct)(SDL_Joystick *joystick);
extern const char *(*SDL_GetJoystickPath)(SDL_Joystick *joystick);
extern int (*SDL_GetNumJoystickHats)(SDL_Joystick *joystick);
extern int (*SDL_GetNumJoystickButtons)(SDL_Joystick *joystick);
extern int (*SDL_GetNumJoystickAxes)(SDL_Joystick *joystick);
extern bool (*SDL_SendJoystickEffect)(SDL_Joystick *joystick, const void *data, int size);
extern bool (*SDL_SetJoystickLED)(SDL_Joystick *joystick, Uint8 red, Uint8 green, Uint8 blue);
extern void (*SDL_CloseJoystick)(SDL_Joystick *joystick);
extern SDL_JoystickType (*SDL_GetJoystickType)(SDL_Joystick *joystick);
extern const char *(*SDL_GetJoystickName)(SDL_Joystick *joystick);
extern SDL_PropertiesID (*SDL_GetJoystickProperties)(SDL_Joystick *joystick);

extern SDL_Haptic *(*SDL_OpenHapticFromJoystick)(SDL_Joystick *joystick);
extern void (*SDL_CloseHaptic)(SDL_Haptic * haptic);
extern int (*SDL_GetMaxHapticEffects)(SDL_Haptic * haptic);
extern int (*SDL_GetMaxHapticEffectsPlaying)(SDL_Haptic * haptic);
extern Uint32 (*SDL_GetHapticFeatures)(SDL_Haptic * haptic);
extern int (*SDL_GetNumHapticAxes)(SDL_Haptic * haptic);
extern bool (*SDL_HapticEffectSupported)(SDL_Haptic * haptic, SDL_HapticEffect * effect);
extern int (*SDL_CreateHapticEffect)(SDL_Haptic * haptic, SDL_HapticEffect * effect);
extern bool (*SDL_UpdateHapticEffect)(SDL_Haptic * haptic, int effect, SDL_HapticEffect * data);
extern bool (*SDL_RunHapticEffect)(SDL_Haptic * haptic, int effect, Uint32 iterations);
extern bool (*SDL_StopHapticEffect)(SDL_Haptic * haptic, int effect);
extern void (*SDL_DestroyHapticEffect)(SDL_Haptic * haptic, int effect);
extern bool (*SDL_GetHapticEffectStatus)(SDL_Haptic * haptic, int effect);
extern bool (*SDL_SetHapticGain)(SDL_Haptic * haptic, int gain);
extern bool (*SDL_SetHapticAutocenter)(SDL_Haptic * haptic, int autocenter);
extern bool (*SDL_StopHapticEffects)(SDL_Haptic * haptic);
extern bool (*SDL_HapticRumbleSupported)(SDL_Haptic * hapatic);
extern bool (*SDL_InitHapticRumble)(SDL_Haptic * haptic);
extern bool (*SDL_PlayHapticRumble)(SDL_Haptic * haptic, float strength, Uint32 length );
extern bool (*SDL_StopHapticRumble)(SDL_Haptic * haptic);

extern bool (*SDL_IsGamepad)(int joystick_index);

extern char *(*SDL_GetError)();

extern bool (*SDL_GetBooleanProperty)(SDL_PropertiesID props, const char *name, bool default_value);

void init_sdl_bindings();

#ifdef __cplusplus
}
#endif

#endif //__SDL_BINDER_H
