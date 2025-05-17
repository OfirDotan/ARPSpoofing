#include <stdio.h>
#include "ether.h"
#include "arp.h"

int main() {
	//test ether send

	ethernet_frame frame;
	uint8_t source_address[MAC_ADDRESS_SIZE] = {0x00, 0x0C, 0x29, 0xF4, 0x92, 0xA0};
	uint8_t dest_address[MAC_ADDRESS_SIZE] = {0x00, 0x0C, 0x29, 0xF4, 0x92, 0xA0};
    uint8_t payload[46] = {0xAB};

	initialize_ethernet(&frame, dest_address, 6, source_address, 6, 0x88B5, payload, 46);
	

	int result = send_frame(&frame);
	printf("Status: %d\n", result);
}