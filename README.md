# ARP Spoofing

## Important To Note
* The C implementation here is not complete and still needs rework. I need to complete the actual spoofing, as currently all there is here is just the tools to use it (AKA building the packets and sending it using RAW sockets).
* Sometime in the future, I want to complete this project and rework my C code to be prettier and more formal.
* I want to replace the bash script I created with a proper CMake file. 

## What is ARP? 🖥️🌐
ARP (Address Resolution Protocol) is a protocol in computer networks that is typically used to retrieve the 2nd layer address (typically MAC) of a host using a 3rd layer address (typically IP).
In simpler terms, ARP resolves the mapping from an IP address (network layer) to a MAC address (data link layer).

### What is an ARP Table? 📋

An ARP table is where a device stores 3rd layer address-to-2nd layer address mappings. When a device resolves an address via ARP, it saves that mapping in the ARP table for faster future lookups. Entries in the table have a timeout and get refreshed to avoid outdated information.

### How does it work?

#### Step 1: The Request 💬
* Firstly, our source host will have an IP address of a host it wants to find its MAC, then it will send a request in its network the following way:
  1. Includes the sender’s IP and MAC addresses.
  2. Sets the receiver’s MAC address as FF:FF:FF:FF:FF:FF (a broadcast address, meaning "everyone listen").
  3. The IP address we have of the receiver.

#### Step 2: Network Reaction ⚙️
* All computers in the network will receive that request, and look at the receiver IP, see that it's not theirs, and will discard it. Except for our destined receiver, which will look at the receiver IP in the packet, and will see that it's his.

#### Step 3: The Reply ↩️
* Now, our receiver will craft an ARP response back to the sender. 
* Inside that packet, our host will add its own IP and MAC, and the receiver's (which originally was the sender) IP address and MAC.

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
* The protocol does not have to use MAC and IP., I chose to use these implementations of the 2nd and 3rd addresses for simplicity’s sake, and for easier explanation.
* The GIF was made by [Practical Networking](https://www.practicalnetworking.net/), and I highly suggest you read his lessons about ARP and networking in general.

## ARP Spoofing Basics

### What is a Man in the Middle attack?
First, before understanding ARP spoofing, we need to understand what a Man in the Middle (also known as MITM) attack is. A MITM is a type of cyberattack where the attacker injects himself in the middle of the communication between 2 hosts. In this type of attack, the two hosts think that they are directly communicating with one another.


#### Read World Analogy
To better explain a MITM attack, let's use a real world analogy:
> Imagine that you and your friend were passing notes to each other in class. To do that, you write your note to another person and ask them to pass it along. You expect that your note will reach your friend unopened and safely. So in a MITM attack, the person who inserted himself in as the "note passer", secretly opens the note, reads and sometimes even changes it before passing it along to your friend.
In the digital world, this is what happens during a MITM attack. Instead of physical notes, the attacker intercepts and manipulates packets and frames as they travel between two devices, all while making both parties believe they are communicating directly and securely.

### What is ARP Spoofing?
ARP Spoofing is a Man in the Middle attack (MITM) cyberattack that uses the vulnerability in the ARP protocol in order to capture, read, and edit packets between two hosts on a local network.

So basically, take the note-passing example from earlier and apply it to the real world, but this time using the weaknesses of the ARP protocol. The main problem with ARP is that anyone can send fake ARP replies and pretend they have a different IP address than they really do. By doing this, the attacker tricks both sides into registering false information, making them think they’re communicating directly with each other.

Just like the fake note passer who secretly reads or changes the message before passing it on, the attacker sits in the middle of the communication between two devices. This is how a MITM attack happens using ARP spoofing.

<img src="https://i.postimg.cc/kX1ZB8kG/image.png">
A very high-level example of what ARP spoofing might look like in the real world.

## Important to note
This project does not rely on IP forwarding and will work better if the IP forwarding is off. If 

THIS PROJECT WAS MADE USING 2 DIFFERENT IMPLEMENTATIONS:

Python - A higher level script using scapy. Works on both Windows and Linux.
C - A low level implementation using raw sockets. Works on Linux. WIP AND NOT COMPLETE.

How to disable IP forwarding?
* Windows (run as admin) **ONLY FOR THE PYTHON SCRIPT
  ```bash
  netsh interface ipv4 set global forwarding=disabled
  ```
* Linux
  ```bash
  sudo sysctl -w net.ipv4.ip_forward=0
  ```

## **Disclaimer**
This project is intended for educational purposes only. It was created as a way for me to apply networking concepts I’ve learned in a practical setting and to further explore my personal interest in computer networking, network security, and techniques such as man-in-the-middle (MITM) attacks.
