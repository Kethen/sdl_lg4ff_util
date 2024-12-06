#include "sdl_joystick.h"

#include <string>
#include <format>

std::string joystick_get_hat_name(int hat_value){
	switch(hat_value){
		case SDL_HAT_CENTERED:
			return std::string("center");
		case SDL_HAT_UP:
			return std::string("up");
		case SDL_HAT_RIGHT:
			return std::string("right");
		case SDL_HAT_DOWN:
			return std::string("down");
		case SDL_HAT_LEFT:
			return std::string("left");
		case SDL_HAT_RIGHTUP:
			return std::string("right-up");
		case SDL_HAT_RIGHTDOWN:
			return std::string("right-down");
		case SDL_HAT_LEFTUP:
			return std::string("left-up");
		case SDL_HAT_LEFTDOWN:
			return std::string("left-down");
		default:
			return std::format("unknown hat {:d}", hat_value);
	}
}

std::string joystick_get_type_name(SDL_JoystickType type){
	switch(type){
		case SDL_JOYSTICK_TYPE_UNKNOWN:
			return "unknown";
		case SDL_JOYSTICK_TYPE_GAMEPAD:
			return "game pad";
		case SDL_JOYSTICK_TYPE_WHEEL:
			return "wheel";
		case SDL_JOYSTICK_TYPE_ARCADE_STICK:
			return "arcade stick";
		case SDL_JOYSTICK_TYPE_FLIGHT_STICK:
			return "flight stick";
		case SDL_JOYSTICK_TYPE_DANCE_PAD:
			return "dance pad";
		case SDL_JOYSTICK_TYPE_GUITAR:
			return "guitar";
		case SDL_JOYSTICK_TYPE_DRUM_KIT:
			return "drum kit";
		case SDL_JOYSTICK_TYPE_ARCADE_PAD:
			return "arcade pad";
		case SDL_JOYSTICK_TYPE_THROTTLE:
			return "throttle";
		default:
			return "unknown (undefined)";
	}
}
