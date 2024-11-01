#pragma once
#include <boost/asio.hpp>
#include <fstream>
#include "connection.h"


using boost::asio::ip::tcp;

namespace tcp_connection {
	class Server {
	public:
		Server(boost::asio::io_context& io_context, const short port);

	private:
		std::atomic<int> connection_id;  // Потокобезопасный счетчик

		void start_accept();
		boost::asio::io_context io_context;
		tcp::acceptor acceptor_;
		std::ofstream fstream_;

	};
}