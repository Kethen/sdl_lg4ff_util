#include <stdint.h>

#include "sdl_binder.h"
#include "device_ids.h"
#include "logging.h"

static int hid_write(SDL_Joystick *joystick, uint8_t *buf, size_t size){
	return SDL_JoystickSendEffect(joystick, buf, size);
}

int mode_change(SDL_Joystick *joystick, int mode){
	int ret = 0;
	switch(mode){
		case USB_DEVICE_ID_LOGITECH_G29_WHEEL:{
			uint8_t cmd[] = {0xf8, 0x09, 0x05, 0x01, 0x01, 0x00, 0x00};
			ret = hid_write(joystick, cmd, sizeof(cmd));
			break;
		}
		case USB_DEVICE_ID_LOGITECH_G27_WHEEL:{
			uint8_t cmd[] = {0xf8, 0x09, 0x04, 0x01, 0x00, 0x00, 0x00};
			ret = hid_write(joystick, cmd, sizeof(cmd));
			break;
		}
		case USB_DEVICE_ID_LOGITECH_G25_WHEEL:{
			uint8_t cmd[] = {0xf8, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00};
			ret = hid_write(joystick, cmd, sizeof(cmd));
			break;
		}
		case USB_DEVICE_ID_LOGITECH_DFGT_WHEEL:{
			uint8_t cmd[] = {0xf8, 0x09, 0x03, 0x01, 0x00, 0x00, 0x00};
			ret = hid_write(joystick, cmd, sizeof(cmd));
			break;
		}
		case USB_DEVICE_ID_LOGITECH_DFP_WHEEL:{
			uint8_t cmd[] = {0xf8, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00};
			ret = hid_write(joystick, cmd, sizeof(cmd));
			break;
		}
		case USB_DEVICE_ID_LOGITECH_WHEEL:{
			uint8_t cmd[] = {0xf8, 0x09, 0x00, 0x01, 0x00, 0x00, 0x00};
			ret = hid_write(joystick, cmd, sizeof(cmd));
			break;
		}
		default:{
			LOG_ERR("unknown product id 0x%04x for mode switching\n", mode);
			exit(1);
		}
	}
	if(ret <= 0){
		return -1;
	}
	return 0;
}
