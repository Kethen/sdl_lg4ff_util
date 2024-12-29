#include <fstream>
#include <format>

#include <string.h>
#include <stdbool.h>

#include "logging.h"
#include "sdl_binder.h"
#include "main.h"
#include "device_ids.h"
#include "sdl_joystick.h"

void sdl_event_loop(const char *log_path){
	std::ofstream log_out;
	try{
		log_out.open(log_path, std::ios_base::trunc);
	}catch(...){
		LOG_ERR("cannot open %s for event logging, terminating\n", log_path);
	}
	LOG("opened %s for event logging\n", log_path);

	if(!SDL_Init(SDL_INIT_GAMEPAD | SDL_INIT_HAPTIC)){
		LOG_ERR("failed initializing SDL, %s, terminating\n", SDL_GetError());
		exit(1);
	}
	sdl_initialized = true;

	SDL_SetJoystickEventsEnabled(true);

	while(true){
		SDL_Event event = {0};
		if(!SDL_WaitEvent(&event)){
			continue;
		}

		switch(event.type){
			case SDL_EVENT_JOYSTICK_ADDED:{
				SDL_JoyDeviceEvent *e = (SDL_JoyDeviceEvent *)&event;
				SDL_Joystick *handle = SDL_OpenJoystick(e->which);
				bool is_controller = SDL_IsGamepad(e->which);
				if(handle == NULL){
					log_out << std::format("failed opening joy device with index {:d}\n", e->which) << std::flush;
					break;
				}
				SDL_JoystickID id = SDL_GetJoystickID(handle);
				joystick_ref ref = {
					.handle = handle,
					.vendor_id = SDL_GetJoystickVendor(handle),
					.device_id = SDL_GetJoystickProduct(handle),
					.type = SDL_GetJoystickType(handle),
					.is_controller = is_controller,
				};
				memset(ref.path, 0, sizeof(ref.path));
				const char *path = SDL_GetJoystickPath(handle);
				strncpy(ref.path, path == NULL ? SDL_GetError() : path, sizeof(ref.path) - 1);
				memset(ref.name, 0, sizeof(ref.name));
				const char *name = SDL_GetJoystickName(handle);
				strncpy(ref.name, name == NULL ? SDL_GetError() : name, sizeof(ref.name) - 1);
				if(device_supported(ref.vendor_id, ref.device_id)){
					opened_joystick_map_mutex.lock();
					opened_joystick_map[id] = ref;
					opened_joystick_map_mutex.unlock();
					log_out << std::format("opened joydevice {} {} i: {:d} v: {:#04x} d: {:#04x} t: {} controller: {}\n", ref.path, ref.name, id, ref.vendor_id, ref.device_id, joystick_get_type_name(ref.type), ref.is_controller ? "yes" : "no") << std::flush;
					log_out << std::format("hats: {:d}, buttons: {:d}, axes: {:d}\n", SDL_GetNumJoystickHats(handle), SDL_GetNumJoystickButtons(handle), SDL_GetNumJoystickAxes(handle)) << std::flush;
				}else{
					log_out << std::format("ignoring device {} {} i: {:d} v: {:#04x} d: {:#04x} t: {} controller: {}\n", ref.path, ref.name, id, ref.vendor_id, ref.device_id, joystick_get_type_name(ref.type), ref.is_controller ? "yes" : "no") << std::flush;
					SDL_CloseJoystick(handle);
				}
				break;
			}
			case SDL_EVENT_JOYSTICK_REMOVED:{
				SDL_JoyDeviceEvent *e = (SDL_JoyDeviceEvent *)&event;
				joystick_ref ref = get_joystick_ref_copy(e->which);
				if(ref.handle != NULL){
					SDL_CloseJoystick(ref.handle);
					remove_joystick_ref(e->which);
					log_out << std::format("removed joydevice {} i: {:d} v: {:#04x} d: {:#04x}\n", ref.path, e->which, ref.vendor_id, ref.device_id) << std::flush;
					if(SDL_GetJoystickID(ref.handle) != 0){
						log_out << std::format("warning, dangling joystick reference {:#016x}\n", (uint64_t)ref.handle) << std::flush;
					}
				}else{
					log_out << std::format("untracked device {} removed\n", e->which) << std::flush;
				}
				break;
			}
			case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
			case SDL_EVENT_JOYSTICK_BUTTON_UP:{
				SDL_JoyButtonEvent *e = (SDL_JoyButtonEvent *)&event;
				joystick_ref ref = get_joystick_ref_copy(e->which);
				if(ref.handle != NULL){
					log_out << std::format("ic: {:d} v: {:#04x} d: {:#04x} button: {:d} {:d}\n", e->which, ref.vendor_id, ref.device_id, e->button, e->down) << std::flush;
				}
				break;
			}
			case SDL_EVENT_JOYSTICK_HAT_MOTION:{
				SDL_JoyHatEvent *e = (SDL_JoyHatEvent *)&event;
				joystick_ref ref = get_joystick_ref_copy(e->which);
				if(ref.handle != NULL){
					log_out << std::format("i: {:d} v: {:#04x} d: {:#04x} hat: {:d} {}\n", e->which, ref.vendor_id, ref.device_id, e->hat, joystick_get_hat_name(e->value)) << std::flush;
				}
				break;
			}
			case SDL_EVENT_JOYSTICK_AXIS_MOTION:{
				SDL_JoyAxisEvent *e = (SDL_JoyAxisEvent *)&event;
				joystick_ref ref = get_joystick_ref_copy(e->which);
				if(ref.handle != NULL){
					log_out << std::format("i: {:d} v: {:#04x} d: {:#04x} axis: {:d} {:d}\n", e->which, ref.vendor_id, ref.device_id, e->axis, e->value) << std::flush;
				}
				break;
			}
		}
	}
}
