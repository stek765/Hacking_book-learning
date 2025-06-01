/*
SOCKETS:
 A socket is a standard way to communicate over a network, using the OSI model. (5th layer, session layer)
 It's abstracted by the operating system, and there can be many different sockets open at the same time.

 Our computer ALWAYS RECEIVES DATA, but each packet gets distributed into a specific socket,
 like distributing traffic in different lanes.

---


SOCKET PACKET ENCAPSULATION:
╔════════════════════════════════════════════════════════╗
║ APPLICATION LAYER                                      ║ ← what you write (e.g. "GET /index.html")
╚════════════════════════════════════════════════════════╝
                                ↓
╔════════════════════════════════════════════════════════╗
║ TRANSPORT LAYER (TCP/UDP)                              ║ ← adds TCP header (port, ack, seq, etc.)
║ ▸ TCP header: 20 bytes                                 ║ 
╚════════════════════════════════════════════════════════╝
                                ↓
╔════════════════════════════════════════════════════════╗
║ NETWORK LAYER (IP)                                     ║ ← adds IP header (source, destination)
║ ▸ IP header: 20 bytes                                  ║
╚════════════════════════════════════════════════════════╝
                                ↓
╔════════════════════════════════════════════════════════╗
║ DATA LINK LAYER (Ethernet, Wi-Fi)                      ║ ← MAC addresses, CRC, frame handling
║ ▸ Ethernet header: 14 bytes + trailer: 4 bytes         ║
╚════════════════════════════════════════════════════════╝
                                ↓
╔════════════════════════════════════════════════════════╗
║ PHYSICAL LAYER (cable, air, fiber)                     ║ ← transmits 0s and 1s physically
╚════════════════════════════════════════════════════════╝

_______________________________________________________________________


SOCKET INTERACTION EXAMPLE:

When you call send() from your socket:    
-> send(sockfd, "Hello", 5, 0);

⚙️ The operating system does the following:
	1.	Adds a TCP header: source/destination ports, sequence number, etc.
	2.	Adds an IP header: source/destination IP, TTL, checksum, etc.
	3.	Adds Ethernet/Wi-Fi header: MAC addresses
	4.	Sends the bits out physically via wire or air

  When you call recv():       
-> char buf[128];  
-> recv(sockfd, buf, sizeof(buf), 0); 

	1.	The kernel receives the physical packet
	2.	It strips the Ethernet, then IP, then TCP headers
	3.	You only receive the payload (buf = "Hello")


How the actual packet looks like:
    ╔══════════════════════════════════════════════════════╗
    ║ Ethernet Header (14B)                                ║
    ╠══════════════════════════════════════════════════════╣
    ║ IP Header (20B)                                      ║
    ╠══════════════════════════════════════════════════════╣
    ║ TCP Header (20B)                                     ║
    ╠══════════════════════════════════════════════════════╣
    ║ Payload: "Hello"                                     ║
    ╚══════════════════════════════════════════════════════╝

_______________________________________________________________________
*/

// SOCKET CODE EXAMPLE:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>  // for close()

int main(int argc, char const *argv[])
{
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);  // 1. create socket
    struct sockaddr_in addr;

    addr.sin_family = AF_INET;              // 2. specify address family
    addr.sin_port = htons(8080);            // 3. set the port (8080 in this case, converted to network byte order)
    addr.sin_addr.s_addr = INADDR_ANY;      // 4. accept connections on any IP address

    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr)); // 5. link socket to address
    listen(sockfd, SOMAXCONN);   // 6. start listening for connections (SOMAXCONN is a constant for maximum pending connections, usually 128)

    int clientfd = accept(sockfd, NULL, NULL);  // 7. accept an incoming connection (blocks until a client connects)
    char buffer[128];       
    recv(clientfd, buffer, sizeof(buffer), 0);  // 8. recieve data from the client
   
    close(sockfd);  // 9. close the socket
    return 0;
}


