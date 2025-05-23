#include <stdio.h>
#include <sys/socket.h> // For socket functions
#include <arpa/inet.h> // For htons

#include "ether.h"
#include "arp.h"

int main() {
	//Test frame sending

	ethernet_frame frame;
	uint8_t source_mac_address[MAC_ADDRESS_SIZE] = {0x00, 0x0C, 0x29, 0xF4, 0x92, 0xA0};
	uint8_t dest_mac_address[MAC_ADDRESS_SIZE] = {0x00, 0x0C, 0x29, 0xF4, 0x92, 0xA0};
    uint8_t payload[46] = {0xAB};

	initialize_ethernet(&frame, dest_mac_address, 6, source_mac_address, 6, 0x88B5, payload, 46);
	

	ethernet_frame arp; 
	uint8_t source_ip_address[IP_ADDRESS_SIZE] =  {192, 168, 1, 29};
	uint8_t dest_ip_address[IP_ADDRESS_SIZE] = {192, 168, 159, 128};
	initialize_arp(&arp, source_mac_address, 6, dest_mac_address, 6, source_ip_address, 4, dest_ip_address, 4, ARP_RESPONSE);

	//int result = send_frame(&frame);
	int raw_socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ALL_PROTOCOLS));
	int result = send_frame(&arp, raw_socket_fd);
	printf("Status: %d\n", result);
}