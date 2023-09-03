from scapy.all import *
from scapy.all import IP, TCP, sniff
import socket

# Set the network interface to capture packets on (modify this based on your system)
interface = "en0"
# Set a filter to capture only TCP packets
filter_str = "tcp"

# Resolve the IP address of the website "www.justdial.com" and store it in a variable
website_ip = socket.gethostbyname("www.justdial.com")

# Initialize an empty list to store captured packets
packets_arr = []

# Declare and initialize three variables cnt1, cnt2, and cnt3 to zero
cnt1, cnt2, cnt3 = 0, 0, 0

# Define a function to capture and process network packets
def capture_packets(packet):
    
    # Declare cnt1, cnt2, and cnt3 as global variables to modify them within the function
    global cnt1, cnt2, cnt3
    
    # Check if the packet has an IP layer
    if packet.haslayer(IP):
        dest = packet[IP].dst  # Extract destination IP address
        src = packet[IP].src   # Extract source IP address
        
        # Check if the packet is related to the specified website IP address, if not, return
        if not(dest == website_ip or src == website_ip):
            return
    else:
        return
    
    # Check if the packet has a TCP layer
    if TCP in packet:
        # Check for SYN packet and ensure it's the first one
        if packet[TCP].flags == 2 and cnt1 == 0:
            print(packet.summary())
            packets_arr.append(packet)
            cnt1 += 1
        
        # Check for SYN-ACK packet and ensure it's the first one
        elif packet[TCP].flags == 18 and cnt2 == 0:
            print(packet.summary())
            packets_arr.append(packet)
            cnt2 +=1
        
        # Check for ACK packet and ensure it's the first one
        elif packet[TCP].flags == 16 and cnt3 == 0:
            print(packet.summary())
            packets_arr.append(packet)
            cnt3 += 1


# Start capturing packets
try:
# while(cnt1==0 or cnt2==0 or cnt3==0):
	sniff(iface=interface, prn=capture_packets, filter=filter_str)
	# sniff(iface=interface, prn=capture_packets, filter=filter_str)
	# Define the name of the output capture file
	pcap_filename = "3way.pcap"
	# Write the captured packets to the PCAP file
	wrpcap(pcap_filename, packets_arr)

except KeyboardInterrupt:
    # Handle user interruption
    print("Capture stopped by user.")
