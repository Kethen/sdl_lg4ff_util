#include <stdbool.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

#include <mutex>
#include <map>
#include <thread>

#include "logging.h"
#include "sdl_binder.h"
#include "event_loop.h"
#include "menu.h"

#define __IS_SELF
#include "main.h"
#undef __IS_SELF

std::map<SDL_JoystickID, joystick_ref> opened_joystick_map;
std::mutex opened_joystick_map_mutex;

bool sdl_initialized = false;

joystick_ref get_joystick_ref_copy(SDL_JoystickID instance){
	if(opened_joystick_map.contains(instance)){
		opened_joystick_map_mutex.lock();
		joystick_ref ref = opened_joystick_map[instance];
		opened_joystick_map_mutex.unlock();
		return ref;
	}
	return {
		.handle = 0,
		.vendor_id = 0,
		.device_id = 0,
	};
}

std::map<SDL_JoystickID, joystick_ref> get_opened_joystick_map_copy(){
	std::map<SDL_JoystickID, joystick_ref> new_map;
	opened_joystick_map_mutex.lock();
	auto itr = opened_joystick_map.begin();
	while(itr != opened_joystick_map.end()){
		new_map[itr->first] = itr->second;
		itr++;
	}
	opened_joystick_map_mutex.unlock();
	return new_map;
}

void remove_joystick_ref(SDL_JoystickID instance){
	if(opened_joystick_map.contains(instance)){
		opened_joystick_map_mutex.lock();
		opened_joystick_map.erase(instance);
		opened_joystick_map_mutex.unlock();
	}	
}

void sig_term(int _){
	exit(0);
}

int main(){
	//setenv("SDL_JOYSTICK_HIDAPI_LG4FF", "1", 1);

	init_logging();
	init_sdl_bindings();
	const char *event_log_path = "./sdl_event_log.txt";
	std::thread event_thread(sdl_event_loop, event_log_path);
	LOG("started event loop, logging at %s\n", event_log_path);

	while(!sdl_initialized){
		sleep(0);
	}

	signal(SIGTERM, sig_term);
	signal(SIGINT, sig_term);

	menu_loop(std::string(event_log_path));

	event_thread.join();

	return 0;
}
