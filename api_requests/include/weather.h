#pragma once
#include "request.h"
#include "request_structs.h"

namespace winforms {
	using namespace types;

	class WeatherRequest : public RequestHTTPS {
	public:
		WeatherRequest(RequestHTTPS& conn);
		weather_t get_weather(const place_t& point);
		inline static const std::string ADDRESS = "api.openweathermap.org";
		inline static const std::string API_URL = "/data/2.5/weather";

	private:
		const std::string api_key = "d70d7513397a4bc368fc8939b2c395e4";
	};
}
