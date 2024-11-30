#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

#include "sdl_common.h"
#include "sdl_events.h"
#include "sdl_joystick.h"

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
void (*SDL_JoystickClose)(SDL_Joystick *joystick);

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
	LOAD_FUNCTION(SDL_JoystickClose);
	#undef STR
	#undef LOAD_FUNCTION
}