#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h> // For socket functions
#include <arpa/inet.h> // For htons
#include <unistd.h> // For close

#include "ether.h"
#include "arp.h"

int main() {
	// Temp basic test to see if the ethernet and arp were constructed correctly.

	int raw_socket_fd = socket(AF_PACKET, SOCK_RAW, htons(ALL_PROTOCOLS));

	// uint8_t source_mac_address[MAC_ADDRESS_SIZE] = {0x00, 0x0C, 0x29, 0xF4, 0x92, 0xA0};
	// uint8_t dest_mac_address[MAC_ADDRESS_SIZE] = {0x00, 0x0C, 0x29, 0xF4, 0x92, 0xA0};

	// ethernet_frame arp; 
	// uint8_t source_ip_address[IP_ADDRESS_SIZE] =  {192, 168, 1, 29};
	// uint8_t dest_ip_address[IP_ADDRESS_SIZE] = {192, 168, 159, 128};
	// initialize_arp(&arp, source_mac_address, 6, dest_mac_address, 6, source_ip_address, 4, dest_ip_address, 4, ARP_RESPONSE);

	// int result = send_frame(&arp, raw_socket_fd);
	// printf("Status: %d\n", result);

	uint8_t* target_hardware;

	int res = get_mac_address(
		(uint8_t[]){0x00, 0x0C, 0x29, 0xF4, 0x92, 0xA0}, MAC_ADDRESS_SIZE,
		(uint8_t[]){10, 0, 0, 79}, IP_ADDRESS_SIZE,
		(uint8_t[]){10, 0, 0, 16}, IP_ADDRESS_SIZE,
		&target_hardware, raw_socket_fd
	);
	printf("Result: %d\n", res);

	printf("Sender Hardware Address: ");
	if(target_hardware != NULL) {
		for(int i = 0; i < MAC_ADDRESS_SIZE; i++) {
			if(i + 1 != MAC_ADDRESS_SIZE) {
				printf("%02X:", target_hardware[i]);
			} else {
				printf("%02X", target_hardware[i]);
			}
		}
		printf("\n");
	}

	free(target_hardware);
	close(raw_socket_fd);
}