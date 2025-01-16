#ifndef __MINGW32__
#include <dlfcn.h>
#else
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>

#include "sdl_common.h"
#include "sdl_events.h"
#include "sdl_joystick.h"
#include "sdl_haptic.h"

#include "logging.h"
bool (*SDL_Init)(Uint32 flags);
void (*SDL_SetJoystickEventsEnabled)(bool enabled);
bool (*SDL_WaitEvent)(SDL_Event * event);
SDL_Joystick *(*SDL_OpenJoystick)(int device_index);
SDL_JoystickID (*SDL_GetJoystickID)(SDL_Joystick *joystick);
Uint16 (*SDL_GetJoystickVendor)(SDL_Joystick *joystick);
Uint16 (*SDL_GetJoystickProduct)(SDL_Joystick *joystick);
const char *(*SDL_GetJoystickPath)(SDL_Joystick *joystick);
int (*SDL_GetNumJoystickHats)(SDL_Joystick *joystick);
int (*SDL_GetNumJoystickButtons)(SDL_Joystick *joystick);
int (*SDL_GetNumJoystickAxes)(SDL_Joystick *joystick);
bool (*SDL_SendJoystickEffect)(SDL_Joystick *joystick, const void *data, int size);
bool (*SDL_SetJoystickLED)(SDL_Joystick *joystick, Uint8 red, Uint8 green, Uint8 blue);
void (*SDL_CloseJoystick)(SDL_Joystick *joystick);
SDL_JoystickType (*SDL_GetJoystickType)(SDL_Joystick *joystick);
const char *(*SDL_GetJoystickName)(SDL_Joystick *joystick);
SDL_PropertiesID (*SDL_GetJoystickProperties)(SDL_Joystick *joystick);

SDL_Haptic *(*SDL_OpenHapticFromJoystick)(SDL_Joystick *joystick);
void (*SDL_CloseHaptic)(SDL_Haptic * haptic);
int (*SDL_GetMaxHapticEffects)(SDL_Haptic * haptic);
int (*SDL_GetMaxHapticEffectsPlaying)(SDL_Haptic * haptic);
Uint32 (*SDL_GetHapticFeatures)(SDL_Haptic * haptic);
int (*SDL_GetNumHapticAxes)(SDL_Haptic * haptic);
bool (*SDL_HapticEffectSupported)(SDL_Haptic * haptic, SDL_HapticEffect * effect);
int (*SDL_CreateHapticEffect)(SDL_Haptic * haptic, SDL_HapticEffect * effect);
bool (*SDL_UpdateHapticEffect)(SDL_Haptic * haptic, int effect, SDL_HapticEffect * data);
bool (*SDL_RunHapticEffect)(SDL_Haptic * haptic, int effect, Uint32 iterations);
bool (*SDL_StopHapticEffect)(SDL_Haptic * haptic, int effect);
void (*SDL_DestroyHapticEffect)(SDL_Haptic * haptic, int effect);
bool (*SDL_GetHapticEffectStatus)(SDL_Haptic * haptic, int effect);
bool (*SDL_SetHapticGain)(SDL_Haptic * haptic, int gain);
bool (*SDL_SetHapticAutocenter)(SDL_Haptic * haptic, int autocenter);
bool (*SDL_StopHapticEffects)(SDL_Haptic * haptic);
bool (*SDL_HapticRumbleSupported)(SDL_Haptic * hapatic);
bool (*SDL_InitHapticRumble)(SDL_Haptic * haptic);
bool (*SDL_PlayHapticRumble)(SDL_Haptic * haptic, float strength, Uint32 length );
bool (*SDL_StopHapticRumble)(SDL_Haptic * haptic);

bool (*SDL_IsGamepad)(int joystick_index);

const char *(*SDL_GetError)();

bool (*SDL_GetBooleanProperty)(SDL_PropertiesID props, const char *name, bool default_value);

const char *lib_paths[] = {
	"./libSDL3.so.0",
	"./libSDL3.0.dylib",
	"./SDL3.dll"
};

