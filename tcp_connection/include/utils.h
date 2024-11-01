#pragma once
#include <string>

namespace tcp_connection::utils {
	std::string make_outfile_name(const std::string& filename, int acc);
	size_t get_fsize(std::ifstream& fstream);
	std::string bytes_to_str(size_t bytes);
}