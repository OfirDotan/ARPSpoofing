#include <sys/socket.h> // For socket functions
#include <arpa/inet.h> // For htons
#include <net/if.h> // For if_nametoindex
#include <netpacket/packet.h> // For sockaddr_ll
#include <string.h> //For memcpy
#include "common.h"
#include "ether.h"

int initialize_ethernet(ethernet_frame* frame, 
						uint8_t destination_address[], size_t destination_size, 
						uint8_t source_address[], size_t source_size, 
						uint16_t ethernet_type, uint8_t payload[], size_t payload_size) {

	if (destination_size != 6 || source_size != 6 || payload_size > 1500){
		return -1;
	}

	uint8_t default_payload[MINIMUM_PAYLOAD_SIZE] = {0};	
	if(payload_size < MINIMUM_PAYLOAD_SIZE || payload == NULL){
		frame->payload_size = MINIMUM_PAYLOAD_SIZE;
		memcpy(frame->payload, default_payload, MINIMUM_PAYLOAD_SIZE);
		if (payload != NULL) {
			memcpy(frame->payload, payload, payload_size);
		}
	}
	else {
		frame->payload_size = payload_size;
		memcpy(frame->payload, payload, payload_size);
	}

	memcpy(frame->ethernet_header.destination_address, destination_address, MAC_ADDRESS_SIZE);
	memcpy(frame->ethernet_header.source_address, source_address, MAC_ADDRESS_SIZE);
	frame->ethernet_header.ethernet_type = htons(ethernet_type);

	return 0;
}

int send_frame(ethernet_frame* frame, int raw_socket_fd) {
	int status = 0;

	if (raw_socket_fd < 0) {
		FAIL_AND_CLEANUP;
	}
	
	struct sockaddr_ll socket_address;
	memset(&socket_address, 0, sizeof(struct sockaddr_ll));

	socket_address.sll_family = AF_PACKET;
	socket_address.sll_protocol = htons(ALL_PROTOCOLS);
	socket_address.sll_ifindex =  if_nametoindex("ens33");

	socket_address.sll_halen = MAC_ADDRESS_SIZE;
	memcpy(socket_address.sll_addr, frame->ethernet_header.destination_address, MAC_ADDRESS_SIZE);

	int frame_size = sizeof(ether_header) + frame->payload_size;

	ssize_t bytes_sent = sendto(raw_socket_fd, frame, frame_size, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));

	if (bytes_sent < 0) {
		FAIL_AND_CLEANUP;
	}
	status = bytes_sent;

l_cleanup:
    return status;
}