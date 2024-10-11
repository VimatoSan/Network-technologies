#include "app.h"
#include <boost/system/detail/error_code.hpp>
#include <boost/asio.hpp>
#include "receiver.h"
#include "sender.h"
#include <iostream>

const std::string ipv4_default_addr = "0.0.0.0";
const std::string ipv6_default_addr = "::";


App::App(std::string address, const short port) : multicast_port(port), address(address) {
	if (App::is_ipv4()) {
		this->default_address = ipv4_default_addr;
	}
	else if (App::is_ipv6()) {
		this->default_address = ipv6_default_addr;
	}
	else {
		throw std::exception("Incorrect ip address");
	}
}

bool App::is_ipv4() {
	boost::system::error_code ec;
	boost::asio::ip::address_v4::from_string(this->address, ec);
	return !ec;
}

bool App::is_ipv6() {
	boost::system::error_code ec;
	boost::asio::ip::address_v6::from_string(this->address, ec);
	return !ec;
}

void App::start() {
	boost::asio::io_context io_context;
	Receiver r(io_context, boost::asio::ip::make_address(this->address), boost::asio::ip::make_address(this->default_address), this->multicast_port);
	Sender s(io_context, boost::asio::ip::make_address(this->address), this->multicast_port);
	io_context.run();

	return;
}