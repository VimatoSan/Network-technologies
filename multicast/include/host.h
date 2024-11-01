#pragma once
#include <string>
namespace multicast::structs {
	struct Host {
		Host(std::string address, int port);
		std::string address;
		int port;

		bool operator==(const Host& other) const;
	};
}

namespace std {
	template<>
	struct hash<multicast::structs::Host> {
		std::size_t operator()(const multicast::structs::Host& k) const;
	};
}