#include "device_ids.h"

#include <stdbool.h>

bool device_supported(uint32_t vendor_id, uint32_t device_id){
	if(vendor_id != USB_VENDOR_ID_LOGITECH){
		return false;
	}
	for(int i = 0;i < sizeof(supported_ids) / sizeof(uint32_t);i++){
		if(supported_ids[i] == device_id){
			return true;
		}
	}
	return false;
}
