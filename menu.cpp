#include <string>
#include <format>
#include <iostream>

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>

#include "main.h"
#include "logging.h"
#include "mode_change.h"
#include "device_ids.h"

#define STR(s) #s

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
	LOG("3. Run FFB test routine\n");
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
		LOG("%d. %04x:%04x %s %s", itr->first, itr->second.vendor_id, itr->second.device_id, itr->second.path, joystick_get_type_name(itr->second.type).c_str());
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
		if(SDL_SetJoystickLED(opened_joystick_map[wheel_id].handle, sdl_led_intensity, 0, 0) == 0){
			status = std::format("SDL_JoystickSetLED on wheel {:d} succeeded", wheel_id);
		}else{
			status = std::format("SDL_JoystickSetLED failed on wheel {:d}", wheel_id);
		}
	}
	opened_joystick_map_mutex.unlock();

	return status;
}

static std::string run_haptic_test_routine(){
	clear_console();

	int wheel_id = wheel_select_menu();

	if(wheel_id == -1){
		return std::format("wheel id {:d} not found.", wheel_id);
	}

	opened_joystick_map_mutex.lock();
	if(opened_joystick_map.contains(wheel_id)){
		SDL_Joystick *joystick = opened_joystick_map[wheel_id].handle;
		SDL_Haptic *haptic = SDL_OpenHapticFromJoystick(joystick);

		#define ASSERT(c) {\
			if(!(c)){ \
				SDL_CloseHaptic(haptic); \
				return std::format("test failed at file {} line {:d}: {}, {}", __FILE__, __LINE__, STR(c), SDL_GetError()); \
			} \
		}

		ASSERT(haptic != NULL);

		ASSERT(SDL_GetMaxHapticEffects(haptic) == 16);
		ASSERT(SDL_GetMaxHapticEffectsPlaying(haptic) == 16);

		uint32_t supported_effects = SDL_GetHapticFeatures(haptic);
		ASSERT(supported_effects & SDL_HAPTIC_CONSTANT);
		ASSERT(supported_effects & SDL_HAPTIC_SINE);
		ASSERT(!(supported_effects & SDL_HAPTIC_LEFTRIGHT));
		ASSERT(supported_effects & SDL_HAPTIC_SQUARE);
		ASSERT(supported_effects & SDL_HAPTIC_TRIANGLE);
		ASSERT(supported_effects & SDL_HAPTIC_SAWTOOTHUP);
		ASSERT(supported_effects & SDL_HAPTIC_SAWTOOTHDOWN);
		ASSERT(supported_effects & SDL_HAPTIC_RAMP);
		ASSERT(supported_effects & SDL_HAPTIC_SPRING);
		ASSERT(supported_effects & SDL_HAPTIC_DAMPER);
		ASSERT(!(supported_effects & SDL_HAPTIC_INERTIA));
		ASSERT(supported_effects & SDL_HAPTIC_FRICTION);
		ASSERT(!(supported_effects & SDL_HAPTIC_CUSTOM));
		ASSERT(supported_effects & SDL_HAPTIC_GAIN);
		ASSERT(supported_effects & SDL_HAPTIC_AUTOCENTER);
		ASSERT(supported_effects & SDL_HAPTIC_STATUS);
		ASSERT(!(supported_effects & SDL_HAPTIC_PAUSE));

		ASSERT(SDL_GetNumHapticAxes(haptic) == 1);

		#define ASSERT_EFFECT_SUPPORTED(t) { \
			SDL_HapticEffect e; \
			e.type = t; \
			ASSERT(SDL_HapticEffectSupported(haptic, &e)); \
		}
		#define ASSERT_EFFECT_NOT_SUPPORTED(t) { \
			SDL_HapticEffect e; \
			e.type = t; \
			ASSERT(!SDL_HapticEffectSupported(haptic, &e)); \
		}
		ASSERT_EFFECT_SUPPORTED(SDL_HAPTIC_CONSTANT);
		ASSERT_EFFECT_SUPPORTED(SDL_HAPTIC_SINE);
		ASSERT_EFFECT_SUPPORTED(SDL_HAPTIC_SQUARE);
		ASSERT_EFFECT_SUPPORTED(SDL_HAPTIC_TRIANGLE);
		ASSERT_EFFECT_SUPPORTED(SDL_HAPTIC_SAWTOOTHUP);
		ASSERT_EFFECT_SUPPORTED(SDL_HAPTIC_SAWTOOTHDOWN);
		ASSERT_EFFECT_SUPPORTED(SDL_HAPTIC_RAMP);
		ASSERT_EFFECT_SUPPORTED(SDL_HAPTIC_SPRING);
		ASSERT_EFFECT_SUPPORTED(SDL_HAPTIC_DAMPER);
		ASSERT_EFFECT_NOT_SUPPORTED(SDL_HAPTIC_INERTIA);
		ASSERT_EFFECT_SUPPORTED(SDL_HAPTIC_FRICTION);
		ASSERT_EFFECT_NOT_SUPPORTED(SDL_HAPTIC_CUSTOM);
		#undef ASSERT_EFFECT_SUPPORTED
		#undef ASSERT_EFFECT_NOT_SUPPORTED

		ASSERT(SDL_HapticRumbleSupported(haptic));

		LOG("Capability check OK\n");

		SDL_HapticEffect effect = {0};
		effect.type = SDL_HAPTIC_CONSTANT;
		effect.constant.direction.type = SDL_HAPTIC_STEERING_AXIS;
		effect.constant.length = 5000;
		effect.constant.delay = 0;
		effect.constant.level = 65535 / 8;

		int constant_effect_id = SDL_CreateHapticEffect(haptic, &effect);
		ASSERT(constant_effect_id == 0);
		LOG("Created constant effect with id %d\n", constant_effect_id);
		ASSERT(SDL_RunHapticEffect(haptic, constant_effect_id, 1));
		LOG("Playing effect, please verify that it is running\n");
		sleep(1);
		ASSERT(SDL_GetHapticEffectStatus(haptic, constant_effect_id));
		sleep(5);
		ASSERT(!SDL_GetHapticEffectStatus(haptic, constant_effect_id));
		LOG("Effect should have stopped naturally, please verify\n");
		sleep(3);

		LOG("Playing effect again\n");
		ASSERT(SDL_RunHapticEffect(haptic, constant_effect_id, 2));
		sleep(2);
		LOG("Stopping effect using SDL_HapticStopEffect, please verify\n");
		ASSERT(SDL_StopHapticEffect(haptic, constant_effect_id));
		sleep(3);

		LOG("Playing effect again\n");
		ASSERT(SDL_RunHapticEffect(haptic, constant_effect_id, 2));
		sleep(2);
		LOG("Stopping effect using SDL_HapticStopAll, please verify\n");
		ASSERT(SDL_StopHapticEffects(haptic));
		sleep(3);

		LOG("Testing SDL_HapticUpdateEffect, please verify direction changes\n");
		effect.constant.length = SDL_HAPTIC_INFINITY;
		ASSERT(SDL_UpdateHapticEffect(haptic, constant_effect_id, &effect));
		ASSERT(SDL_RunHapticEffect(haptic, constant_effect_id, 2));
		sleep(2);
		effect.constant.level = -65535 / 8;
		ASSERT(SDL_UpdateHapticEffect(haptic, constant_effect_id, &effect));
		sleep(2);
		effect.constant.level = 65535 / 8;
		ASSERT(SDL_UpdateHapticEffect(haptic, constant_effect_id, &effect));
		sleep(2);
		LOG("Stopping effect, please verify\n");
		ASSERT(SDL_StopHapticEffect(haptic, constant_effect_id));
		sleep(3);

		LOG("Testing Rumble\n");
		ASSERT(SDL_InitHapticRumble(haptic));
		ASSERT(SDL_PlayHapticRumble(haptic, 0.5, 5000));
		sleep(6);
		LOG("Rumble should have naturally stopped now, please verify\n");
		sleep(3);

		LOG("Starting rumble again\n");
		ASSERT(SDL_PlayHapticRumble(haptic, 0.5, 10000));
		sleep(2);
		LOG("Stopping rumble, please verify\n")
		ASSERT(SDL_StopHapticRumble(haptic));
		sleep(3);

		SDL_DestroyHapticEffect(haptic, constant_effect_id);
		memset(&effect, 0, sizeof(SDL_HapticEffect));
		effect.type = SDL_HAPTIC_FRICTION;
		effect.condition.direction.type = SDL_HAPTIC_STEERING_AXIS;
		effect.condition.right_sat[0] = 0x7fff;
		effect.condition.left_sat[0] = 0x7fff;
		effect.condition.right_coeff[0] = 0x2000;
		effect.condition.left_coeff[0] = 0x2000;
		effect.condition.length = SDL_HAPTIC_INFINITY;

		int friction_effect_id = SDL_CreateHapticEffect(haptic, &effect);
		ASSERT(friction_effect_id == 0);
		ASSERT(SDL_RunHapticEffect(haptic, friction_effect_id, 1));
		LOG("Testing friction effect, please verify\n");
		sleep(5);
		ASSERT(SDL_StopHapticEffect(haptic, friction_effect_id));
		LOG("Friction effect stopped, please verify\n");
		sleep(3);

		effect.type = SDL_HAPTIC_DAMPER;

		int damper_effect_id = SDL_CreateHapticEffect(haptic, &effect);
		// sdl internally allocated a sine effect for rumble
		ASSERT(damper_effect_id == 2);
		ASSERT(SDL_RunHapticEffect(haptic, damper_effect_id, 1));
		LOG("Testing damper effect, please verify\n");
		sleep(5);
		ASSERT(SDL_StopHapticEffect(haptic, damper_effect_id));
		LOG("Damper effect stopped, please verify\n");
		sleep(3);

		effect.type = SDL_HAPTIC_SPRING;;

		int spring_effect_id = SDL_CreateHapticEffect(haptic, &effect);
		ASSERT(spring_effect_id == 3);
		ASSERT(SDL_RunHapticEffect(haptic, spring_effect_id, 1));
		LOG("Testing spring effect, please verify\n");
		sleep(5);
		ASSERT(SDL_StopHapticEffect(haptic, spring_effect_id));
		LOG("Spring effect stopped, please verify\n");
		sleep(3);

		LOG("Testing autocenter, please verify\n");
		ASSERT(SDL_SetHapticAutocenter(haptic, 50));
		sleep(5);
		ASSERT(SDL_SetHapticAutocenter(haptic, 0));
		LOG("Autocenter stopped, please verify\n");
		sleep(3);

		LOG("Testing ramp effect\n");
		memset(&effect, 0, sizeof(SDL_HapticEffect));
		effect.type = SDL_HAPTIC_RAMP;
		effect.ramp.direction.type = SDL_HAPTIC_STEERING_AXIS;
		effect.ramp.length = 5000;
		effect.ramp.start = 65535 / 8;
		effect.ramp.end = 65535 / 4;
		int ramp_effect_id = SDL_CreateHapticEffect(haptic, &effect);
		ASSERT(ramp_effect_id == 4);
		ASSERT(SDL_RunHapticEffect(haptic, ramp_effect_id, 1));
		sleep(6);
		LOG("Ramp effect should have stopped natrually\n");
		sleep(3);
		
		LOG("Starting rummble again\n");
		ASSERT(SDL_PlayHapticRumble(haptic, 0.5, SDL_HAPTIC_INFINITY));
		sleep(3);
		LOG("Adjusting gain to 0\n");
		ASSERT(SDL_SetHapticGain(haptic, 0));
		sleep(3);
		LOG("Adjusting gain to 50\n");
		ASSERT(SDL_SetHapticGain(haptic, 50));
		sleep(3);
		LOG("Adjusting gain to 100\n");
		ASSERT(SDL_SetHapticGain(haptic, 100));
		sleep(3);
		LOG("Rumble should stop now\n");
		ASSERT(SDL_StopHapticRumble(haptic));
		sleep(3);

		#undef ASSERT
		SDL_CloseHaptic(haptic);

	}
	opened_joystick_map_mutex.unlock();

	return std::string("haptic test routine finished");
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
			case 3:
				status = run_haptic_test_routine();
				break;
			default:
				status = std::format("bad option {:d}", input_val);
		}
	}
}