#ifdef __MINGW32__
#define RTLD_NOW 0

void *dlopen(const char *path, uint32_t unused){
	return LoadLibraryA(path);
}

void *dlsym(void *handle, const char *name){
	return GetProcAddress(handle, name);
}

#endif

void init_sdl_bindings(){
	void *library_handle = NULL;
	for(int i = 0;i < sizeof(lib_paths) / sizeof(char *);i++){
		library_handle = dlopen(lib_paths[i], RTLD_NOW);
		if(library_handle != NULL){
			LOG("opened SDL library at %s\n", lib_paths[i]);
			break;
		}else{
			LOG_ERR("cannot open SDL library at %s\n", lib_paths[i]);
		}
	}
	if(library_handle == NULL){
		LOG_ERR("cannot open SDL library, terminating\n");
		exit(1);
	}

	#define STR(s) #s
	#define LOAD_FUNCTION(_name){ \
		_name = dlsym(library_handle, STR(_name)); \
		if(_name == NULL){ \
			LOG_ERR("cannot open fetch function %s, terminating\n", STR(_name)); \
			exit(1); \
		} \
		LOG("fetched function %s\n", STR(_name)); \
	}
	LOAD_FUNCTION(SDL_Init);
	LOAD_FUNCTION(SDL_SetJoystickEventsEnabled);
	LOAD_FUNCTION(SDL_WaitEvent);
	LOAD_FUNCTION(SDL_OpenJoystick);
	LOAD_FUNCTION(SDL_GetJoystickID);
	LOAD_FUNCTION(SDL_GetJoystickVendor);
	LOAD_FUNCTION(SDL_GetJoystickProduct);
	LOAD_FUNCTION(SDL_GetJoystickPath);
	LOAD_FUNCTION(SDL_GetNumJoystickHats);
	LOAD_FUNCTION(SDL_GetNumJoystickButtons);
	LOAD_FUNCTION(SDL_GetNumJoystickAxes);
	LOAD_FUNCTION(SDL_SendJoystickEffect);
	LOAD_FUNCTION(SDL_SetJoystickLED);
	LOAD_FUNCTION(SDL_CloseJoystick);
	LOAD_FUNCTION(SDL_GetJoystickType);
	LOAD_FUNCTION(SDL_GetJoystickName);
	LOAD_FUNCTION(SDL_GetJoystickProperties);

	LOAD_FUNCTION(SDL_OpenHapticFromJoystick);
	LOAD_FUNCTION(SDL_CloseHaptic);
	LOAD_FUNCTION(SDL_GetMaxHapticEffects);
	LOAD_FUNCTION(SDL_GetMaxHapticEffectsPlaying);
	LOAD_FUNCTION(SDL_SetHapticGain);
	LOAD_FUNCTION(SDL_SetHapticAutocenter);
	LOAD_FUNCTION(SDL_GetHapticFeatures);
	LOAD_FUNCTION(SDL_GetNumHapticAxes);
	LOAD_FUNCTION(SDL_HapticEffectSupported);
	LOAD_FUNCTION(SDL_CreateHapticEffect);
	LOAD_FUNCTION(SDL_UpdateHapticEffect);
	LOAD_FUNCTION(SDL_RunHapticEffect);
	LOAD_FUNCTION(SDL_StopHapticEffect);
	LOAD_FUNCTION(SDL_DestroyHapticEffect);
	LOAD_FUNCTION(SDL_GetHapticEffectStatus);
	LOAD_FUNCTION(SDL_StopHapticEffects);
	LOAD_FUNCTION(SDL_HapticRumbleSupported);
	LOAD_FUNCTION(SDL_InitHapticRumble);
	LOAD_FUNCTION(SDL_PlayHapticRumble);
	LOAD_FUNCTION(SDL_StopHapticRumble);

	LOAD_FUNCTION(SDL_IsGamepad);

	LOAD_FUNCTION(SDL_GetError);

	LOAD_FUNCTION(SDL_GetBooleanProperty);
	#undef STR
	#undef LOAD_FUNCTION
}
