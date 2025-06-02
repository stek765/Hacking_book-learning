/*
A standardized library called libpcap can be used to smooth out the inconsistencies of raw sockets.
(it still uses raw sockets, but it provides a consistent API across different platforms).

Here we'll rewrite the previous raw TCP sniffer using libpcap.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h> // the libpcap header file

#include "hacking.h"

// Pcap functions use a error buffer to return error and status messages.
// so this function is designed to display this buffer to the user.
void pcap_fatal(const char *failed_in, const char *errbuf) {
    printf("Fatal Error in%s: %s\n", failed_in, errbuf);
    exit(1);
}

int main() {
    struct pcap_pkthdr header; // Header for the packet
    const u_char *packet; // Pointer to the packet data
    char errbuf[PCAP_ERRBUF_SIZE]; // Buffer for error messages
    char *device; // Device to capture packets from
    pcap_t *pcap_handle; // Pcap handle for the capture session (works like a file descriptor)
    int i;

    device = pcap_lookupdev(errbuf); // Find a suitable device to capture packets from
    if (device == NULL) {
        pcap_fatal("pcap_lookupdev", errbuf); // If no device is found, print error and exit
    }
    printf("Sniffing on device: %s\n", device);

    pcap_handle = pcap_open_live(device, BUFSIZ, 1, 1000, errbuf); // Open the device for live capture (1 = promiscuous mode)
    if (pcap_handle == NULL) {
        pcap_fatal("pcap_open_live", errbuf); // If opening the device fails, print error and exit
    }

    for(i = 0; i < 3; i++) {
    packet = pcap_next(pcap_handle, &header); // Capture the next packet
    if (packet == NULL) {
        printf("No packet captured. Timeout or error.\n");
        continue; // Skip this iteration
    }
    printf("Got a %d byte packet\n", header.len); // Print the size of the captured packet
    dump(packet, header.len); // Dump the packet data to the console
}
    pcap_close(pcap_handle); // Close the pcap handle to free resources
}

// compile with: gcc -o 22_libpcap_sniffer 22_libpcap_sniffer.c -l pcap

