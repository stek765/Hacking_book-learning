// server_example.c

/* The idea is to have a generic server (socket) that accpet any connection
   and once a client connects, it creates a new socket to communicate with the client.

            +---------------------+
            |     sockfd          |  ← generic listening socket
            |  (bind + listen)    |
            +---------------------+
                      |
            --------accept()-------
           /          |           \
          /           |            \
         ↓            ↓             ↓
  +-----------+  +-----------+  +-----------+
  | clientfd1 |  | clientfd2 |  | clientfd3 |  ← one per connected client
  +-----------+  +-----------+  +-----------+
- - - - - - - - - - - - - - - - - - - - - - - - - - -
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h> // per close()

#include "hacking.h" // Custom library with fatal(), dump(), ecc.

#define PORT 7890  // Port on which the server will listen

int main(void) {
    int sockfd, new_sockfd;
    struct sockaddr_in host_addr, client_addr;  // Prepare Server and Client structs to save their informations
    socklen_t sin_size;  
    int recv_length = 1, yes = 1;
    char buffer[1024];

    // 1. Create socket
    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1)
        fatal("in socket()"); 

    // 2. Set SO_REUSEADDR to reuse the port 
    if (setsockopt(sockfd,  SOL_SOCKET,     SO_REUSEADDR,   &yes,          sizeof(int))   == -1)
    //            [socket | socket option |  reuse flag   | yes=1/active | size of yes]
        fatal("setting socket option SO_REUSEADDR");

    // 3. Set servers address and port
    host_addr.sin_family = AF_INET;
    host_addr.sin_port = htons(PORT);
    host_addr.sin_addr.s_addr = 0;            // INADDR_ANY → accept any IP
    memset(&(host_addr.sin_zero), '\0', 8);   // zero padding (fill the rest of the struct with zeros)

    /*
      struct sockaddr_in {
         short int          sin_family;   // Address family (AF_INET)
         unsigned short int sin_port;     // Port number
         struct in_addr     sin_addr;     // IP address
         unsigned char      sin_zero[8];  // Padding (not used)
        };

        sockfd isn't inside sockaddr_in because it's a file descriptor managed by the OS.
    */

    // 4. Associate the socket with the address and port (bind in struct sockaddr_in)
    if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1)
        fatal("binding to socket");

    // 5. Put the socket in listening mode
    if (listen(sockfd, 5) == -1)    // 5 is the maximum number of pending connections
        fatal("listening on socket");

    printf("[+] Waiting for incoming connections on port %d...\n", PORT);
    sin_size = sizeof(struct sockaddr_in);  // Size of the client address structure

    // 6. Accept a connection from a client (Client info will be saved in the struct client_addr)
    if ((new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size)) == -1)
        fatal("accepting connection"); 

    printf("[+] Got connection from %s\n", inet_ntoa(client_addr.sin_addr));

    // 7. Receive data from the client
    while ((recv_length = recv(new_sockfd, &buffer, 1024, 0)) > 0) {
        printf("[+] Received %d bytes:\n", recv_length);
        dump((unsigned char *)buffer, recv_length);
    }

    // 8. Close the sockets
    close(new_sockfd);   // Close the new socket (client connection)
    close(sockfd);       // Close the listening socket
    return 0;
}

// open another terminal and run:   nc     127.0.0.1 7890      to communicate with the server 
// or using telnet like so:         telnet 127.0.0.1 7890 

// (default ports are listed in /etc/services)