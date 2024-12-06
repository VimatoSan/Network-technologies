#pragma once
#include <string>
#include <boost/json.hpp>

namespace winforms::utils {
	std::string double_to_str(double value);

	std::string get_str_from_json(const boost::json::object& json, const std::string& key);

	int convert_pressure(int pascals);
}