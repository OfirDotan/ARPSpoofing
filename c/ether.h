#ifndef ETHER_H
#define ETHER_H

#define MAC_ADDRESS_SIZE 6
#define PAYLOAD_SIZE_LIMIT 1500

#include <stdint.h>

typedef struct ether_header {
	uint8_t destinationAddress[MAC_ADDRESS_SIZE];
	uint8_t sourceAddress[MAC_ADDRESS_SIZE];
	uint16_t ethernetType;
} ether_header;


typedef struct ethernet_frame {
	ether_header ethernetHeader; // Ethernet header
	uint8_t payload[PAYLOAD_SIZE_LIMIT]; // Payload
} ethernet_frame;	

int initialize_ethernet(ethernet_frame* frame, uint8_t destinationAddress[], uint8_t sourceAddress[], uint16_t ethernetType, uint8_t payload[]);

#endif ETHER_H