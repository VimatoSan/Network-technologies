#include <iostream>
#include <cmath>
#include "weather.h"
#include <boost/format.hpp>
#include "utils.h"

using winforms::WeatherRequest;
using namespace winforms::types;
using  winforms::utils::double_to_str;
using  winforms::utils::convert_pressure;


WeatherRequest::WeatherRequest(RequestHTTPS& conn) : RequestHTTPS(conn) {}


weather_t WeatherRequest::get_weather(const place_t& point) {
	std::string target = (boost::format("%1%?lat=%2%&lon=%3%&appid=%4%&units=metric")
		% API_URL % double_to_str(point.lat) % double_to_str(point.lng) % api_key).str();
	auto parsed_json = get_request(ADDRESS, target);
	auto json_obj = parsed_json.as_object();
	weather_t w;

	w.status = json_obj["weather"].as_array()[0].as_object()["main"].as_string();
	w.temp = json_obj["main"].as_object()["temp"].as_double();
	w.feels_like = json_obj["main"].as_object()["feels_like"].as_double();
	w.pressure = json_obj["main"].as_object()["pressure"].as_int64(); // value * 10^2 Pascals
	w.wind_speed = json_obj["wind"].as_object()["speed"].as_double();

	w.pressure = convert_pressure(w.pressure * 100);
	w.wind_speed = std::round(w.wind_speed * 10) / 10;
	w.temp = std::round(w.temp * 10) / 10;
	w.feels_like = std::round(w.feels_like * 10) / 10;

	return w;
}

