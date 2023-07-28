#pragma once

#include <string>

#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

class interface {
public:
	interface();
	
	static void loadInterfaces();
	static void free();
	static int interfaceCount();
	static interface& get(int index);
	
	const std::string& name() const;
	
	const in_addr& ip() const;
	const in_addr& mask() const;
	
private:
	in_addr m_ipAddr;
	in_addr m_ipMask;


	std::string m_name;

	static interface* s_interfaces;
	static int s_interfaceCount;
};
