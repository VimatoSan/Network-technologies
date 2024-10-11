#include "host.h"

bool Host::operator==(const Host& other) const {
	return address == other.address && port == other.port;
}

Host::Host(std::string address, int port) {
	this->address = address;
	this->port = port;
}


std::size_t std::hash<Host>::operator()(const Host& host) const {
	std::size_t h1 = std::hash<std::string>()(host.address);
	std::size_t h2 = std::hash<int>()(host.port);
	return h1 ^ (h2 << 1);
}