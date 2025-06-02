/*
On the data-link layer, lies the distinction between switched and unswitched networks.

(ROUTER)
Inside a router, there are several components: 
  - Router
  - Access Point
  - Switch      (to send packets to the correct device on the network)
  - Firewall
  - DNS Server
  - DHCP Server (for assigning IP addresses to devices on the network)
_

(SNIFFING)
  On an unswitched network, Ethernet packets pass through every device on the network, expecting 
  each system device to only look at the packets sent to its destination MAC address.
  However it's fairly trivial to set a device to 'Promiscuous mode', which allows it to read all 
  packets on the network, not just those addressed to it.

  Most packet-capturing programs, such as tcpdump, drop the device they are listening to into promiscuous mode
  by default. This can be set using ifconfig (es: ifconfig eth0 promisc)
  
  The act of capturing packets that aren't necessarily meant for public viewing is called 'sniffing'.
_

(RAW SOCKETS)
  Till now we used stream sockets to communicate over the network. It is possible to acces the network
  lower layers using raw sockets, specified by using SOCK_RAW as the type.
  At this lower layer, all the detail are exposed and must be handled explocitly by the programmer.
  In this case, the protocol matters since there are multiple options, such as IPPROTO_TCP, IPPROTO_UDP, IPPROTO_ICMP.
*/


// Raw TCPsniff:
// This program opens a raw TCP socket and listens for 3 packets, printing the raw data of each one with dump().
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include "hacking.h"

int main(void){
    int i, recv_lenght, sockfd; 
    u_char buffer[9000]; // Buffer to hold the received packet data (u_char = unsigned char)

    if((sockfd = socket(PF_INET, SOCK_RAW, IPPROTO_TCP)) == -1) // PF_INET = IPv4, SOCK_RAW = raw socket, IPPROTO_TCP = TCP protocol
        fatal("in socket");
    
    for(i=0; i < 3; i++){
        recv_lenght = recv(sockfd, buffer, 8000, 0); // buffer stores, 8000 is max size, 0 means no flags
        printf("Got a %d byte packet\n", recv_lenght);
        dump(buffer, recv_lenght); // dump prints the raw data of the packet reading for a lenght inside buffer
    }
}

// (this one dont' work on MAC OS, because of the security restrictions on raw sockets)
// Another thing is that Raw socket code is inconsistent across different operating systems, 
// making it nearly impossible to write portable code that works on all systems.

