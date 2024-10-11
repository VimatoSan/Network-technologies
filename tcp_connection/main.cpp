#include "client.h"
#include "server.h"
#include <exception>
#include <iostream>

int main(int argc, char *argv[]) {
	setlocale(LC_ALL, "RU");
	boost::asio::io_context context;
	try {
		if (argc == 3 && (std::string(argv[1]) == "-s")) {
			Server(context, std::stoi(argv[2]));
			context.run();

		}
		else if (argc == 5 && (std::string(argv[1]) == "-c")) {
			const std::string filename(argv[2]);
			const std::string address(argv[3]);
			const std::string port(argv[4]);
			boost::asio::io_context io_context;
			Client(io_context, filename, address, port);
			context.run();
		}
		else {
			std::cout << argc << std::endl;
			std::cerr << "Not enough args.\nRequired <program> -s <port> for start server\nor <program> -c <filename>  <address>  <port> for start client" << std::endl;
			return 1;
		}
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}