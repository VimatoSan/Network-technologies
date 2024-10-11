#include <iostream>
#include "server.h"
#include <boost/asio.hpp>
#include "connection.h"
#include <exception>


using boost::asio::ip::tcp;

Server::Server(boost::asio::io_context& io_context, const short port) : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), connection_id(0) {
    std::cout << "Server start" << std::endl;
    start_accept();
}


void Server::start_accept() {
    auto new_connection = std::make_unique<Connection>(io_context, connection_id.load());
    connection_id.fetch_add(1);
    acceptor_.accept(new_connection->get_socket());
    std::thread t(&Connection::accept_handle, std::move(new_connection));
    t.detach();
    start_accept();
}
