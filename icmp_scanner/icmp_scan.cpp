#include "icmp_scan.h"

#include <thread>
#include <unistd.h>

#include <sys/types.h>
#include <fcntl.h>

#include <arpa/inet.h>
#include <iostream>

std::vector<in_addr> icmp::scan(const interface& intf) {
	std::vector<in_addr> ips;
	
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_ICMP);
	fcntl(sock, F_SETFL, O_NONBLOCK);
	
	std::thread listen(icmp::listen, sock, std::ref(ips), 3 * 1000 * 1000);
	
	uint32_t subnet = htonl(intf.mask().s_addr & intf.ip().s_addr);
	uint32_t addressCount = ~htonl(intf.mask().s_addr) + 1;
	
	for(int i = 0; i < addressCount; i++) {		
		sockaddr_in addr;
		addr.sin_family = AF_INET;
		addr.sin_port = 0;
		addr.sin_addr.s_addr = htonl(subnet + i);
			
		icmp::message mess;
		mess.id = 10;
		mess.checksum = calculateChecksum(mess);
		
		sendto(sock, &mess, sizeof(mess), 0, (sockaddr*)&addr, sizeof(addr));
		
		usleep(10 * 1000);
	}
	
	listen.join();
	
	close(sock);
	
	return ips;
}

void icmp::listen(int sock, std::vector<in_addr>& ips, int utime) {

	sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);

	char buffer[64];

	const int sleep = 50;
	for(int i = 0; i < utime / sleep; i++) {
		int len = recvfrom(sock, buffer, sizeof(buffer), 0, (sockaddr*)&addr, &addrlen);
		if(len > 0) {                  
			ips.push_back(addr.sin_addr);
		}
		usleep(sleep);
	}
}

uint16_t icmp::calculateChecksum(const icmp::message& mess) {
	uint16_t sum = ((uint16_t)mess.type << 8) + (uint16_t)mess.code + ntohs(mess.id) + ntohs(mess.seq);
        return htons(~sum);
}
