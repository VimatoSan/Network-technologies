#include <iostream>
#include "utils.h"
#include <filesystem>
#include <fstream>
#include <array>
#include "constants.h"

using tcp_connection::consts::DIR_OUT_PATH;

namespace tcp_connection::utils {
	std::string bytes_to_str(size_t bytes) {
		std::array<std::string, 4> pref{ "B", "KB", "MB", "GB" };
		int i = 0;
		size_t rbytes = 0;
		for (i; i < pref.size() && bytes > 1024; i++) {
			rbytes += bytes % 1024 * std::pow(1024, i);;
			bytes /= 1024;
		}
		int fraction = round((double)rbytes / std::pow(1024, i) * 100); // Получаем значение после запятой, округленное до сотых
		if (fraction > 0)
			return  std::to_string(bytes) + "." + std::to_string(fraction) + pref[i];
		else
			return std::to_string(bytes) + pref[i];
	}


	std::string make_outfile_name(const std::string& filename, int acc) {
		std::string fname;
		if (acc > 0)
			fname = "(" + std::to_string(acc) + ")" + filename;
		else
			fname = filename;
		if (std::filesystem::exists(DIR_OUT_PATH + fname))
			return tcp_connection::utils::make_outfile_name(filename, ++acc);
		else
			return fname;
	}

	size_t get_fsize(std::ifstream& fstream) {
		fstream.seekg(0, std::ios::end);
		size_t size = fstream.tellg();
		fstream.seekg(0, std::ios::beg);

		return size;
	}
}