/*
╔════════════════════════════════════════════╗
║            struct sockaddr_in              ║
╠══════════════════╦═════════════════════════╣
║ sin_family       ║ AF_INET       (2)       ║ ← AF_INET -> IPv4
║ sin_port         ║ htons(8080)   (0x1F90)  ║ ← porta in big-endian!! -> byte più significativo prima
║ sin_addr.s_addr  ║ inet_addr("127.0.0.1")  ║ ← IP in binario (0x7F000001)
╚══════════════════╩═════════════════════════╝
_______________________________________________________________________



SOCKET GRAPHICAL REPRESENTATION:

    +---------------------------------------+
    |            socket()                   |  // returns a file descriptor
    |---------------------------------------|
    | int sockfd = socket(AF_INET,          |  // AF_INET → IPv4, SOCK_STREAM → TCP, 0 -> first protocol available of that family (std tcp)
    |                    SOCK_STREAM, 0);   |
    +------------------+--------------------+
                       ↓
    +------------------+--------------------+
    |                bind()                 |  // associates the socket with an IP and port
    |---------------------------------------|
    | sockaddr_in addr;                     |  
    | addr.sin_family = AF_INET;            |  // AF_INET → IPv4
    | addr.sin_port = htons(8080);          |  // htons() converts port to network byte order
    | addr.sin_addr.s_addr = INADDR_ANY;    |  // INADDR_ANY binds to all available interfaces
    |                                       |  
    | bind(sockfd, (struct sockaddr*)&addr, |
    |      sizeof(addr));                   |
    +------------------+--------------------+
                       ↓ 
    +------------------+--------------------+
    |             listen()                  |   // makes the socket passive, ready to accept connections
    |---------------------------------------|
    | listen(sockfd, SOMAXCONN);            |   // SOMAXCONN is a constant for maximum pending connections, so basically it allows the server to queue up to 128 connections
    +------------------+--------------------+
                        ↓
    +------------------+--------------------+
    |             accept()                  |   // blocks and waits for an ,incoming connection
    |---------------------------------------|
    | int client_fd = accept(sockfd, NULL,  |   // NULL means we don't care about the client's address
    |                           NULL);      |
    +------------------+--------------------+
                        ↓
    +------------------+--------------------+
    |           recv() / send()             |   // used to receive and send data over the socket
    |---------------------------------------|
    | recv(client_fd, buf, size, 0);        |   // buf is a buffer to store received data, size is the maximum number of bytes to read, 0 means default flags
    | send(client_fd, buf, size, 0);        |   // buf is a buffer containing data to send, size is the number of bytes to send, 0 means default flags
    +---------------------------------------+
_______________________________________________________________________








- - - - - - - - - - - - 
SOME THEORETICAL NOTES:
- - - - - - - - - - - - 

SOCKET FUNCTIONS (in <sys/socket.h>):
 - socket()      → create a new socket
 - bind()        → associate the socket with an IP and port
 - listen()      → make the socket passive (ready to accept connections)
 - accept()      → accept an incoming connection (server-side)
 - connect()     → initiate a connection (client-side)
 - send(), recv()→ send or receive data
 - sendto(), recvfrom() → used in connectionless sockets (like UDP)
 - shutdown(), close()  → gracefully close a socket

---

PROTOCOL FAMILIES:
 Used to specify the address scheme the socket will use (usually passed as first param in socket()).

 - PF_INET      → IPv4
 - PF_INET6     → IPv6
 - PF_UNIX      → Local (file-based) sockets
 - PF_PACKET    → Low-level packet access (Linux only)
 - PF_NETLINK   → Kernel-user communication

---

TYPES OF SOCKETS:
 Used to define the semantics of communication (second param in socket()).

 - SOCK_STREAM   → Connection-oriented, reliable (like TCP)
 - SOCK_DGRAM    → Connectionless, unreliable (like UDP)
 - SOCK_RAW      → Direct access to IP packets (used in tools like ping)
 - SOCK_SEQPACKET→ Reliable, connection-based with message boundaries preserved

---

MACRO FOR sockaddr_common:
 All sockaddr structures start with these two fields (for compatibility):

    sa_family_t sa_family; // address family (AF_INET, etc.)
    char        sa_data[]; // protocol-specific address

Most structs (like sockaddr_in) embed this as their first fields.

---

ADDRESS FAMILIES (used in code as AF_*, passed to socket()):

 - AF_INET       → IPv4 Internet protocols
 - AF_INET6      → IPv6 Internet protocols
 - AF_UNIX / AF_LOCAL → Local IPC via file system
 - AF_PACKET     → Raw packet interface (Linux)
 - AF_NETLINK    → Kernel interface (Linux)
 - AF_BLUETOOTH  → Bluetooth sockets

Note: PF_* and AF_* are often the same integer values, but semantically:
 - AF = "Address Family" (used in sockaddr structures)
 - PF = "Protocol Family" (used in socket())

They are interchangeable in most practical code.

*/
     
    

