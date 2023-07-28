In this repository I store all my projects in which I explore different features of simple network protocols like IP, ICMP and ARP. All projects published here were written in C++ and make use of networking libraries for Linux. 
### ARP scanner
This program sends ARP messages targeted to all IP addresses available in the network; the responses it receives back indicate the presence of other devices. ARP was implemented from scratch just to see how it works. To work properly the program has to be launched with sudo.

### DNS request
The program first reads the DNS address from /etc/resolv.conf and then sends a request to it with a user-specified domain. Formatting of the DNS packet has been implemented from scratch.

### ICMP scanner
This program is very similar to the ARP scanner, the only difference being that it uses ICMP instead of ARP.
