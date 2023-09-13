#include <iostream>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <cstring>
#include <cstdlib>

std::string tcp_handshake_file = "tcp_handshake.pcap";
std::string udp_packets_file = "udp_packets.pcap";

pcap_dumper_t *tcp_handshake_dumpfile = nullptr;
pcap_dumper_t *udp_packets_dumpfile = nullptr;

int tcp_handshake_counter = 0;
int udp_packets_counter = 0;

void capture_packets(u_char *user, const struct pcap_pkthdr *pkthdr, const u_char *packet)
{
    struct ip *ip_header = (struct ip *)(packet + 14); // Skip Ethernet header

    // Check if it's a TCP packet
    if (ip_header->ip_p == IPPROTO_TCP)
    {
        struct tcphdr *tcp_header = (struct tcphdr *)(packet + 14 + (ip_header->ip_hl << 2)); // Skip Ethernet and IP headers

        if (tcp_header->th_flags == TH_SYN || tcp_header->th_flags == (TH_SYN | TH_ACK) || tcp_header->th_flags == TH_ACK)
        { // TCP Handshake (SYN, SYN-ACK, ACK)
            if (tcp_handshake_counter < 3 && tcp_handshake_dumpfile != nullptr)
            {
                pcap_dump((u_char *)tcp_handshake_dumpfile, pkthdr, packet);
                tcp_handshake_counter++;
            }
        }
    }
    else if (ip_header->ip_p == IPPROTO_UDP)
    {
        // UDP Packet
        if (udp_packets_counter < 2 && udp_packets_dumpfile != nullptr)
        {
            pcap_dump((u_char *)udp_packets_dumpfile, pkthdr, packet);
            udp_packets_counter++;
        }
    }
}

int main()
{
    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t *handle;

    handle = pcap_open_live("en0", BUFSIZ, 1, 1000, errbuf); // Change the interface name as needed

    if (handle == nullptr)
    {
        std::cerr << "Could not open device: " << errbuf << std::endl;
        return 1;
    }

    struct bpf_program fp;
    std::string filter = "tcp or udp"; // Capture both TCP and UDP packets

    if (pcap_compile(handle, &fp, filter.c_str(), 0, PCAP_NETMASK_UNKNOWN) == -1)
    {
        std::cerr << "Could not parse filter: " << pcap_geterr(handle) << std::endl;
        return 1;
    }

    if (pcap_setfilter(handle, &fp) == -1)
    {
        std::cerr << "Could not install filter: " << pcap_geterr(handle) << std::endl;
        return 1;
    }

    // Open PCAP dump files for TCP handshake and UDP packets
    tcp_handshake_dumpfile = pcap_dump_open(handle, tcp_handshake_file.c_str());
    udp_packets_dumpfile = pcap_dump_open(handle, udp_packets_file.c_str());

    if (tcp_handshake_dumpfile == nullptr || udp_packets_dumpfile == nullptr)
    {
        std::cerr << "Could not open the PCAP files for writing." << std::endl;
        return 1;
    }

    // Start packet capture
    pcap_loop(handle, 0, capture_packets, nullptr);

    // Close the PCAP files
    pcap_dump_close(tcp_handshake_dumpfile);
    pcap_dump_close(udp_packets_dumpfile);

    return 0;
}
