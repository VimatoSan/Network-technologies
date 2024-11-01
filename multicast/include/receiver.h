#pragma once
#include <boost/asio.hpp>
#include <unordered_map>
#include "host.h"

using multicast::structs::Host;

namespace multicast {
	class Receiver {
	public:
		Receiver(boost::asio::io_context& io_context,
			const boost::asio::ip::address& multicast_address,
			const boost::asio::ip::address& listen_address,
			const short multicast_port);

	private:
		void show_hosts();
		void delete_disconn();
		void do_receive();

		std::mutex mutex;
		std::unordered_map<Host, int> copies;
		boost::asio::ip::udp::socket socket_;
		boost::asio::ip::udp::endpoint sender_endpoint_;
		std::array<char, 1024> data_;
	};
}