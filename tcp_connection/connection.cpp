#include "connection.h"
#include "constants.h"
#include "utils.h"
#include <iostream>
#include <boost/asio.hpp>
#include <array>
#include <filesystem>
#include <boost/algorithm/string.hpp>


using namespace tcp_connection::consts;
using tcp_connection::Connection;
using tcp_connection::utils::make_outfile_name;

Connection::Connection(boost::asio::io_context& io_context, int id) 
    : conn_context(io_context, id), timer(conn_context) {}

Connection::~Connection() {
    conn_context.print_info("Connection deleted");
}

void Connection::write_sock(std::string message) {
    boost::system::error_code error;
    boost::asio::write(conn_context.socket, boost::asio::buffer(message), error);
    if (error) {
        conn_context.print_info("Error while answering to client: " + error.message());
    }
}


void Connection::read_until(char del) {
    char symb = '\0';
    boost::system::error_code ec;
    buff_size = 0;
    for (buff_size; symb != del; buff_size++) {
        size_t t = conn_context.socket.read_some(boost::asio::buffer(&symb, 1), ec);
        if (!ec && t == 1)
            buffer[buff_size] = symb;
        else
            throw std::exception(ec.message().c_str());
    }
    buff_size--;
}

void Connection::read_meta() {
    boost::system::error_code ec;
    read_until(META_ENDING_SYMBOL);
   
    std::string file_metadata(buffer.data(), buff_size);
    conn_context.print_info("Meta: " + file_metadata);
    size_t pos = file_metadata.find(' ');

    file_size = std::stoul(file_metadata.substr(0, pos));
    fname = file_metadata.substr(pos + 1);
    std::vector<std::string> dirs;
    boost::split(dirs, fname, boost::is_any_of(std::string("\\/")));
    fname = dirs.back();

    if (!std::filesystem::exists(DIR_OUT_PATH)) {
        std::filesystem::create_directory(DIR_OUT_PATH);
    }

    fname = make_outfile_name(fname, 0);
}

tcp::socket& Connection::get_socket() {
    return conn_context.socket;
}

void Connection::close() {
    fstream.close();
    conn_context.socket.close();
    timer.stop();
}

void Connection::accept_handle() {
    conn_context.print_info("Connection accepted");
    try { 
        read_meta();
        fstream.open(consts::DIR_OUT_PATH + fname, std::ios::binary);
    } 
    catch (const std::exception &e) {
        conn_context.print_info(std::string("Error while reading meta: ") + e.what());
        close();
        return;
    }
    timer.start();
    boost::system::error_code ec;
    while (conn_context.recv_bytes < file_size) {
        if (file_size - conn_context.recv_bytes < consts::BUFFER_SIZE) {
            buff_size = file_size - conn_context.recv_bytes;
        }
        else {
            buff_size = consts::BUFFER_SIZE;
        }
        boost::asio::read(conn_context.socket, boost::asio::buffer(buffer, buff_size), ec);

        if (!ec) {
            conn_context.mutex.lock();
            conn_context.pbytes += buff_size;
            conn_context.mutex.unlock();
            conn_context.recv_bytes += buff_size;
            write_file();
        }
        else {
            break;
        }
    }
    if (conn_context.recv_bytes == file_size) {
        conn_context.print_info("Successfully uploaded!");
        if (conn_context.socket.is_open())
            conn_context.socket.write_some(boost::asio::buffer("Successfully uploaded!"));
    }
    else {
        conn_context.print_info("Required " + std::to_string(file_size - conn_context.recv_bytes) + " bytes");
        if (conn_context.socket.is_open())
            conn_context.socket.write_some(boost::asio::buffer("Bad ending"));
    }
    close();
}

void Connection::write_file() {
    try {
        fstream.write(buffer.data(), buff_size);
    }
    catch (const std::exception& e) {
        conn_context.print_info("Error while write in the file: " + std::string(e.what()));
    }
}