#pragma once
#include <string>
#include <vector>

namespace winforms::types {
	typedef struct place {
		place(double lat, double lng, const std::string& name,
			const std::string& country, const std::string& state,
			const std::string& city, const std::string& osm);
		double lat;
		double lng;
		std::string name;
		std::string country;
		std::string state;
		std::string city;
		std::string osm_value;

		std::string to_string();
	} place_t;


	using _places = std::vector<place_t>;


	typedef struct weather {
		double temp;
		double feels_like;
		std::string status;
		double wind_speed;
		int pressure;

		std::wstring to_wstring();
	} weather_t;



	typedef struct geo_id {
		std::string id;
	} geo_id_t;


	using _geo_ids = std::vector<geo_id_t>;


	typedef struct description {
		std::string text;
	} description_t;

	typedef struct output_data {
		std::vector<description_t> interest_places;
		weather_t weather;

		std::wstring to_wstring();
	} output_t;


	using _str_places = std::vector<std::string>;
}