#include "request_structs.h"
#include <sstream>
#include <codecvt>

#include <iomanip>


namespace winforms::types {
	place_t::place(double lat, double lng, const std::string& name,
		const std::string& country, const std::string& state,
		const std::string& city, const std::string& osm) {
		this->lat = lat;
		this->lng = lng;
		this->name = name;
		this->country = country;
		this->state = state;
		this->city = city;
		this->osm_value = osm;
	}


	std::string to_string_with_precision(double value, int precision = 1) {
		std::ostringstream oss;
		oss << std::fixed << std::setprecision(precision) << value;
		return oss.str();
	}


	std::string place_t::to_string() {
		std::string result = std::to_string(lat) + " " + std::to_string(lng);
		if (name != "")
			result += ", " + name;
		if (country != "")
			result += ", " + country;
		if (state != "")
			result += ", " + state;
		if (city != "")
			result += ", " + city;
		if (osm_value != "")
			result += ", " + osm_value;

		return result;
		/*return std::to_string(lat) + " " + std::to_string(lng) + ", " +
			name + ", " + country + ", " + state + ", " + ", " + city + ", " + osm_value;*/
	}


	std::wstring output_t::to_wstring() {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring result = weather.to_wstring() + L"\n\n-----------------\n\n";;

		for (auto d : interest_places) {
			result += converter.from_bytes(d.text) + L"\n\n-----------------\n\n";
		}
		return result;
	}


	std::wstring weather_t::to_wstring() {
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring result = L"WEATHER:\nTemp: " + converter.from_bytes(to_string_with_precision(temp)) + L"°C\n" +
			L"Feels like: " + converter.from_bytes(to_string_with_precision(feels_like)) + L"°C\n" +
			L"Status: " + converter.from_bytes(status) + L"\n" +
			L"Pressure: " + converter.from_bytes(std::to_string(pressure)) + L" mm\n" +
			L"Wind speed: " + converter.from_bytes(to_string_with_precision(wind_speed)) + L" m/s";

		return result;
	}
}