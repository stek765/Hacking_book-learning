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