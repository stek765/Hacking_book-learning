/*
    THE IDEA:
    - - - - - - - - - - - - - - - - - - - - - - - - - - - 
    This program connects to a web server and retrieves the "Server" header
    to identify the web server software being used. 

    It uses the `gethostbyname`function to resolve the hostname to an IP address, 
    then creates a socket to connect to the server on port 80 (HTTP).

    After establishing the connection, it sends a simple HTTP HEAD request and reads the response
    until it finds the "Server" header, which it then prints to the console.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "hacking.h"
#include "hacking-network.h"

int main(int argc, char *argv[]) {
    int sockfd;
    struct hostent *host_info;
    struct sockaddr_in target_addr;
    unsigned char buffer[4096];

    if(argc < 2) {
        printf("Usage: %s <hostname>\n", argv[0]);
        exit(1);
    }

    if((host_info = gethostbyname(argv[1])) == NULL)
        fatal("looking up hostname");

    if((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        fatal("in socket");

    target_addr.sin_family = AF_INET;
    target_addr.sin_port = htons(80);
    target_addr.sin_addr = *((struct in_addr *)host_info->h_addr); // put the resolved IP address into the sockaddr_in structure
    memset(&(target_addr.sin_zero), '\0', 8); // Zero the rest of the struct.

    if (connect(sockfd, (struct sockaddr *)&target_addr, sizeof(struct sockaddr)) == -1)
        fatal("connecting to target server");

    send_string(sockfd, "HEAD / HTTP/1.0\r\n\r\n"); // HEAD request to get headers only (information about the resource, not the resource itself)

    while(recv_line(sockfd, buffer)) {
        if(strncasecmp(buffer, "Server:", 7) == 0) {
            printf("The web server for %s is %s\n", argv[1], buffer+8);
            exit(0);
        }
    }

    printf("Server line not found\n");
    exit(1);
}