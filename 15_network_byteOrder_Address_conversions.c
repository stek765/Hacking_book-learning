/*

NETWORK BYTE ORDER: 

    The port number and IP address are stored in network byte order, which is big-endian.
    This means that the most significant byte is stored first, followed by the less significant bytes.

    this is the opposite of th x86 architecture, which uses little-endian byte order (least significant byte first).
    so these values must be converted. 

    To do so there are several functions available in the <arpa/inet.h> library:
    //     - htons()  → Host to Network Short (converts 16-bit port numbers)
    //     - htonl()  → Host to Network Long (converts 32-bit IP addresses)
    //     - ntohs()  → Network to Host Short (converts 16-bit port numbers back)
    //     - ntohl()  → Network to Host Long (converts 32-bit IP addresses back)

- - - - - - - - - - - - -

INTERNET ADDRESS CONVERSIONS:

    There are functions to convert to and from human-readable IP  addresses (ex: 12.110.110.204, which is 
    an ASCII string) and binary format, defined in <arpa/inet.h>:
    //     - inet_aton()  → Converts a string IP address to binary format (in_addr structure)
    //     - inet_ntoa()  → Converts a binary IP address back to a string format
*/