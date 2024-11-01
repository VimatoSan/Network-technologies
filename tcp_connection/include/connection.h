#pragma once
#include <boost/asio.hpp>
#include <fstream>
#include "timer.h"
#include "conn_context.h"
#include "constants.h"


// #define BUFFER_SIZE 4096

using boost::asio::ip::tcp;

namespace tcp_connection {
    class Connection {

    public:
        Connection(boost::asio::io_context& io_context, int id);
        ~Connection();
        std::array<char, consts::BUFFER_SIZE> buffer;

        void accept_handle();
        tcp::socket& get_socket();

    private:
        // const std::string DIR_OUT_PATH = "uploads/";
        conn_context_t conn_context;

        size_t buff_size;
        size_t file_size;

        std::string fname;

        std::ofstream fstream; // file stream
        Timer timer;
        void read_until(char del);
        // void print(const std::string& message);
        //void error_handle(std::string error_message);
        void write_sock(std::string message);

        // void make_outfile(const std::string& filename, int acc);
        void close();
        void read_meta();
        void write_file();
    };
}