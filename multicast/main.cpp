#include "receiver.h"
#include "sender.h"
#include <iostream>
#include "app.h"
#include <boost/asio.hpp>
#include "constants.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Not enough args. Required <program> <multicast_address>" << std::endl;
        return 1;
    }

    try {
        std::string addr(argv[1]);
        multicast::App app(addr, multicast::consts::port);
        app.start();
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}