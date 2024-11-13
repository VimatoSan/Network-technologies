#include "proxy_server.h"
#include <iostream>
#include "connection.h"

ProxyServer::ProxyServer(int port) : io_context(), acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {}

void ProxyServer::start() {
    accept();	
    io_context.run();
}

void ProxyServer::accept() {
	auto connection = std::make_shared<Connection>(io_context);
    acceptor_.async_accept(connection->get_client_socket(), [this, connection](const boost::system::error_code& error) {
        if (!error) {
            std::cout << "Accepted connection" << std::endl;
            connection->start();
        }
        else {
            std::cout << error.what();
        }
        accept();
    });
}