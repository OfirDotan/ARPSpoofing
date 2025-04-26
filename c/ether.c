#include "ether.h"
#include <string.h>

int initialize_ethernet(ethernet_frame* frame, uint8_t destinationAddress[], uint8_t sourceAddress[], uint16_t ethernetType, uint8_t payload[]) {
	if (sizeof(destinationAddress) != 6 || sizeof(sourceAddress) != 6 || sizeof(payload) > 1500){
		return -1;
	}
	
	memcpy(frame->ethernetHeader.destinationAddress, destinationAddress, MAC_ADDRESS_SIZE);
	memcpy(frame->ethernetHeader.sourceAddress, sourceAddress, MAC_ADDRESS_SIZE);
	frame->ethernetHeader.ethernetType = ethernetType;
	
	if (payload != NULL) {
		memcpy(frame->payload, payload, sizeof(payload));
	}

	return 0;
}