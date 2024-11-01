#include "timer.h"
#include "conn_context.h"
#include <iostream>
#include <array>
#include "utils.h"

using tcp_connection::Timer;
using tcp_connection::utils::bytes_to_str;

Timer::Timer(conn_context_t &conn) : timer_(conn.io_context), conn_context(conn) {}

void Timer::start() {
	stop_flag = false;
	begin_time = std::chrono::steady_clock::now();
	tthread = std::thread(&Timer::thread_create, this);
}

void Timer::show_mid_speed() {
	auto end_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end_time - begin_time;
	conn_context.print_info("speed for all time: " + bytes_to_str(conn_context.recv_bytes / elapsed.count()) + "/s"); // средн€€ скорость
}

void Timer::show_upload_speed(int seconds) {;
	conn_context.print_info("speed per period: " + bytes_to_str(conn_context.pbytes / 3) + "/s"); // мгновенна€ скорость?;'
	conn_context.print_info("speed for all time: " + bytes_to_str(conn_context.recv_bytes / seconds) + "/s"); // средн€€ скорость
}

void Timer::thread_create() {
	int tsec = 0;
	while (!stop_flag) {
		if (tsec != 0 && tsec % 3 == 0) {
			show_upload_speed(tsec);
			if (conn_context.pbytes == 0) {
				conn_context.print_info("Nothing to read. Closing request");
				conn_context.socket.write_some(boost::asio::buffer("File not upload, but bytes stream empty"));
				conn_context.socket.close();
			}
			conn_context.mutex.lock();
			conn_context.pbytes = 0;
			conn_context.mutex.unlock();
		}
		timer_.expires_from_now(boost::asio::chrono::seconds(1));
		timer_.wait();
		
		tsec += 1;
	}
}

void Timer::stop() {
	if (!stop_flag) {
		stop_flag = true;
		show_mid_speed();

		tthread.join();
	}
}