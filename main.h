#ifndef __MAIN_H
#define __MAIN_H

#include <stdbool.h>
#include <stdint.h>

#include <mutex>
#include <map>
#include "sdl_binder.h"
#include "sdl_joystick.h"

typedef struct _joystick_ref {
	SDL_Joystick *handle;
	uint32_t vendor_id;
	uint32_t device_id;
} joystick_ref;

joystick_ref get_joystick_ref_copy(SDL_JoystickID instance);
void remove_joystick_ref(SDL_JoystickID instance);

#ifndef __IS_SELF
extern std::map<SDL_JoystickID, joystick_ref> opened_joystick_map;
extern bool sdl_initialized;
extern std::mutex opened_joystick_map_mutex;
#endif

#endif //__MAIN_H
