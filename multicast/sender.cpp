#include <iostream>
#include <boost/asio.hpp>
#include "sender.h"

using namespace boost::asio;


Sender::Sender(boost::asio::io_context& io_context,
        const boost::asio::ip::address& multicast_address, const short multicast_port)
        : endpoint_(multicast_address, multicast_port),
        socket_(io_context, endpoint_.protocol()),
        timer_(io_context) {
        do_send();
    }

void Sender::do_send() {
        socket_.async_send_to(
            boost::asio::buffer(""), endpoint_,
            [this](boost::system::error_code ec, std::size_t /*length*/)
            {
                if (!ec)
                    do_timeout();
            });
    }

void Sender::do_timeout() {
        timer_.expires_after(std::chrono::seconds(1));
        timer_.async_wait(
            [this](boost::system::error_code ec) {
                if (!ec)
                    do_send();
            });
    }