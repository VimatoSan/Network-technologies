#pragma once
#include <string>

struct Host {
	Host(std::string address, int port);
	std::string address;
	int port;

	bool operator==(const Host& other) const;
};

namespace std {
	template<>
	struct hash<Host> {
		std::size_t operator()(const Host& k) const;
	};
}