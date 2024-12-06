#pragma once

#include <boost/asio.hpp>
#include <boost/json.hpp>
#include <boost/asio/ssl.hpp>


using boost::asio::ip::tcp;

using ssl_stream_t = boost::asio::ssl::stream<tcp::socket>;
using ssl_context_t = boost::asio::ssl::context;
using io_context_t = boost::asio::io_context;


namespace winforms {
	class RequestHTTPS
	{
	public:
		RequestHTTPS();
		RequestHTTPS(RequestHTTPS& conn);
		RequestHTTPS(io_context_t& io_context, ssl_context_t& ssl_context);

		static const int HTTPS_PORT = 443;
		boost::asio::io_context& get_context();
		ssl_context_t& get_ssl_context();
		static ssl_context_t& create_ssl_context();
		static io_context_t& create_io_context();

	private:
		io_context_t& io_context;
		ssl_context_t& ssl_context;

	protected:
		boost::json::value get_request(const std::string& address, const std::string& target);
	};
}