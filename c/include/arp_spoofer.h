#ifndef ARP_SPOOFER_H
#define ARP_SPOOFER_H
#include "ether.h"
#include "arp.h"


void arp_poisoning(uint8_t victim_protocol_address[], size_t victim_protocol_size,
                   uint8_t gateway_protocol_address[], size_t gateway_protocol_size,
                   uint8_t spoofer_protocol_address[], size_t spoofer_protocol_size,
                   uint8_t spoofer_hardware_address[], size_t spoofer_hardware_size,
                   int raw_socket_fd);

#endif