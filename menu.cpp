#include <string>
#include <format>
#include <iostream>

#include <stdlib.h>

#include "main.h"
#include "logging.h"
#include "mode_change.h"
#include "device_ids.h"


static void clear_console(){
	LOG("\e[1;1H\e[2J");
}

static int get_input_int(){
	std::string input;
	std::getline(std::cin, input);
	int input_val = atoi(input.c_str());

	return input_val;
}

static void print_actions(std::string status){	
	clear_console();

	LOG("-- sdl_lg4ff_util --\n");

	LOG("actions:\n\n");

	LOG("1. Change wheel mode\n");
	LOG("2. Set wheel LED\n");
	LOG("\n\n")

	LOG("Status:\n");
	LOG("%s", status.c_str());
	LOG("\n\n");

	LOG("Select action:");
}

static int wheel_select_menu(){
	LOG("available wheels:\n\n");

	auto map_copy = get_opened_joystick_map_copy();
	auto itr = map_copy.begin();
	while(itr != map_copy.end()){
		LOG("%d. %04x:%04x %s", itr->first, itr->second.vendor_id, itr->second.device_id, itr->second.path);
		itr++;
	}

	LOG("\n\n");

	LOG("Select wheel:");
	int wheel_id = get_input_int();

	if(!map_copy.contains(wheel_id)){
		return -1;
	}

	return wheel_id;
}

static std::string change_mode_menu(){
	clear_console();

	int wheel_id = wheel_select_menu();

	if(wheel_id == -1){
		return std::format("wheel id {:d} not found.", wheel_id);
	}

	LOG("modes:\n\n");
	LOG("1. G29\n");
	LOG("2. G27\n");
	LOG("3. G25\n");
	LOG("4. DFGT\n");
	LOG("5. DFP\n");
	LOG("6. DFEX\n");

	LOG("\n\n");

	LOG("Select mode:");
	int mode = get_input_int();

	int mode_change_id = 0;
	switch(mode){
		case 1:
			mode_change_id = USB_DEVICE_ID_LOGITECH_G29_WHEEL;
			break;
		case 2:
			mode_change_id = USB_DEVICE_ID_LOGITECH_G27_WHEEL;
			break;
		case 3:
			mode_change_id = USB_DEVICE_ID_LOGITECH_G25_WHEEL;
			break;
		case 4:
			mode_change_id = USB_DEVICE_ID_LOGITECH_DFGT_WHEEL;
			break;
		case 5:
			mode_change_id = USB_DEVICE_ID_LOGITECH_DFP_WHEEL;
			break;
		case 6:
			mode_change_id = USB_DEVICE_ID_LOGITECH_WHEEL;
			break;
		default:
			return std::format("bad mode option {:d}", mode);
	}

	std::string status = std::format("wheel {:d} removed during mode change", wheel_id);
	opened_joystick_map_mutex.lock();
	if(opened_joystick_map.contains(wheel_id)){
		if(mode_change(opened_joystick_map[wheel_id].handle, mode_change_id) == 0){
			status = std::format("mode change command ({:#04x}) sent to wheel {:d}", mode_change_id, wheel_id);
		}else{
			status = std::format("failed sending mode change command to wheel {:d}", wheel_id);
		}
	}
	opened_joystick_map_mutex.unlock();

	return status;
}

static std::string set_led_menu(){
	clear_console();

	int wheel_id = wheel_select_menu();

	if(wheel_id == -1){
		return std::format("wheel id {:d} not found.", wheel_id);
	}

	LOG("Set LED intensity (0 - 5):");
	int led_intensity = get_input_int();
	if(led_intensity < 0){
		led_intensity = 0;
	}
	if(led_intensity > 5){
		led_intensity = 5;
	}
	uint8_t sdl_led_intensity = (255 * led_intensity) / 5;

	std::string status = std::format("wheel {:d} removed during SDL_JoystickSetLED", wheel_id);
	opened_joystick_map_mutex.lock();
	if(opened_joystick_map.contains(wheel_id)){
		if(SDL_JoystickSetLED(opened_joystick_map[wheel_id].handle, sdl_led_intensity, 0, 0) == 0){
			status = std::format("SDL_JoystickSetLED on wheel {:d} succeeded", wheel_id);
		}else{
			status = std::format("SDL_JoystickSetLED failed on wheel {:d}", wheel_id);
		}
	}
	opened_joystick_map_mutex.unlock();

	return status;
}

void menu_loop(std::string log_path){
	std::string status = std::format("Events are logged at {}", log_path);

	while(true){
		print_actions(status);
		int input_val = get_input_int();
		switch(input_val){
			case 1:
				status = change_mode_menu();
				break;
			case 2:
				status = set_led_menu();
				break;
			default:
				status = std::format("bad option {:d}", input_val);
		}
	}
}

