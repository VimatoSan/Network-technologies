#include "timer.h"
#include "conn_context.h"
#include <iostream>
#include <array>


std::string bytes_to_str(size_t bytes) {
	std::array<std::string, 4> pref{ "B", "KB", "MB", "GB" };
	int i = 0;
	for (i; i < pref.size() && bytes > 1024; i++) {
		bytes /= 1024;
	}
	return std::to_string(bytes) + pref[i];
}

void Timer::print(const std::string& message) {
	std::cout << "[" + std::to_string(conn_context.id) + "] " + message << std::endl;
}

Timer::Timer(conn_context_t &conn) : timer_(conn.io_context), conn_context(conn) {}

void Timer::start() {
	stop_flag = false;
	begin_time = std::chrono::steady_clock::now();
	tthread = std::thread(&Timer::th_create, this);
	// t.detach();
}

void Timer::show_mid_speed() {
	auto end_time = std::chrono::steady_clock::now();
	std::chrono::duration<double> elapsed = end_time - begin_time;
	print("speed for all time: " + bytes_to_str(conn_context.recv_bytes / elapsed.count()) + "/s"); // средн€€ скорость
}

void Timer::show_upload_speed(int seconds) {;
	print("speed per period: " + bytes_to_str(conn_context.pbytes / 3) + "/s"); // мгновенна€ скорость?;'
	print("speed for all time: " + bytes_to_str(conn_context.recv_bytes / seconds) + "/s"); // средн€€ скорость
}

void Timer::th_create() {
	int tsec = 0;
	while (!stop_flag) {
		if (tsec != 0 && tsec % 3 == 0) {
			show_upload_speed(tsec);
			if (conn_context.pbytes == 0) {
				print("Nothing to read. Closing request");
				// «акрывать сокет
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