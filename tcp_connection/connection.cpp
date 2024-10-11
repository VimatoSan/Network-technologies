#include "connection.h"
#include "constants.h"
#include <iostream>
#include <boost/asio.hpp>
#include <array>
#include <filesystem>
#include <boost/algorithm/string.hpp>


Connection::Connection(boost::asio::io_context& io_context, int id) 
    : conn_context(io_context, id), timer(conn_context) {}

Connection::~Connection() {
    print("Connection deleted");
}

void Connection::print(const std::string& message) {
    std::cout << "[" + std::to_string(conn_context.id) + "] " + message << std::endl;
}

void Connection::error_handle(std::string error_message) {
    print("Interrupt connection thread. Cause: " + error_message);
    close();
}

void Connection::write_sock(std::string message) {
    boost::system::error_code error;
    boost::asio::write(conn_context.socket, boost::asio::buffer(message), error);
    if (error) {
        print("Error while answering to client: " + error.message());
    }
}

void Connection::make_outfile(const std::string &filename, int acc) {
    try {
        if (std::filesystem::exists(DIR_OUT_PATH + filename)) {
            make_outfile("(" + std::to_string(acc) + ")" + fname, ++acc);
        }
        else 
            fstream.open(DIR_OUT_PATH + filename, std::ios::binary);
    }
    catch (const std::exception& e) {
        error_handle("Error while openning file: " + std::string(e.what()));
    }
}

int Connection::read_until(char del) {
    char symb = '\0';
    boost::system::error_code ec;
    buff_size = 0;
    for (buff_size; symb != del; buff_size++) {
        size_t t = conn_context.socket.read_some(boost::asio::buffer(&symb, 1), ec);
        if (!ec && t == 1)
            buffer[buff_size] = symb;
        else 
            return 1;
    }
    buff_size--;
        
    return 0;
}

int Connection::read_meta() {
    boost::system::error_code ec;
    if (read_until(META_ENDING_SYMBOL)) {
        return 1;
    }
   
    std::string file_metadata(buffer.data(), buff_size);
    print("Meta: " + file_metadata);
    size_t pos = file_metadata.find(' ');

    file_size = std::stoul(file_metadata.substr(0, pos));
    fname = file_metadata.substr(pos + 1);
    std::vector<std::string> dirs;
    boost::split(dirs, fname, boost::is_any_of(std::string("\\/")));
    fname = dirs.back();

    if (!std::filesystem::exists("uploads")) {
        std::filesystem::create_directory("uploads");
    }

    make_outfile(fname, 0);
    
    return 0;
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
    print("Connection accepted");
    if (read_meta()) {
        print("Error while reading meta");
        return;
    }
    timer.start();
    boost::system::error_code ec;
    while (conn_context.recv_bytes < file_size) {
        if (file_size - conn_context.recv_bytes < BUFFER_SIZE) {
            buff_size = file_size - conn_context.recv_bytes;
        }
        else {
            buff_size = BUFFER_SIZE;
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
        print("Successfully uploaded!");
        if (conn_context.socket.is_open())
            conn_context.socket.write_some(boost::asio::buffer("Successfully uploaded!"));
    }
    else {
        print("Required " + std::to_string(file_size - conn_context.recv_bytes) + " bytes");
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
        print("Error while write in the file: " + std::string(e.what()));
    }
}