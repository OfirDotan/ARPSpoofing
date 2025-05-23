#ifndef ETHER_H
#define ETHER_H

#include <stddef.h> // For size_t
#include <stdint.h> // For uint8_t, uint16_t

#define MAC_ADDRESS_SIZE 6
#define PAYLOAD_SIZE_LIMIT 1500
#define MINIMUM_PAYLOAD_SIZE 46
#define ALL_PROTOCOLS 0x0003


typedef struct {
	uint8_t destination_address[MAC_ADDRESS_SIZE];
	uint8_t source_address[MAC_ADDRESS_SIZE];
	uint16_t ethernet_type;
} ether_header;


typedef struct {
	ether_header ethernet_header; // Ethernet header
	uint8_t payload[PAYLOAD_SIZE_LIMIT]; // Payload data
	uint8_t payload_size;
} ethernet_frame;	

int initialize_ethernet(ethernet_frame* frame, 
						uint8_t destination_address[], size_t destination_size, 
						uint8_t source_address[], size_t source_size, 
						uint16_t ethernet_type, uint8_t payload[], size_t payload_size);


int send_frame(ethernet_frame* frame, int raw_socket_fd);

#endif