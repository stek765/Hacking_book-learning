/*
The packets sent over the network are structured in layers, each adding its own header to the data. 
Regardless of the packets contain, the protocols used at lower layers to move the data from 
point A to point B are the usually the same. 

-> So, once you understand th "office bureaucracy" of these common lower layer protocols, 
   you can peek inside envelopes in transit, and even falsify documents to manipulate the system.
__________________________________________________________________________________


DATA-LINK LAYER:

This layer is the lowest visible layer, and provides a way to address and send messages to anyone else
as well as to figure out who's in the same network.

ETHERNET and MAC:
  Ethernet exists on this layer, providing a standard addressing system for all Ethernet devices.
  These addresses are known as Media Acces Control (MAC) addresses, and are GLOBALLY unique to each device.
  Consisting of 6 bytes, they are usually represented as 12 hexadecimal digits (ex: 00:1A:2B:3C:4D:5E).
  This address is unique to any peace of hardware and is stored in the device's integrated circuit memory.
_
  An Ethernet header is 14 bytes in size, and contains the source and destination MAC addresses 
  for this Ethernet packet. 
_
  (BROADCAST ADDRESS)
  Ethernet addressing also provides a special broadcast address, consisting of all binary's 1 (ff:ff:ff:ff:ff:ff).
  Which means that the packet is sent to all devices on the network.
_
  (ARP - ADDRESS RESOLUTION PROTOCOL)
  Since MAC addresses are fixed to the hardware, while IP addresses can change, it is necessary 
  to have a way to map IP addresses to MAC addresses.
  in Ethernet, this method is known as Address Resolution Protocol (ARP).

  There are 4 different types of ARP messages, but the 2 most important are: 
     - ARP rquest messages (says: "Hello, who has this IP address? Please send me your MAC address") -> Broadcasted to all devices on the network.
     - ARP reply  messages (says: "I have this IP address, and my MAC address is this one") -> Sent directly to the requesting device.
  
  If the first host wants to establish a TCP connection over IP to the second device's IP address, 
  it will first check it's ARP cache to see if an entry exists for that IP address.
  If not, it will send an ARP request to the network, asking for the MAC address of the device with that IP.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -



NETWORK LAYER:

  This layer is responsible for routing packets across networks, and is where IP addresses come into play.
  The protocol used at this layer is appropriately called the Internet Protocol (IP). (IPv4 or IPv6)

  Every system on the Internet has a unique IP address, consisting of a 4 byte number (IPv4) or a 16 byte number (IPv6)
  in the form of xx.xx.xx.xx . 
  The IP header for packets is 20 bytes in size and is standardized with the RFC 791.

FROM RFC 791, an IP header format:

        Byte #:   0       1         2       3
                +-------+-------+---------------+---------------+
                |Vers/IHL|  TOS  |         Total Length          |
                +-------+-------+---------------+---------------+

        Byte #:   4       5         6       7
                +---------------+-------+-----------------------+
                | Identification |Flags  | Fragment Offset       |
                +---------------+-------+-----------------------+

        Byte #:   8       9         10      11
                +-------+-------+---------------+---------------+
                |  TTL  | Proto |   Header Checksum             |
                +-------+-------+---------------+---------------+

        Byte #:  12      13      14      15
                +--------------- Source Address ----------------+
                |              (es. 192.168.1.1)                |
                +-----------------------------------------------+

        Byte #:  16      17      18      19
                +----------- Destination Address ---------------+
                |              (es. 8.8.8.8)                    |
                +-----------------------------------------------+

    // Options and Padding are not included in this example, as they are rarely used.

  the IP header also has a protocol field to describe the type of data being sent in the packet and 
  the source and destination addresses for routing.
  In addition, the header carries a checksum, to help detect transmissions error, and 
  fields to deal wth packet fragmentation. 
_

(ICMP - INTERNET CONTROL MESSAGE PROTOCOL)
  IP is not a reliable protocol, so it does not guarantee that packets will arrive at their destination.
  If there's a problem, an ICMP packet is sent back to the sender to notify them of the issue.

  ICMP is also commonly used for network diagnostics (es: ping command, which uses ICMP echo request and reply messages).
_
  IP and ICMP are both connectionless. All they care about is getting the packets to their destination address.
  (IP can deal wth limitation on packet size by fregmentign packets into smaller pieces,
  and reassembling them at the destination).
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -



TRANSPORT LAYER: 
  
  The transport layer is responsible for reliable data trasmission between hosts.
  The 2 major protocols at this layer are TCP (Transmission Control Protocol) and UDP (User Datagram Protocol).
_
(TCP - TRANSMISSION CONTROL PROTOCOL)
  TCP is the most commonly used protocol for services on the Internet: telnet, HTTP, SMPT(email), FTP(file transfer), etc.
  Provides a transparent, yet reliable and bidirectional, connection between 2 IP addresses.
  - Bidirectional = both hosts can send and receive data at the same time.
  - Reliable = TCP guarantees that all data sent will arrive at the destination, and in the same order it was sent.
               if a packet is lost, the destination will hold on to the packets it has while the source resends the lost packet.
  
  All of this functionality is made possible by a set of flags, called TCP flags, and a sequence number.
  TCP FLAGS:
    +------+----------------+----------------------------------------------------------+
    | Flag |     Meaning    |                        Purpose                          |
    +------+----------------+----------------------------------------------------------+
    | URG  | Urgent         | Indicates that the data should be processed immediately |
    |      |                | by the receiving application.                           |
    | ACK  | Acknowledgment | Confirms receipt of data and indicates the next expected|
    |      |                | byte from the sender.                                   |
    | PSH  | Push           | Instructs the receiver to pass the data to the          |
    |      |                | application without buffering.                          |
    | RST  | Reset          | Abruptly terminates a connection due to errors or       |
    |      |                | unexpected conditions.                                  |
    | SYN  | Synchronize    | Initiates a connection by synchronizing sequence numbers|
    |      |                | between hosts.                                          |
    | FIN  | Finish         | Gracefully terminates a connection when data transfer is|
    |      |                | complete.                                               |
    +------+----------------+----------------------------------------------------------+
_ 

(TCP HEADER FROM FRC 793)
  
        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |        Source Port            |     Destination Port          |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |                     Sequence Number                            |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |                  Acknowledgment Number                         |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        | Data |Rese-|  |U|A|P|R|S|F|    |                               |
        |Offset|rved |NS|R|C|S|S|Y|I|    |    Window Size                |
        |      |     |  |G|K|H|T|N|N|    |                               |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |     Checksum                  |   Urgent Pointer               |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |         Options (if any, variable length)                      |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

  The sequence number and acknowledgment number are used to maintain state.
  The SYN and ACK flags are used together to open connections in a three-step handshaking process.

  (THREE-WAY HANDSHAKE)
            Client                               Server
            |                                     |
            | ----------- SYN ------------------> |
            |           seq = X                   |
            |           ack = 0                   |
            |                                     |
            | <-------- SYN + ACK --------------  |
            |           seq = Y                   |
            |           ack = X + 1               |
            |                                     |
            | ----------- ACK ------------------> |
            |           seq = X + 1               |
            |           ack = Y + 1               |
            |                                     |
            |        Connection Established       |

  Syn is used only in this initial part.

  Sequence number (seq) allow TCP to put unordered packets back in order, to prevent wheter packets
  are missing and to prevent mixing up packets from other connections.
_

(UDP - USER DATAGRAM PROTOCOL)
  It's faster than TCP, but does not guarantee that packets will arrive at their destination.
  It is used for applications that can tolerate some data loss, such as video streaming or online gaming.
  UDP is connectionless, meaning that it does not establish a connection before sending data.
  
  It's similar to IP protocol, but it adds a very important information: the source and destination ports.
  The UDP header is 8 bytes in size, and consists of the following fields:

        0                   1                   2                   3
        0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |        Source Port            |     Destination Port          |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |             Length            |          Checksum             |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
        |                       Data (variable length)                  |
        +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

    The source and destination ports are used to identify the application that is sending or receiving the data.
    The length field indicates the length of the UDP header and data, and the checksum field is used to detect errors.
  
 */