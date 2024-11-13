#include "connection.h"
#include <iostream>
#include "proxy_const.h"
#include "proxy_exception.h"

Connection::Connection(boost::asio::io_context& io_context) : io_context(io_context), client_socket(io_context), host_socket(io_context) {}

tcp::socket& Connection::get_client_socket() {
    return client_socket;
}

void print(uint8_t *request_buff, size_t bytes) {
    std::cout << "Bytes: " << bytes << std::endl;

    for (int i = 0; i < bytes; i++)
        std::cout << request_buff[i] << " ";
    std::cout << std::endl;
}

void Connection::read_init() {
    uint8_t request_buff[MAX_INIT_REQUEST];
    boost::system::error_code error;
    std::cout << "Wait to read" << std::endl;
    size_t bytes = client_socket.read_some(boost::asio::buffer(request_buff), error);
    if (error) {
        std::cout << error.message() << std::endl;
    }
    if (bytes >= MIN_GREETING_SIZE && request_buff[VERSION_INDEX] == SOCKS5_VERSION) {
        send_init_answer();
    }
    else {
        std::cout << "Not SOCKS5 request. Closing Connection" << std::endl;
        close();
    }   
 }

void Connection::send_init_answer() {
    char bytes[2] = { SOCKS5_VERSION, SOCKS5_AUTH_METHOD_NO_AUTH };
    boost::asio::write(client_socket, boost::asio::buffer(bytes));
    read_request();
}

std::string Connection::ipv4_to_string(const uint8_t *ip_bytes) {
    std::ostringstream ip_stream;
    ip_stream << (int)ip_bytes[0] << "."
        << (int)ip_bytes[1] << "."
        << (int)ip_bytes[2] << "."
        << (int)ip_bytes[3];
    return ip_stream.str();
}


// In addr_end_index will save index of end address
std::string Connection::get_address(uint8_t *request_buff) {
    std::string address;
    uint8_t address_type = request_buff[ADDRESS_TYPE_INDEX];
    size_t addr_end_index = get_address_end_index(request_buff);
    if (address_type == IPV4_VERSION) {
        uint8_t buff[IPV4_ADDRESS_SIZE];
        std::copy(request_buff + BEGIN_ADDRESS_INDEX, request_buff + addr_end_index, buff);
        return ipv4_to_string(buff);
    }
    else if (address_type == DOMAIN_NAME) {
        std::vector<char> buff;
        std::copy(request_buff + BEGIN_ADDRESS_INDEX + 1, request_buff + addr_end_index, buff.data());
        return std::string(buff.data());
    } 
    else
        throw AddressTypeException();

}


size_t Connection::get_address_end_index(uint8_t* request_buff) {
    uint8_t address_type = request_buff[ADDRESS_TYPE_INDEX];
    if (address_type == IPV4_VERSION) {
        return BEGIN_ADDRESS_INDEX + IPV4_ADDRESS_SIZE;
    }
    else if (address_type == DOMAIN_NAME) {
        size_t domain_name_size = request_buff[BEGIN_ADDRESS_INDEX];
        return BEGIN_ADDRESS_INDEX + 1 + domain_name_size;
    } 
    else
        throw AddressTypeException();
}

int Connection::get_port(uint8_t* request_buff) {
    uint8_t address_type = request_buff[ADDRESS_TYPE_INDEX];
    size_t addr_end_index = get_address_end_index(request_buff);
    uint8_t port_buff[2];
    std::copy(request_buff + addr_end_index, request_buff + addr_end_index + 2, port_buff);
    return (port_buff[0] << 8) | port_buff[1];
}

void Connection::close() {
    client_socket.close();
    host_socket.close();
}


void Connection::verify_request(uint8_t* request_buff, size_t bytes) {
    if (bytes < MIN_REQUEST_SIZE || request_buff[VERSION_INDEX] != SOCKS5_VERSION || request_buff[RESERVED_BYTE_INDEX] != RESERVED_BYTE) {
        throw InvalidRequestException();
    }
    if (request_buff[REQUEST_COMMAND_CODE_INDEX] != SOCKS5_ESTABLISH_CONNECTION_COMMAND_CODE) {
        throw UnavailableCommandTypeException();
    }
    return;
}


void Connection::read_client() {
    auto self(shared_from_this());
    client_socket.async_read_some(boost::asio::buffer(client_buffer), [self, this](const boost::system::error_code& error, const size_t &bytes) {
        if (error) {
            close();
            return;
        }
    
        boost::system::error_code err;
        size_t send_bytes = boost::asio::write(host_socket, boost::asio::buffer(client_buffer, bytes), err);

        if (err) {
            close();
            return;
        }
        read_client();
    });
}



void Connection::read_host() {
    auto self(shared_from_this());
    host_socket.async_read_some(boost::asio::buffer(host_buffer), [self, this](const boost::system::error_code& error, const size_t& bytes) {
        if (error) {
            close();
            return;
        }

        boost::system::error_code err;
        size_t send_bytes = boost::asio::write(client_socket, boost::asio::buffer(host_buffer, bytes), err);

        if (err) {
            close();
            return;
        }
        read_host();
    });
}

void Connection::connect(std::string address, std::string port) {
    boost::system::error_code error;
    boost::asio::ip::tcp::resolver resolver(io_context);
    boost::asio::ip::tcp::resolver::results_type endpoints = resolver.resolve(address, port, error);
    if (error) 
        throw ResolvingAddressException();
    boost::asio::connect(host_socket, endpoints, error);
    if (error)
        throw HostConnectionException();
}

void Connection::read_request() {
    uint8_t request_buff[256];

    boost::system::error_code error;
    size_t bytes = client_socket.read_some(boost::asio::buffer(request_buff), error);
    if (error)
        close();

    int code = SUCCESS;
    try {
        verify_request(request_buff, bytes);
       
        std::string address = get_address(request_buff);
        int port = get_port(request_buff);
        connect(address, std::to_string(port));
    }
    catch (const UnavailableCommandTypeException&) {
        code = INVALID_COMMAND_TYPE;
    }
    catch (const InvalidRequestException&) {
        std::cout << "Not SOCKS5 request. Closing Connection" << std::endl;
        close();
        return;
    }
    catch (const AddressTypeException&) {
        code = INVALID_ADDRESS;
    }
    catch (const ResolvingAddressException&) {
        code = INVALID_ADDRESS;
    }
    catch (const HostConnectionException&) {
        code = HOST_UNAVAILABLE;
    }
    request_buff[RESPONSE_CODE_INDEX] = code;
    boost::asio::write(client_socket, boost::asio::buffer(request_buff, bytes), error);
    if (code != SUCCESS || error) {
        close();
        return;
    }
    // Запуск ассинхронной передачи данных
    std::cout << "Start reading" << std::endl;

    read_client();
    read_host();
}

void Connection::start() {
    try {
        read_init();
    }
    catch (const std::exception& e) {
        close();
        std::cout << e.what();
    }
}

