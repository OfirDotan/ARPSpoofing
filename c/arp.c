#include "arp.h"
#include <string.h>

int initialize_arp(arp_packet* packet, uint8_t senderHardwareAddress[], uint8_t targetHardwareAddress[], uint8_t senderProtocolAddress[], uint8_t targetProtocolAddress[], uint16_t operation) {
	
	if (sizeof(senderHardwareAddress) != 6 || sizeof(targetHardwareAddress) != 6) {
		return -1;
	}
	if (sizeof(senderProtocolAddress) != 4 || sizeof(targetProtocolAddress) != 4) {
		return -1;
	}

	initialize_ethernet(&(packet->frame), targetHardwareAddress, senderHardwareAddress, ETHERNET_CODE, NULL);

	//ARP header init
	packet->arpHeader.hardwareType = ETHERNET_CODE;
	packet->arpHeader.protocolType = IPV4_CODE;
	packet->arpHeader.hardwareLength = MAC_ADDRESS_SIZE;
	packet->arpHeader.protocolLength= IP_ADDRESS_SIZE;
	packet->arpHeader.operation = operation;
	
	memcpy(packet->arpHeader.senderHardwareAddress, senderHardwareAddress, MAC_ADDRESS_SIZE);
	if (operation == ARP_REQUEST) {
		memcpy(packet->arpHeader.targetHardwareAddress, 0, MAC_ADDRESS_SIZE);
	}
	else {
		memcpy(packet->arpHeader.targetHardwareAddress, targetHardwareAddress, MAC_ADDRESS_SIZE);
	}

	memcpy(packet->arpHeader.senderProtocolAddress, senderProtocolAddress, IP_ADDRESS_SIZE);
	memcpy(packet->arpHeader.targetProtocolAddress, targetProtocolAddress, IP_ADDRESS_SIZE);

	return 0;
}