#include "arp.h"
#include "ether.h"
#include <string.h>
#include <stddef.h>
#include <arpa/inet.h>

/*
Returns 0 on success, -1 on failure
*/
int initialize_arp(ethernet_frame* frame, 
                    uint8_t sender_hardware_address[], size_t sender_hardware_size,
                    uint8_t target_hardware_address[], size_t targetHardware_size,
                    uint8_t sender_protcol_address[], size_t sender_protcol_size,
                    uint8_t target_protcol_address[], size_t target_protcol_size,
                    uint16_t operation){
	if (sender_hardware_size != 6 || targetHardware_size != 6) {
		return -1;
	}
	if (sender_protcol_size != 4 || target_protcol_size != 4) {
		return -1;
	}

	initialize_ethernet(frame, target_hardware_address, 6, sender_hardware_address, 6, ARP_CODE, 0, 0);

	//ARP header init
	arp_header frame_arp_header;
    frame_arp_header.hardware_type = htons(ETHERNET_CODE);
	frame_arp_header.protocol_type = htons(IPV4_CODE);
	frame_arp_header.hardware_length = MAC_ADDRESS_SIZE;
	frame_arp_header.protocol_length= IP_ADDRESS_SIZE;
	frame_arp_header.operation = htons(operation);
	
	memcpy(frame_arp_header.sender_hardware_address, sender_hardware_address, MAC_ADDRESS_SIZE);
	if (operation == ARP_REQUEST) {
		memset(frame_arp_header.target_hardware_address, 0, MAC_ADDRESS_SIZE);
	}
	else {
		memcpy(frame_arp_header.target_hardware_address, target_hardware_address, MAC_ADDRESS_SIZE);
	}

	memcpy(frame_arp_header.sender_protocol_address, sender_protcol_address, IP_ADDRESS_SIZE);
	memcpy(frame_arp_header.target_protocol_address, target_protcol_address, IP_ADDRESS_SIZE);

    memcpy(frame->payload, &frame_arp_header, sizeof(arp_header));
    frame->payload_size = sizeof(arp_header);

	return 0;
}