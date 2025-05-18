#include <stdio.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netpacket/packet.h>
#include <net/ethernet.h>
#include <string.h>
#include "ether.h"

int initialize_ethernet(ethernet_frame* frame, 
						uint8_t destination_address[], size_t destination_size, 
						uint8_t source_address[], size_t source_size, 
						uint16_t ethernet_type, uint8_t payload[], size_t payload_size) {
	uint8_t default_payload[MINIMUM_PAYLOAD_SIZE] = {0};
	if (destination_size != 6 || source_size != 6 || payload_size > 1500){
		return -1;
	}
	
	if(payload_size == 0){
		payload_size = MINIMUM_PAYLOAD_SIZE;
		payload = default_payload;
	}

	frame->payload_size = payload_size;
	memcpy(frame->ethernet_header.destination_address, destination_address, MAC_ADDRESS_SIZE);
	memcpy(frame->ethernet_header.source_address, source_address, MAC_ADDRESS_SIZE);
	frame->ethernet_header.ethernet_type = htons(ethernet_type);
	
	if (payload != NULL) {
		memcpy(frame->payload, payload, payload_size);
	}

	return 0;
}

int send_frame(ethernet_frame* frame) {
	int ret = 0;
	int raw_socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

	if (raw_socket_fd < 0) {
		ret -1;
		goto l_cleanup;
	}
	
	struct sockaddr_ll socket_address;
	memset(&socket_address, 0, sizeof(struct sockaddr_ll));

	socket_address.sll_family = AF_PACKET;
	socket_address.sll_protocol = htons(ETH_P_ALL);
	socket_address.sll_ifindex =  if_nametoindex("ens33");

	socket_address.sll_halen = ETH_ALEN;
	memcpy(socket_address.sll_addr, frame->ethernet_header.destination_address, MAC_ADDRESS_SIZE);

	int frame_size = sizeof(ether_header) + frame->payload_size;

	ssize_t bytes_sent = sendto(raw_socket_fd, frame, frame_size, 0, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_ll));

	if (bytes_sent < 0) {
		ret -1;
		goto l_cleanup;
	}
	ret = bytes_sent;

l_cleanup:
    if (raw_socket_fd >= 0) {
        close(raw_socket_fd);
    }
    return ret;
}