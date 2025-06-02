/*
On Linux, the structure for lower layer procotols can be found in if_ether.h,ip.h, tcp.h, etc.

OR.. we could just create our onw header structures based on the RFCs. To gain a better understanding
of how these protocols work and how to decode them. (look hacking-network.h)


_______________________________________________________________________
(ETHERNET STRUCT for HEADER) (14 bytes in size)
  The official if_ether.h struct:  [source MAC address, destination MAC address, 16 bit protocol type]
      struct ethhdr { 
          unsigned char h_dest[ETH_ALEN];     // Destination MAC address (6 bytes)
          unsigned char h_source[ETH_ALEN];   // Source MAC address (6 bytes)
          __be16 h_proto;                     // Packet type ID field (2 bytes), __be16 -> unsigned short integer
      };
_______________________________________________________________________


_______________________________________________________________________
(IP STRUCT for HEADER) (20 bytes in size)
  struct iphdr {
      #if __BYTE_ORDER == __LITTLE_ENDIAN
          unsigned int ihl:4;
          unsigned int version:4;
      #elif __BYTE_ORDER == __BIG_ENDIAN
          unsigned int version:4;
          unsigned int ihl:4;
      #else
      # error "Please fix <bits/endian.h>"
      #endif
          uint8_t tos;
          uint16_t tot_len;
          uint16_t id;
          uint16_t frag_off;
          uint8_t ttl;
          uint8_t protocol;
          uint16_t check;
          uint32_t saddr;
          uint32_t daddr;
};
_______________________________________________________________________


_______________________________________________________________________
(TCP STRUCT for HEADER) (20 bytes in size)
struct tcphdr {
    uint16_t source;      // Source port
    uint16_t dest;        // Destination port
    uint32_t seq;         // Sequence number
    uint32_t ack_seq;     // Acknowledgement number
#if __BYTE_ORDER == __LITTLE_ENDIAN
    uint16_t res1:4;
    uint16_t doff:4;
    uint16_t fin:1;
    uint16_t syn:1;
    uint16_t rst:1;
    uint16_t psh:1;
    uint16_t ack:1;
    uint16_t urg:1;
    uint16_t ece:1;
    uint16_t cwr:1;
#elif __BYTE_ORDER == __BIG_ENDIAN
    uint16_t doff:4;
    uint16_t res1:4;
    uint16_t cwr:1;
    uint16_t ece:1;
    uint16_t urg:1;
    uint16_t ack:1;
    uint16_t psh:1;
    uint16_t rst:1;
    uint16_t syn:1;
    uint16_t fin:1;
#else
# error "Adjust your <bits/endian.h> defines"
#endif
    uint16_t window;      // Window size
    uint16_t check;       // Checksum
    uint16_t urg_ptr;     // Urgent pointer
};
_______________________________________________________________________


Now, using pcp_loop() with a callback function, we can capture packets and use our 
header structures to decode them.
*/

#include <stdio.h>
#include <pcap.h>
#include "hacking-network.h"
#include "hacking.h"

#define ETHER_HDR_LEN 14

void pcap_fatal(const char *fatal, const char *errbuf);
void decode_ethernet(const u_char *);
void decode_ip(const u_char *);
u_int decode_tcp(const u_char *);

void caught_packet(u_char *, const struct pcap_pkthdr *, const u_char *);

int main() {
    struct pcap_pkthdr cap_header;
    const u_char *packet, *pkt_data;
    char errbuf[PCAP_ERRBUF_SIZE];
    char *device;

    pcap_t *pcap_handle;

    device = pcap_lookupdev(errbuf);
    if (device == NULL) {
        pcap_fatal("pcap_lookupdev() failed", errbuf);
    }

    printf("Sniffing on device: %s\n", device);

    pcap_handle = pcap_open_live(device, 4096, 1, 0, errbuf);
    if (pcap_handle == NULL) {
        pcap_fatal("pcap_open_live() failed", errbuf);
    }

    pcap_loop(pcap_handle, 3, caught_packet, NULL);
    
    pcap_close(pcap_handle);
}

void caught_packet(u_char *user_args, const struct pcap_pkthdr *cap_header, const u_char *packet) {
    int tcp_header_length, total_header_size, pkt_data_len;
    const u_char *pkt_data;

    printf("=== Got a %d byte packet ===\n", cap_header->len);
    decode_ethernet(packet);
    decode_ip(packet + ETHER_HDR_LEN);
    tcp_header_length = decode_tcp(packet + ETHER_HDR_LEN + sizeof(struct ip_hdr));

    total_header_size = ETHER_HDR_LEN + sizeof(struct ip_hdr) + tcp_header_length;
    pkt_data = packet + total_header_size;
    pkt_data_len = cap_header->len - total_header_size;

    if(pkt_data_len > 0) {
        printf("\t\t\t%3d bytes of packet data\n", pkt_data_len);
    } else {
        printf("\t\t\tNo packet data\n");
    }
}

void pcap_fatal(const char *failed_in, const char *errbuf) {
    printf("Fatal Error in %s: %s\n", failed_in, errbuf);
    exit(1);
}

void decode_ethernet(const u_char *header_start) {
    const struct ether_hdr *ethernet_header;
    ethernet_header = (const struct ether_hdr *)header_start;

    printf("\t Layer 2: Ethernet Header\n");
    printf("\t\t Source: %02x:%02x\n", ethernet_header->ether_src_addr[0],
           ethernet_header->ether_src_addr[1]);
    printf("\t\t Dest: %02x:%02x\n", ethernet_header->ether_dest_addr[0],
           ethernet_header->ether_dest_addr[1]);
    printf("\t\t Type: %hu\n", ethernet_header->ether_type);
}

void decode_ip(const u_char *header_start) {
    const struct ip_hdr *ip_header;
    ip_header = (const struct ip_hdr *)header_start;

    char src_str[INET_ADDRSTRLEN];
    char dest_str[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &ip_header->ip_src_addr, src_str, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ip_header->ip_dest_addr, dest_str, INET_ADDRSTRLEN);


    printf("\t Layer 3: IP Header\n");
    printf("\t\t Source: %s\n", src_str);
    printf("\t\t Dest: %s\n", dest_str);
    printf("\t\t ID: %hu\n", ntohs(ip_header->ip_id));
    printf("\t\t Length: %hu\n", ntohs(ip_header->ip_len));
}

u_int decode_tcp(const u_char *header_start) {
    const struct tcp_hdr *tcp_header;
    tcp_header = (const struct tcp_hdr *)header_start;
    int header_size = 4 * tcp_header->tcp_offset;

    printf("\t Layer 4: TCP Header\n");
    printf("\t\t Src Port: %hu\n", ntohs(tcp_header->tcp_src_port));
    printf("\t\t Dest Port: %hu\n", ntohs(tcp_header->tcp_dest_port));
    printf("\t\t Seq #: %u\n", ntohl(tcp_header->tcp_seq));
    printf("\t\t Ack #: %u\n", ntohl(tcp_header->tcp_ack));
    printf("\t\t Header size: %d\n", header_size);
    printf("\t\t Flags: ");

    if(tcp_header->tcp_flags & TCP_FIN) printf("FIN ");
    if(tcp_header->tcp_flags & TCP_SYN) printf("SYN ");
    if(tcp_header->tcp_flags & TCP_RST) printf("RST ");
    if(tcp_header->tcp_flags & TCP_PUSH) printf("PUSH ");
    if(tcp_header->tcp_flags & TCP_ACK) printf("ACK ");
    if(tcp_header->tcp_flags & TCP_URG) printf("URG ");

    printf("\n");

    return header_size;
}
