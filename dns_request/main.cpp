#include <cstring>
#include <iostream>
#include <fstream>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

struct dns_header {
	uint16_t identifier;
	uint16_t flags;

	uint16_t questionCount;
	uint16_t answerRrCount;
	uint16_t authoritiveRrCount;
	uint16_t additionalRrCount;
};

// useful for debugging
void printBytes(uint8_t* bytes, int len) {
	for (int i = 0; i < len; i++) {
		for(int bit = 7; bit >= 0; bit--) {
			std::cout << ((bytes[i] & (1 << bit)) >> bit);
		}
		std::cout << (i % 2 == 0 ? " " : "\n");
	}
	std::cout << std::endl << std::endl;
}

int main() {
	std::string requestedDomain;
	std::cout << "Enter the domain: ";
	std::cin >> requestedDomain;
	
	int domainLength = requestedDomain.length();

	int sock = socket(AF_INET, SOCK_DGRAM, 0);

	// create the dns header
	dns_header header;
	std::memset(&header, 0, sizeof(dns_header));
	header.questionCount = htons(1); // <- 1 question
	header.flags = htons(0x100); // <- recursion desired

	uint8_t buffer[512];
	std::memset(buffer, 0, 512);

	std::memcpy(buffer, &header, 12);
	std::memcpy(buffer + 13, requestedDomain.c_str(), domainLength + 1);

	// insert number of characters seperated by periods
	// e.g. www.google.com -> 3www6google3com
	int point = -1;
	for(int i = 0; i < domainLength; i++) {
		if(requestedDomain[i] == '.') {
			buffer[13 + point] = i - point - 1;
			point = i;
		}
	}
	buffer[13 + point] = domainLength - point - 1;

	// set the TYPE and CLASS fields of the request 
	buffer[13 + domainLength + 1] = 0;
	buffer[13 + domainLength + 2] = 1;
	buffer[13 + domainLength + 3] = 0;
	buffer[13 + domainLength + 4] = 1;
	
	// read DNS address from /etc/resolv.conf
	std::string dnsAddress;
	
	std::ifstream resolv("/etc/resolv.conf");
	std::string line;
	while (std::getline(resolv, line)) {
		if (line.find("nameserver") != std::string::npos) {
			dnsAddress = line.substr(11);
			break;
		}
	}
	resolv.close();
	
	if(dnsAddress.length() == 0) {
		std::cout << "Unable to read the DNS address!" << std::endl;
		return 0;
	}
	
	// prepare the socket
	sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(53); 
	addr.sin_addr.s_addr = inet_addr(dnsAddress.c_str());
	
	// send the request
	int requestLength = domainLength + 18;
	sendto(sock, buffer, requestLength, 0, (sockaddr*)&addr, sizeof(addr));

	// receive and print the address
	int len = recv(sock, buffer, 512, 0);
	if(len > 12) {
		for(int i = 0; i < 4; i++) {
			std::cout << unsigned(buffer[requestLength + 12 + i]) << (i == 3 ? '\n' : '.');
		}
	}

	return 0;
}
