#ifndef ETHER_H
#define ETHER_H

#include <stdint.h>

typedef struct ether_header {
	uint8_t destinationAddress[6];
	uint8_t sourceAddress[6];
	uint16_t ethernetType;
} ether_header;


typedef struct ethernet_frame {
	ether_header ethernetHeader; // Ethernet header
	uint8_t payload[1500]; // Payload
} ethernet_frame;	

#endif ETHER_H