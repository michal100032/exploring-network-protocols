#include <iostream>
#include <arpa/inet.h>

#include "icmp_scan.h"
#include "interface.h"


int main() {
	interface::loadInterfaces();
	
	for(int i = 0; i < interface::interfaceCount(); i++) {
		interface intf = interface::get(i);
		if(intf.name() == "lo") 
			continue;
		std::cout << "Scanning interface " << intf.name() << "..." << std::endl;	
		
		std::vector<in_addr> ips = icmp::scan(intf);
		for(int ip = 0; ip < ips.size(); ip++) {
			std::cout << inet_ntoa(ips[ip]) << std::endl;
		}
	}

	return 0;
}
