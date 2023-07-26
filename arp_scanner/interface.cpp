#include "interface.h"

#include <sys/types.h>
#include <ifaddrs.h>

#include <vector>
#include <cstring>

// #include <iostream>

interface* interface::s_interfaces = nullptr;
int interface::s_interfaceCount = 0;

interface::interface() {
	
}

void interface::loadInterfaces() {
	ifaddrs* addrs = nullptr;
	getifaddrs(&addrs);
	
	std::vector<interface> interfs;
	
	for(ifaddrs* ifa = addrs; ifa != nullptr; ifa = ifa->ifa_next) {
		int interIndex = -1;
		for(int i = 0; i < interfs.size(); i++) {
			if(strcmp(interfs[i].m_name.c_str(), ifa->ifa_name) == 0) {
				interIndex = i;
				break;
			}
		}
		if(interIndex == -1) {
			interIndex = interfs.size();
			interfs.emplace_back();
			interfs[interIndex].m_name = ifa->ifa_name;
		}
		
		if(ifa->ifa_addr->sa_family == AF_PACKET) {
			memcpy(&interfs[interIndex].m_macAddr, &((sockaddr_ll*)ifa->ifa_addr)->sll_addr, sizeof(mac_addr));
		} else if(ifa->ifa_addr->sa_family == AF_INET) {
			memcpy(&interfs[interIndex].m_ipAddr, &((sockaddr_in*)ifa->ifa_addr)->sin_addr, sizeof(in_addr));
			memcpy(&interfs[interIndex].m_ipMask, &((sockaddr_in*)ifa->ifa_netmask)->sin_addr, sizeof(in_addr));
		}

	}
	s_interfaceCount = interfs.size();
	s_interfaces = new interface[s_interfaceCount];
	memcpy(s_interfaces, &interfs[0], sizeof(interface) * s_interfaceCount);
}

void interface::free() {
	delete[] s_interfaces;
}

int interface::interfaceCount() {
	return s_interfaceCount;
}

interface& interface::get(int index) {
	return s_interfaces[index];
}

const std::string& interface::name() const {
	return m_name;
}

const mac_addr& interface::mac() const {
	return m_macAddr;
}

const in_addr& interface::ip() const {
	return m_ipAddr;
}

const in_addr& interface::mask() const {
	return m_ipMask;
}




