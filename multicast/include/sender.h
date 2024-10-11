#pragma once
#include <boost/asio.hpp>

class Sender {
public:
    Sender(boost::asio::io_context& io_context,
        const boost::asio::ip::address& multicast_address, 
        const short multicast_port);


private:
    void do_send();
    void do_timeout();
    boost::asio::ip::udp::endpoint endpoint_;
    boost::asio::ip::udp::socket socket_;
    boost::asio::steady_timer timer_;
};