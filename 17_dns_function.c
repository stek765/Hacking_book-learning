/*
DNS is a protocol that translates human-readable domain names into IP addresses.
It works by sending a char pointer (string) to a DNS server, which responds with a pointer
to a hostent structure, or NULL if the domain name is not found.

The hostent structure is filled with information about the host, including the IP address.
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

FROM  /usr/include/netdb.h:

// Desciption of database entries for a single host.
struct hostent {
    char  *h_name;        // Nome ufficiale dell'host
    char **h_aliases;     // Lista di alias (nomi alternativi), terminata da NULL  (ptr to char ptrs)
    int    h_addrtype;    // Tipo di indirizzo: AF_INET (IPv4), AF_INET6, ecc.
    int    h_length;      // Lunghezza dell'indirizzo in byte (es. 4 per IPv4)
    char **h_addr_list;   // Lista di indirizzi IP (terminata da NULL)

    // alias: il primo indirizzo è anche accessibile da:
    // char *h_addr → h_addr_list[0]
};
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

The following code demonstrates how to use the DNS functions in C, using gethostbyname() function:
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <netdb.h>

#include "hacking.h" // Custom library with fatal(), dump(), ecc.

int main(int argc, char const *argv[])
{
    struct hostent *host_info;
    struct in_addr *address;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s <hostname>\n", argv[0]);
        exit(1);
    }

    // Get host information by name
    host_info = gethostbyname(argv[1]);
    if (host_info == NULL) {
        fprintf(stderr, "Error: Host %s not found.\n", argv[1]);
        exit(1);
    }else {
        address = (struct in_addr *) (host_info->h_addr_list[0]);  // or host_info->h_addr
        printf("%s has address: %s\n", argv[1], inet_ntoa(*address));
    }
}

