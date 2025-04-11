# ARP Spoofing

## What is ARP? 🖥️🌐
ARP (Address Resolution Protocol) is a protocol in computer networks which is typically used to retrieve the 2nd layer address (typically MAC) of a host using a 3rd layer address (typically IP).
In simpler terms, ARP resolves the mapping from an IP address (network layer) to a MAC address (data link layer).

###
What is an ARP Table? 📋

An ARP table is where a device stores 3rd layer address-to-2nd layer address mappings. When a device resolves an address via ARP, it saves that mapping in the ARP table for faster future lookups. Entries in the table have a timeout and get refreshed to avoid outdated info.

### How does it work?

#### Step 1: The Request 💬
* Firstly, our source host will have an IP of a host it wants to find its MAC, then it will send a request in its network the following way:
  1. Includes the sender’s IP and MAC addresses.
  2. Sets the receiver’s MAC address as FF:FF:FF:FF:FF:FF (a broadcast address, meaning "everyone listen").
  3. The IP address we have of the receiver.

#### Step 2: Network Reaction ⚙️
* All computers in the network will receive that request, and look at the receiver IP, see that it's not theirs and will discard it. Except for our destined receiver, which will look at the receiver IP in the packet, and will see that it's his.

#### Step 3: The Reply ↩️
* Now, our receiver will craft an ARP response back to the sender. 
* Inside that packet, our host will add its own ip and mac, and the receiver's (which originally was the sender) IP address and MAC.

#### Step 4: Updating ARP Tables 🔗
* If the target didn’t already have the sender’s info, it adds the sender’s IP-to-MAC mapping to its ARP table for future use.
* The sender updates its own ARP table with the target’s IP-to-MAC mapping upon receiving the response.
* Example table:
<img src="https://support.pelco.com/servlet/rtaImage?eid=ka34y000000LKtc&feoid=00N4y000003WSIl&refid=0EM4y0000044Ozz" alt="Example ARP Table" width="600"/>

#### Step 5: Success! 👏
* Our sender host has now mapped the target’s IP address to its MAC address, achieving the goal of ARP!

#### ARP Visualized 📺
<img src="https://www.practicalnetworking.net/wp-content/uploads/2017/01/traditional-arp-process.gif" alt="Example ARP GIF" width="600"/>

**Disclaimer ⚠**: 
* The protocol does not have to use MAC and IP, I chose using these implementation of the 2nd and 3rd addresses for simplicity’s sake, and for easier explanation.
* The gif was made by [Practical Networking](https://www.practicalnetworking.net/), and I highly suggest you to read his lessons about ARP and networking in general.

## ARP Spoofing Basics

### What is a Man in the middle attack?
First, before understanding ARP spoofing we need to understand what is a Man in the middle attack (also known as MITM) attack.

### What is ARP Spoofing?
ARP Spoofing is a Man in the middle attack (MITM) cyberattack which uses the vulnerability in the ARP protocol in order to capture, read, and edit packets between two hosts on a local network.


## How to set up?
If you want the script to work better, run this command in your appropriate OS.
* Windows (run as admin)
  ```bash
  netsh interface ipv4 set global forwarding=enabled
  ```
* Linux
  ```bash
  sudo sysctl -w net.ipv4.ip_forward=1
  ```

**Disclaimer**: This project is for educational purposes only! Unauthorized use or attacks on networks are illegal and unethical.
