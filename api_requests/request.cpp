#include "request.h"
#include <boost/beast.hpp>
#include <boost/format.hpp>

#include <iostream>

namespace http = boost::beast::http;
namespace ssl = boost::asio::ssl;
using boost::asio::ip::tcp;
using winforms::RequestHTTPS;


RequestHTTPS::RequestHTTPS() : io_context(create_io_context()), ssl_context(create_ssl_context()) {}

RequestHTTPS::RequestHTTPS(RequestHTTPS& conn) : io_context(conn.get_context()), ssl_context(conn.get_ssl_context()) {}

RequestHTTPS::RequestHTTPS(io_context_t& io_context, ssl_context_t& ssl_context) : io_context(io_context), ssl_context(ssl_context) {}


io_context_t& RequestHTTPS::get_context() {
    return io_context;
}


ssl_context_t& RequestHTTPS::create_ssl_context() {
    static ssl_context_t ctx(boost::asio::ssl::context::tls_client);
    ctx.set_default_verify_paths();
    return ctx;
}


io_context_t& RequestHTTPS::create_io_context() {
    static io_context_t io;

    return io;
}


ssl_context_t& RequestHTTPS::get_ssl_context() {
    return ssl_context;
}


boost::json::value RequestHTTPS::get_request(const std::string& address, const std::string& target) {

    boost::system::error_code error;

    tcp::resolver resolver(io_context);

    tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(HTTPS_PORT), error);
    std::cout << address;
    ssl_context.set_default_verify_paths();
    ssl_stream_t stream_(io_context, ssl_context);
    boost::asio::connect(stream_.next_layer(), endpoints, error);

    if (error) {
        std::cout << "Error connecting";
    }
    stream_.handshake(ssl::stream_base::client);
    http::request<http::string_body> req{ http::verb::get, target, 11 };
    req.set(http::field::host, address);
    req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
    http::write(stream_, req, error);

    if (error) {
        throw std::runtime_error(error.message());
    }

    boost::beast::flat_buffer buffer;
    http::response<http::string_body> res;
    http::read(stream_, buffer, res, error);
    if (error) {
        throw std::runtime_error(error.message());
    }

    return boost::json::parse(res.body());
}
