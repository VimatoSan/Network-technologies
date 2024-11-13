#pragma once

#include <boost/asio.hpp>


using boost::asio::ip::tcp;

const int BUFFER_SIZE = 1024 * 64;

class Connection : public std::enable_shared_from_this<Connection>
{
public:
	Connection(boost::asio::io_context& io_context);
	void start();
	
	tcp::socket& get_client_socket();

private:
	boost::asio::io_context& io_context;
	tcp::socket client_socket;
	tcp::socket host_socket;
	uint8_t client_buffer[BUFFER_SIZE];
	uint8_t host_buffer[BUFFER_SIZE];

	void send_init_answer();
	void read_init();
	void send_request_answer();
	void read_request();

	void read_client();
	void read_host();

	void close();
	void connect(std::string address, std::string port);

	std::string get_address(uint8_t* request_buff);				// Можно вынести в отдельный хэдер утилит 
	std::string ipv4_to_string(const uint8_t* ip_bytes);		//
	int get_port(uint8_t* request_buff);						//
	size_t get_address_end_index(uint8_t* request_buff);		//
	void verify_request(uint8_t* request_buff, size_t bytes);	//
};