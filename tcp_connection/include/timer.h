#pragma once
#include <boost/asio/steady_timer.hpp>
#include "conn_context.h"
#include <chrono>

std::string bytes_to_str(size_t bytes);


class Timer {
public:
	Timer(conn_context_t &conn_context);
	void stop();
	void start();
private:
	void print(const std::string& message);
	void th_create();
	void show_upload_speed(int seconds);
	void show_mid_speed();

	std::thread tthread;
	std::chrono::steady_clock::time_point begin_time;
	boost::asio::steady_timer timer_;

	conn_context_t &conn_context;
	bool stop_flag;
};