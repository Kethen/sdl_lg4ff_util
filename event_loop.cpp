#include <fstream>
#include <format>

#include <string.h>

#include "logging.h"
#include "sdl_binder.h"
#include "main.h"
#include "device_ids.h"

void sdl_event_loop(const char *log_path){
	std::ofstream log_out;
	try{
		log_out.open(log_path, std::ios_base::trunc);
	}catch(...){
		LOG_ERR("cannot open %s for event logging, terminating\n");
	}
	LOG("opened %s for event logging\n", log_path);

	int ret = SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_HAPTIC);
	if(ret != 0){
		LOG_ERR("failed initializing SDL, terminating\n");
		exit(1);
	}
	sdl_initialized = true;

	SDL_JoystickEventState(SDL_ENABLE);

	while(true){
		SDL_Event event = {0};
		SDL_WaitEvent(&event);

		switch(event.type){
			case SDL_JOYDEVICEADDED:{
				SDL_JoyDeviceEvent *e = (SDL_JoyDeviceEvent *)&event;
				SDL_Joystick *handle = SDL_JoystickOpen(e->which);
				if(handle == NULL){
					log_out << std::format("failed opening joy device with index {:d}\n", e->which) << std::flush;
					break;
				}
				SDL_JoystickID id = SDL_JoystickInstanceID(handle);
				joystick_ref ref = {
					.handle = handle,
					.vendor_id = SDL_JoystickGetVendor(handle),
					.device_id = SDL_JoystickGetProduct(handle),
				};
				memset(ref.path, 0, sizeof(ref.path));
				strncpy(ref.path, SDL_JoystickPath(handle), sizeof(ref.path) - 1);
				if(device_supported(ref.vendor_id, ref.device_id)){
					opened_joystick_map_mutex.lock();
					opened_joystick_map[id] = ref;
					opened_joystick_map_mutex.unlock();
					log_out << std::format("opened joydevice {} i: {:d} v: {:#04x} d: {:#04x}\n", ref.path, id, ref.vendor_id, ref.device_id) << std::flush;
				}else{
					log_out << std::format("ignoring device {} i: {:d} v: {:#04x} d: {:#04x}\n", ref.path, id, ref.vendor_id, ref.device_id) << std::flush;
					SDL_JoystickClose(handle);
				}
				break;
			}
			case SDL_JOYDEVICEREMOVED:{
				SDL_JoyDeviceEvent *e = (SDL_JoyDeviceEvent *)&event;
				joystick_ref ref = get_joystick_ref_copy(e->which);
				if(ref.handle != NULL){
					remove_joystick_ref(e->which);
					log_out << std::format("removed joydevice i: {:d} v: {:#04x} d: {:#04x}\n", e->which, ref.vendor_id, ref.device_id) << std::flush;
				}
				break;
			}
			case SDL_JOYBUTTONDOWN:
			case SDL_JOYBUTTONUP:{
				SDL_JoyButtonEvent *e = (SDL_JoyButtonEvent *)&event;
				joystick_ref ref = get_joystick_ref_copy(e->which);
				if(ref.handle != NULL){
					log_out << std::format("i: {:d} v: {:#04x} d: {:#04x} button: {:d} {:d}\n", e->which, ref.vendor_id, ref.device_id, e->button, e->state) << std::flush;
				}
				break;
			}
			case SDL_JOYHATMOTION:{
				SDL_JoyHatEvent *e = (SDL_JoyHatEvent *)&event;
				joystick_ref ref = get_joystick_ref_copy(e->which);
				if(ref.handle != NULL){
					log_out << std::format("i: {:d} v: {:#04x} d: {:#04x} hat: {:d} {:d}\n", e->which, ref.vendor_id, ref.device_id, e->hat, e->value) << std::flush;
				}
				break;
			}
			case SDL_JOYAXISMOTION:{
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
