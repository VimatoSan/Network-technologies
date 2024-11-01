#pragma once
#include <boost/asio.hpp>
#include <iostream>

using boost::asio::ip::tcp;

namespace tcp_connection {
    typedef struct connection_struct {
        connection_struct(boost::asio::io_context& io_context, int id) : io_context(io_context), socket(io_context), id(id),
            pbytes(0), recv_bytes(0) {}
        inline void print_info(const std::string& message) {
            std::cout << "[" + std::to_string(id) + "] " + message << std::endl;
        };

        boost::asio::io_context& io_context;
        const int id;
        std::mutex mutex;
        tcp::socket socket;

        size_t pbytes; // bytes per period
        size_t recv_bytes; // all received bytes

    } conn_context_t;
}