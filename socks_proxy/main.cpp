#include <iostream>
#include <boost/asio.hpp>
#include "proxy_server.h"

using boost::asio::ip::tcp;


int main(int argc, char* argv[])
{   
    setlocale(LC_ALL, "Russian");
    if (argc != 2) {
        std::cout << "Incorrect parameters. required <program_name> <port>";
        return 1;
    }
    int port = atoi(argv[1]);
    ProxyServer(port).start();
    return 0;
}