#ifndef ARP_H
#define ARP_H

#include "ether.h"
#include <stddef.h> // For size_t
#include <stdint.h> // For uint8_t, uint16_t


#define MAC_ADDRESS_SIZE 6
#define IP_ADDRESS_SIZE 4
#define ARP_REQUEST 1
#define ARP_RESPONSE 2
#define ETHERNET_CODE 1
#define IPV4_CODE 0x0800
#define ARP_CODE 0x0806


typedef struct arp_header {
	uint16_t hardware_type;
	uint16_t protocol_type;
	uint8_t hardware_length;
	uint8_t protocol_length;
	uint16_t operation; // 1 - Request, 2 - Reply
	
	uint8_t sender_hardware_address[MAC_ADDRESS_SIZE]; // Sender's MAC address
	uint8_t sender_protocol_address[IP_ADDRESS_SIZE]; //Sender's IP address
	
    uint8_t target_hardware_address[MAC_ADDRESS_SIZE]; // Target's MAC address
    uint8_t target_protocol_address[IP_ADDRESS_SIZE]; // Target's IP address
} arp_header;


typedef struct arp_packet {
	ethernet_frame frame;
	arp_header arpHeader;
} arp_packet;

int initialize_arp(ethernet_frame* frame, 
                    uint8_t sender_hardware_address[], size_t sender_hardware_size,
                    uint8_t target_hardware_address[], size_t targetHardware_size,
                    uint8_t sender_protcol_address[], size_t sender_protcol_size,
                    uint8_t target_protcol_address[], size_t target_protcol_size,
                    uint16_t operation);

#endif