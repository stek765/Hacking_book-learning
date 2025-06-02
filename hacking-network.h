#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h> 


/* This function accpets a socket FD and a ptr to the null terminted string to send.
 * The function will make sure all the bytes of the string are sent.
 * Returns 1 on success and 0 on failure.
*/
int send_string(int sockfd, unsigned char *buffer) {
    int bytes_to_send, sent_bytes;
    bytes_to_send = strlen((char *)buffer);  // total bytes to send

    while (bytes_to_send > 0) {
        sent_bytes = send(sockfd, buffer, bytes_to_send, 0);
        if (sent_bytes == -1)
            return 0; // Send error

        bytes_to_send -= sent_bytes;
        buffer += sent_bytes;
    }

    return 1; // Success
}

/* This function accpets a socket FD and a ptr to a destination buffer.
 * It will receive from the socket untile the EOL byte sequence in seen.
 * The EOL bytes are read from the socket, but the destination buffer is terminated beffore these bytes.
 * Returns the size of the read line (without the EOL bytes).
*/
int recv_line(int sockfd, unsigned char *dest_buffer) {
#define EOL "\r\n"         // End-of-line byte sequence
#define EOL_SIZE 2         // Size of the EOL string

    unsigned char *ptr;
    int eol_matched = 0;
    ptr = dest_buffer; 

    while (recv(sockfd, ptr, 1, 0) == 1) {  // Read one byte at a time and save it in ptr
        if (*ptr == EOL[eol_matched]) {    // If current byte matches expected EOL byte
            eol_matched++;
            if (eol_matched == EOL_SIZE) { // If entire EOL found
                *(ptr + 1 - EOL_SIZE) = '\0';  // Terminate string *before* EOL (es: "hello\0\r\n")
                return strlen((char *)dest_buffer);  // Return length
            }
        } 
        else {
            eol_matched = 0; // Reset match if sequence breaks
        }
        ptr++;  // Move to next byte in buffer
    }

    return 0; // End-of-line not found or recv failed
}

// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 

#define ETHER_ADDR_LEN 6  // Length of Ethernet address
#define ETH_HDR_LEN 14    // Length of Ethernet header

// Ethernet header structure
struct ether_hdr {
    unsigned char ether_dest_addr[ETHER_ADDR_LEN];  // Destination MAC address
    unsigned char ether_src_addr[ETHER_ADDR_LEN];   // Source MAC address
    unsigned short ether_type;                       // Protocol type (2 bytes)
};


// IP header structure
struct ip_hdr {
    unsigned char ip_version_and_header_length;  // Version and header length
    unsigned char ip_tos;                        // Type of service
    unsigned short ip_len;                       // Total length of the IP packet
    unsigned short ip_id;                        // Identification
    unsigned short ip_frag_offset;               // Fragment offset
    unsigned char ip_ttl;                        // Time to live
    unsigned char ip_type;                       // Protocol type
    unsigned short ip_checksum;                 // Header checksum
    unsigned int ip_src_addr;                   // Source IP address
    unsigned int ip_dest_addr;                  // Destination IP address
};


// TCP header structure
struct tcp_hdr {
    unsigned short tcp_src_port;  // Source port
    unsigned short tcp_dest_port; // Destination port
    unsigned int tcp_seq;         // Sequence number
    unsigned int tcp_ack;         // Acknowledgment number
    unsigned char reserved:4;     // 4 bits from the 6 bits of reserved space
    unsigned char tcp_offset:4;   // Data offset for little-endian host
    unsigned char tcp_flags;      // TCP flags and 2 from reserved space
#define TCP_FIN 0x01   // Finish flag
#define TCP_SYN 0x02   // Synchronize flag
#define TCP_RST 0x04   // Reset flag
#define TCP_PUSH 0x08  // Push flag
#define TCP_ACK 0x10   // Acknowledgment flag
#define TCP_URG 0x20   // Urgent flag
    unsigned short tcp_window;    // Window size
    unsigned short tcp_checksum;  // Checksum
    unsigned short tcp_urgent;    // Urgent pointer
};