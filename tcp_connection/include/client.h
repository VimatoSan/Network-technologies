#pragma once
#include <boost/asio.hpp>
#include <fstream>
#include "constants.h"


namespace tcp_connection {
	class Client {
	public:
		Client(boost::asio::io_context& io_context, const std::string& filename, const std::string& address, const std::string& port);
		~Client();
		void send_file();
		int num;

	private:
		std::ifstream fstream_;
		size_t fsize;
		const std::string fname;
		char buffer[consts::BUFFER_SIZE];

		// const boost::asio::ip::tcp::endpoint endpoint_;
		boost::asio::ip::tcp::socket socket_;
		std::string wait_answer();
		// size_t get_fsize();
	};
}