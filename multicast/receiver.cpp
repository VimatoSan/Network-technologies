#include <array>
#include <iostream>
#include <string>
#include <vector>

#include <boost/asio.hpp>
#include "receiver.h"
#include <thread>
#include "constants.h"


using multicast::Receiver;
using namespace multicast::consts;


void Receiver::show_hosts() {
    std::cout << "---- Live copies ----" << std::endl; 
    if (this->copies.empty())
        std::cout << "None" << std::endl;
    else {
        for (auto& pair : this->copies) {
            std::cout << pair.first.address << " " << pair.first.port << std::endl;
        }
    }
    std::cout << "---------------------" << std::endl << std::endl;
}

void Receiver::delete_disconn() {
    while (true) {
       Sleep(DELAY * 1000);
       mutex.lock();
       for (auto& pair : this->copies) {
           pair.second -= DELAY;
           if (pair.second <= 0) {
               printf("Delete host: %s:%d\n", pair.first.address.c_str(), pair.first.port);
               this->copies.erase(pair.first);
               show_hosts();
           }
       }
       mutex.unlock();
    }
}

Receiver::Receiver(boost::asio::io_context& io_context,
    const boost::asio::ip::address& multicast_address,
    const boost::asio::ip::address& listen_address,
    const short multicast_port)
    : socket_(io_context) {
        boost::asio::ip::udp::endpoint listen_endpoint(listen_address, multicast_port);
        socket_.open(listen_endpoint.protocol());
        socket_.set_option(boost::asio::ip::udp::socket::reuse_address(true));
        socket_.bind(listen_endpoint);
        socket_.set_option(boost::asio::ip::multicast::join_group(multicast_address));
        std::thread t(&Receiver::delete_disconn, this);
        t.detach();

        do_receive();
    }

void Receiver::do_receive() {
        socket_.async_receive_from(
            boost::asio::buffer(data_), sender_endpoint_,
            [this](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    mutex.lock();
                    Host h = Host(sender_endpoint_.address().to_string(), sender_endpoint_.port());
                    if (this->copies.count(h) == 0) {
                        this->copies.insert(std::make_pair(h, LIVE_TIME));
                        printf("Add host: %s:%d\n", h.address.c_str(), h.port);
                        show_hosts();
                    }
                    else
                        this->copies[h] = LIVE_TIME;
                    mutex.unlock();
                }
                do_receive();

            });
    }