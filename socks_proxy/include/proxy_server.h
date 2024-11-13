#pragma once

#include <boost/asio.hpp>
#include "connection.h"


using boost::asio::ip::tcp;

class ProxyServer {
public:
	ProxyServer(int port);
	void start();

private:
	void accept();
	std::vector<std::shared_ptr<Connection>> connections;
	boost::asio::io_context io_context;
	tcp::acceptor acceptor_;
};