#include "arp.h"
#include <string.h>

int initialize_arp(arp_packet* packet, uint8_t senderHardwareAddress[], uint8_t targetHardwareAddress[], uint8_t senderProtocolAddress[], uint8_t targetProtocolAddress[], uint16_t operation) {
	
	if (sizeof(senderHardwareAddress) != 6 || sizeof(targetHardwareAddress) != 6) {
		return -1;
	}
	if (sizeof(senderProtocolAddress) != 4 || sizeof(targetProtocolAddress) != 4) {
		return -1;
	}

	//Ethernet header init
	memcpy(packet->frame.ethernetHeader.sourceAddress, senderHardwareAddress, 6);
	memcpy(packet->frame.ethernetHeader.destinationAddress, targetHardwareAddress, 6);
	packet->frame.ethernetHeader.ethernetType = 0x0806;

	//ARP header init
	packet->arpHeader.hardwareType = ETHERNET_CODE;
	packet->arpHeader.protocolType = IPV4_CODE;
	packet->arpHeader.hardwareLength = 6;
	packet->arpHeader.protocolLength= 4;
	packet->arpHeader.operation = operation;
	
	memcpy(packet->arpHeader.senderHardwareAddress, senderHardwareAddress, 6);
	if (operation == ARP_REQUEST) {
		memcpy(packet->arpHeader.targetHardwareAddress, 0, 6);
	}
	else {
		memcpy(packet->arpHeader.targetHardwareAddress, targetHardwareAddress, 6);
	}

	memcpy(packet->arpHeader.senderProtocolAddress, senderProtocolAddress, 4);
	memcpy(packet->arpHeader.targetProtocolAddress, targetProtocolAddress, 4);

	return 0;
}