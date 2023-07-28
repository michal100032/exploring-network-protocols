#pragma once

#include <vector>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "interface.h"

namespace icmp {
	struct message {
	    uint8_t type = 8;
	    uint8_t code = 0;

	    uint16_t checksum;

	    uint16_t id = 0;
	    uint16_t seq = 0;
	};
	
	std::vector<in_addr> scan(const interface& intf);
	uint16_t calculateChecksum(const message& mess);
	void listen(int sock, std::vector<in_addr>& ips, int utime);
}
