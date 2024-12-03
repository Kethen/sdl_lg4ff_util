#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "sdl_common.h"
#include "sdl_events.h"
#include "sdl_joystick.h"
#include "sdl_haptic.h"

#include "logging.h"
int (*SDL_Init)(Uint32 flags);
int (*SDL_JoystickEventState)(int state);
int (*SDL_WaitEvent)(SDL_Event * event);
int (*SDL_WaitEventTimeout)(SDL_Event * event, int timeout);
SDL_Joystick *(*SDL_JoystickOpen)(int device_index);
SDL_JoystickID (*SDL_JoystickInstanceID)(SDL_Joystick *joystick);
Uint16 (*SDL_JoystickGetVendor)(SDL_Joystick *joystick);
Uint16 (*SDL_JoystickGetProduct)(SDL_Joystick *joystick);
const char *(*SDL_JoystickPath)(SDL_Joystick *joystick);
int (*SDL_JoystickNumHats)(SDL_Joystick *joystick);
int (*SDL_JoystickNumButtons)(SDL_Joystick *joystick);
int (*SDL_JoystickNumAxes)(SDL_Joystick *joystick);
int (*SDL_JoystickSendEffect)(SDL_Joystick *joystick, const void *data, int size);
int (*SDL_JoystickSetLED)(SDL_Joystick *joystick, Uint8 red, Uint8 green, Uint8 blue);
void (*SDL_JoystickClose)(SDL_Joystick *joystick);
SDL_JoystickType (*SDL_JoystickGetType)(SDL_Joystick *joystick);

SDL_Haptic *(*SDL_HapticOpenFromJoystick)(SDL_Joystick *joystick);
void (*SDL_HapticClose)(SDL_Haptic * haptic);
int (*SDL_HapticNumEffects)(SDL_Haptic * haptic);
int (*SDL_HapticNumEffectsPlaying)(SDL_Haptic * haptic);
int (*SDL_HapticQuery)(SDL_Haptic * haptic);
int (*SDL_HapticNumAxes)(SDL_Haptic * haptic);
int (*SDL_HapticEffectSupported)(SDL_Haptic * haptic, SDL_HapticEffect * effect);
int (*SDL_HapticNewEffect)(SDL_Haptic * haptic, SDL_HapticEffect * effect);
int (*SDL_HapticUpdateEffect)(SDL_Haptic * haptic, int effect, SDL_HapticEffect * data);
int (*SDL_HapticRunEffect)(SDL_Haptic * haptic, int effect, Uint32 iterations);
int (*SDL_HapticStopEffect)(SDL_Haptic * haptic, int effect);
void (*SDL_HapticDestroyEffect)(SDL_Haptic * haptic, int effect);
int (*SDL_HapticGetEffectStatus)(SDL_Haptic * haptic, int effect);
int (*SDL_HapticSetGain)(SDL_Haptic * haptic, int gain);
int (*SDL_HapticSetAutocenter)(SDL_Haptic * haptic, int autocenter);
int (*SDL_HapticPause)(SDL_Haptic * haptic);
int (*SDL_HapticUnpause)(SDL_Haptic * haptic);
int (*SDL_HapticStopAll)(SDL_Haptic * haptic);
int (*SDL_HapticRumbleSupported)(SDL_Haptic * hapatic);
int (*SDL_HapticRumbleInit)(SDL_Haptic * haptic);
int (*SDL_HapticRumblePlay)(SDL_Haptic * haptic, float strength, Uint32 length );
int (*SDL_HapticRumbleStop)(SDL_Haptic * haptic);

const char *(*SDL_GetError)();

const char *lib_paths[] = {
	"./libSDL2-2.0.so.0",
	"./libSDL2-2.0.0.dylib"
};

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
	LOAD_FUNCTION(SDL_JoystickEventState);
	LOAD_FUNCTION(SDL_WaitEvent);
	LOAD_FUNCTION(SDL_WaitEventTimeout);
	LOAD_FUNCTION(SDL_JoystickOpen);
	LOAD_FUNCTION(SDL_JoystickInstanceID);
	LOAD_FUNCTION(SDL_JoystickGetVendor);
	LOAD_FUNCTION(SDL_JoystickGetProduct);
	LOAD_FUNCTION(SDL_JoystickPath);
	LOAD_FUNCTION(SDL_JoystickNumHats);
	LOAD_FUNCTION(SDL_JoystickNumButtons);
	LOAD_FUNCTION(SDL_JoystickNumAxes);
	LOAD_FUNCTION(SDL_JoystickSendEffect);
	LOAD_FUNCTION(SDL_JoystickSetLED);
	LOAD_FUNCTION(SDL_JoystickClose);
	LOAD_FUNCTION(SDL_JoystickGetType);

	LOAD_FUNCTION(SDL_HapticOpenFromJoystick);
	LOAD_FUNCTION(SDL_HapticClose);
	LOAD_FUNCTION(SDL_HapticNumEffects);
	LOAD_FUNCTION(SDL_HapticNumEffectsPlaying);
	LOAD_FUNCTION(SDL_HapticQuery);
	LOAD_FUNCTION(SDL_HapticNumAxes);
	LOAD_FUNCTION(SDL_HapticEffectSupported);
	LOAD_FUNCTION(SDL_HapticNewEffect);
	LOAD_FUNCTION(SDL_HapticUpdateEffect);
	LOAD_FUNCTION(SDL_HapticRunEffect);
	LOAD_FUNCTION(SDL_HapticStopEffect);
	LOAD_FUNCTION(SDL_HapticDestroyEffect);
	LOAD_FUNCTION(SDL_HapticGetEffectStatus);
	LOAD_FUNCTION(SDL_HapticSetGain);
	LOAD_FUNCTION(SDL_HapticSetAutocenter);
	LOAD_FUNCTION(SDL_HapticPause);
	LOAD_FUNCTION(SDL_HapticUnpause);
	LOAD_FUNCTION(SDL_HapticStopAll);
	LOAD_FUNCTION(SDL_HapticRumbleSupported);
	LOAD_FUNCTION(SDL_HapticRumbleInit);
	LOAD_FUNCTION(SDL_HapticRumblePlay);
	LOAD_FUNCTION(SDL_HapticRumbleStop);

	LOAD_FUNCTION(SDL_GetError);
	#undef STR
	#undef LOAD_FUNCTION
}
