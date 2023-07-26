#pragma once

#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <linux/if_packet.h>
#include <net/ethernet.h> 

#include "mac_addr.h"

class interface {
public:
	interface();
	
	static void loadInterfaces();
	static void free();
	static int interfaceCount();
	static interface& get(int index);
	
	const std::string& name() const;
	
	const mac_addr& mac() const;
	
	const in_addr& ip() const;
	const in_addr& mask() const;
	
private:
	mac_addr m_macAddr;

	in_addr m_ipAddr;
	in_addr m_ipMask;


	std::string m_name;

	static interface* s_interfaces;
	static int s_interfaceCount;
};
