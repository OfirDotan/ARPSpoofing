#include <string.h> // For memcpy
#include <stddef.h> // For size_t
#include <stdlib.h> // For malloc
#include <arpa/inet.h> // For htons
#include <net/if.h> // For if_nametoindex
#include <netpacket/packet.h> // For sockaddr_ll
#include "common.h"
#include "arp.h"
#include "ether.h"

/*
Returns 0 on success, -1 on failure
*/
int initialize_arp(ethernet_frame* frame, 
                    uint8_t sender_hardware_address[], size_t sender_hardware_size,
                    uint8_t target_hardware_address[], size_t targetHardware_size,
                    uint8_t sender_protcol_address[], size_t sender_protcol_size,
                    uint8_t target_protcol_address[], size_t target_protcol_size,
                    uint16_t operation){
	if (sender_hardware_size != MAC_ADDRESS_SIZE || targetHardware_size != MAC_ADDRESS_SIZE) {
		return -1;
	}
	if (sender_protcol_size != IP_ADDRESS_SIZE || target_protcol_size != IP_ADDRESS_SIZE) {
		return -1;
	}


	//ARP header init
	arp_header frame_arp_header;
    frame_arp_header.hardware_type = htons(ETHERNET_CODE);
	frame_arp_header.protocol_type = htons(IPV4_CODE);
	frame_arp_header.hardware_length = MAC_ADDRESS_SIZE;
	frame_arp_header.protocol_length= IP_ADDRESS_SIZE;
	frame_arp_header.operation = htons(operation);
	
	memcpy(frame_arp_header.sender_hardware_address, sender_hardware_address, MAC_ADDRESS_SIZE);
	
	memcpy(frame_arp_header.target_hardware_address, target_hardware_address, MAC_ADDRESS_SIZE);

	memcpy(frame_arp_header.sender_protocol_address, sender_protcol_address, IP_ADDRESS_SIZE);
	memcpy(frame_arp_header.target_protocol_address, target_protcol_address, IP_ADDRESS_SIZE);

	initialize_ethernet(frame, target_hardware_address, MAC_ADDRESS_SIZE, sender_hardware_address, MAC_ADDRESS_SIZE, ARP_CODE, (uint8_t*) &frame_arp_header, sizeof(arp_header));

	return 0;
}


int get_mac_address(uint8_t sender_hardware_address[], size_t sender_hardware_size,
                    uint8_t sender_protcol_address[], size_t sender_protcol_size,
                    uint8_t target_protcol_address[], size_t target_protcol_size,
					uint8_t** received_hardware_address, int raw_socket_fd){

	int status = 0;


	ethernet_frame arp; 
	uint8_t broadcast_address[MAC_ADDRESS_SIZE] = BROADCAST_MAC_ADDRESS;
	initialize_arp(&arp, 
				   sender_hardware_address, MAC_ADDRESS_SIZE, 
				   broadcast_address, MAC_ADDRESS_SIZE, 
				   sender_protcol_address, IP_ADDRESS_SIZE, 
				   target_protcol_address, IP_ADDRESS_SIZE, 
				   ARP_REQUEST);

	int sent = send_frame(&arp, raw_socket_fd);
	if (sent < 0) {
		FAIL_AND_CLEANUP;
	}
	uint8_t buffer[ARP_BUFFER_SIZE];

	struct sockaddr_ll socket_address;
	memset(&socket_address, 0, sizeof(struct sockaddr_ll));
	socklen_t socket_address_len = sizeof(struct sockaddr_ll);

	socket_address.sll_family = AF_PACKET;
	socket_address.sll_ifindex = if_nametoindex("ens33");
	socket_address.sll_protocol = htons(ARP_CODE);

	ssize_t received = recvfrom(raw_socket_fd, buffer, ARP_BUFFER_SIZE, 0, (struct sockaddr*)&socket_address, &socket_address_len);

	printf("Received: %zd bytes\n", received);
	if(received < 0){
		FAIL_AND_CLEANUP;
	}

	arp_header* received_arp = (arp_header*) (((ethernet_frame*) buffer)->payload);

	if(received_arp == NULL || received_arp->sender_hardware_address == NULL){
		FAIL_AND_CLEANUP;
	}

	*received_hardware_address = malloc(MAC_ADDRESS_SIZE);
	memcpy(*received_hardware_address, received_arp->sender_hardware_address, MAC_ADDRESS_SIZE);

l_cleanup:
	if(status < 0) {
		received_hardware_address = NULL;
	}
	return status;
}