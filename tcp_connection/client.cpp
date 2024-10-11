#include "client.h"
#include "constants.h"
#include <fstream>
#include <iostream>
#include <boost/asio.hpp>

std::string Client::wait_answer() {
	boost::system::error_code ec;
	int buff_size = socket_.read_some(boost::asio::buffer(buffer), ec);
	if (!ec) {
		return std::string(buffer, buff_size);
	}
	else {
		return ec.message();
	}
	// std::cout << "Server reply: " << std::string(buffer, buff_size) << std::endl;
}


Client::Client(boost::asio::io_context &io_context, const std::string &filename, const std::string &address, const std::string &port) 
	: fstream_(filename, std::ios_base::binary), fname(filename), socket_(io_context) {
	std::cout << "Client launch!" << std::endl;

	if (!this->fstream_.is_open()) 
		throw std::exception("Error while opening file");
	this->fsize = this->get_fsize();
	boost::asio::ip::tcp::resolver resolver(io_context);
	boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(address, port);
	boost::asio::connect(socket_, endpoints);
	send_file();
	std::cout << "Server reply: " << wait_answer() << std::endl;
}


Client::~Client() {
	std::cout << "Close connection" << std::endl;
	fstream_.close();
	socket_.close();
}


void Client::send_file() {
	boost::system::error_code error;
	std::cout << "Start sending" << std::endl;

	std::string message = std::to_string(fsize) + " " + fname + "-";
	std::cout << "Sending file metadata: " << message << std::endl;
	boost::asio::write(socket_, boost::asio::buffer(message, message.size()), error);
	if (error) {
		throw std::exception("Error during sending info about file");
	}
	std::cout << "Sending meta end" << std::endl;
	size_t total_bytes = 0;
	while (fstream_.read(buffer, BUFFER_SIZE) || fstream_.gcount() > 0) {
		size_t bytes_to_send = fstream_.gcount();  
		total_bytes += bytes_to_send;
		boost::asio::write(socket_, boost::asio::buffer(buffer, bytes_to_send), error);

		if (error) {
			throw std::runtime_error("Error during sending file: " + error.message());
		}
	}
	// std::cout << total_bytes << " " << fsize << std::endl;

	if (total_bytes == fsize) {
		std::cout << "Sending correctly end" << std::endl;
	}
	else {
		socket_.close();
		std::cout << "Not all file was sent";
	}
}


size_t Client::get_fsize() {
	fstream_.seekg(0, std::ios::end);
	size_t size = fstream_.tellg();
	fstream_.seekg(0, std::ios::beg);

	return size;
}
