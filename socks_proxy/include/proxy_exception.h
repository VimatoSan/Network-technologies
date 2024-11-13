#pragma once
#include <string>
#include <exception>


class ProxyException : public std::exception {
//public:
//	explicit ProxyException(const std::string& message) : msg_(message) {}
//	const char* what() const noexcept override {
//		return msg_.c_str();
//	}
//
//private:
//	std::string msg_;

};

class InvalidRequestException : public ProxyException {};
class UnavailableCommandTypeException : public ProxyException {};
class AddressTypeException : public ProxyException {};
class ResolvingAddressException : public ProxyException {};

class HostConnectionException : public ProxyException {};

