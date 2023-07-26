#include "arp.h"

#include <cstring>

#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h> 
#include <net/if_arp.h>

#include <netinet/in.h>
#include <netinet/ip.h>

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

#include <thread>

const mac_addr BROADCAST_MAC = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };

void printBytes(uint8_t* bytes, int len) {
	for(int i = 0; i < len; i++) {
		for(int bit = 7; bit >= 0; bit--) {
			std::cout << ((bytes[i] & (1 << bit)) >> bit);
		}
		std::cout << (i % 2 == 0 ? " " : "\n");
	}
}

arp::arp() {

}

void arp::receiveArp(int sock, std::vector<arp_record>& arps) {
	sockaddr_ll addr;
	socklen_t addrlen = sizeof(addr);

	char buffer[128];

	const int time = 3 * 1000 * 1000;
	const int sleep = 50;
	for(int i = 0; i < time / sleep; i++) {
		int len = recvfrom(sock, buffer, sizeof(buffer), 0, (sockaddr*)&addr, &addrlen);
		if(len > 0) {                  
			arp_message message;
			memcpy(&message, buffer, sizeof(message));

			arps.emplace_back(message.senderProtocolAddr, message.senderHardwareAddr);
		}
		usleep(sleep);
	}
}

std::vector<arp_record> arp::scanInst(interface& intf) {
	int sock = socket(AF_PACKET, SOCK_DGRAM, htons(ETH_P_ARP));

	sockaddr_ll addr = makeBroadcastSock(intf);
	fcntl(sock, F_SETFL, O_NONBLOCK);

	std::vector<arp_record> arps;
	std::thread listen(&arp::receiveArp, this, sock, std::ref(arps));
	
	uint32_t subnet = htonl(intf.mask().s_addr & intf.ip().s_addr);
	uint32_t addressCount = ~htonl(intf.mask().s_addr) + 1;
	
	for(int i = 0; i < addressCount; i++) {
		in_addr ip;
		ip.s_addr = htonl(subnet + i); // 192.168.0.i
		arp_message message(intf.mac(), intf.ip(), BROADCAST_MAC, ip);
		
		sendto(sock, &message, sizeof(message), 0, (sockaddr*)&addr, sizeof(addr));
		
		usleep(10 * 1000);
	}

	listen.join();

	close(sock);

	return arps;
}


sockaddr_ll arp::makeBroadcastSock(interface& intf) {
	sockaddr_ll addr;
	memset(&addr, 0, sizeof(addr));
	addr.sll_ifindex = if_nametoindex(intf.name().c_str());
	addr.sll_family = AF_PACKET;
	addr.sll_protocol = htons(ETH_P_ARP);
	addr.sll_halen = 6;
	addr.sll_hatype = htons(ARPHRD_ETHER);
	addr.sll_pkttype = PACKET_BROADCAST;
	memcpy(addr.sll_addr, BROADCAST_MAC.address, 6);

	return addr;
}
