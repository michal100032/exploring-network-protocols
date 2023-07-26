#include <iostream>

#include "arp.h"
#include "interface.h"

// run with sudo

int main() {
	
	interface::loadInterfaces();
	for(int i = 0; i < interface::interfaceCount(); i++) {
		interface intf = interface::get(i);
		if(intf.name() == "lo")
			continue;
		
		std::cout << "Scanning interface " << intf.name() << "..." << std::endl;
		std::vector<arp_record> arps = arp::scan(intf);

		for(int arp = 0; arp < arps.size(); arp++) {
			std::cout << inet_ntoa(arps[arp].ip) << " " << arps[arp].mac << std::endl;
		}
	}

	return 0;
}
