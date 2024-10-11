#pragma once
#include <string>

class App {
public:
	App(std::string address, const short port);
	void start();


private:
	bool is_ipv4();
	bool is_ipv6();

	const short multicast_port;
	const std::string address;
	std::string default_address;
};