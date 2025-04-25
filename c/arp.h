#ifndef ARP_H
#define ARP_H

#include <stdint.h>
#include "ether.h"

const uint16_t ARP_REQUEST = 1;
const uint16_t ARP_RESPONSE = 2;
const uint16_t ETHERNET_CODE = 1;
const uint16_t IPV4_CODE = 0x0800;
const int MAC_ADDRESS_SIZE = 6;
const int IP_ADDRESS_SIZE = 4;

typedef struct arp_header {
	uint16_t hardwareType;
	uint16_t protocolType;
	uint8_t hardwareLength;
	uint8_t protocolLength;
	uint16_t operation; // 1 - Request, 2 - Reply
	
	uint8_t senderHardwareAddress[MAC_ADDRESS_SIZE]; // Sender's MAC address
	uint8_t targetHardwareAddress[MAC_ADDRESS_SIZE]; // Target's MAC address
	
		uint8_t senderProtocolAddress[IP_ADRESS_SIZE]; //Sender's IP address
	uint8_t targetProtocolAddress[IP_ADRESS_SIZE]; // Target's IP address
} arp_header;


typedef struct arp_packet {
	ethernet_frame frame;
	arp_header arpHeader;
} arp_packet;

int initialize_arp(arp_packet* packet, uint8_t senderHardwareAddress[], uint8_t targetHardwareAddress[], uint8_t senderProtocolAddress[], uint8_t targetProtocolAddress[], uint16_t operation);

#endif ARP_H