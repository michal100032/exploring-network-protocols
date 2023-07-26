#include "mac_addr.h"

std::ostream& operator<<(std::ostream& stream, const mac_addr& mac) {
	stream << std::hex;
	for(int i = 0; i < 5; i++) {
		if(mac.address[i] < 16) {
			stream << 0;
		}
		stream << unsigned(mac.address[i]) << ":";
	}
	if(mac.address[5] < 16) {
		stream << 0;
	}
	stream << unsigned(mac.address[5]);
	stream << std::dec;

	return stream;
}
