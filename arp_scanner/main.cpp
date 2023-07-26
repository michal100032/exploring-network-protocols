#include <iostream>

#include "interface.h"


std::ostream& operator<<(std::ostream& stream, const in_addr& ip) {
	in_addr mip = ip;
	uint8_t* byte = (uint8_t*)&mip;
	while(byte < (uint8_t*)&mip + 3) {
		stream << unsigned(*byte) << ".";
		byte++;
	}
	stream << unsigned(*byte);
	return stream;
}

int main() {
	
	interface::loadInterfaces();
	for(int i = 0; i < interface::interfaceCount(); i++) {
		std::cout << interface::get(i).name() << std::endl;
		std::cout << interface::get(i).ip() << std::endl;
		std::cout << interface::get(i).mask() << std::endl;
		std::cout << interface::get(i).mac() << std::endl;
	}

	return 0;
}
