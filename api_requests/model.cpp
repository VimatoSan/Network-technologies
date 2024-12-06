#include <boost/asio.hpp>
#include <iostream>

#include "model.h"
#include "places.h"
#include "weather.h"
#include "request.h"
#include "geo_id.h"
#include "description.h"

using winforms::Model;
using namespace winforms::types;


Model::Model() : conn(new RequestHTTPS()) {
}


place_t Model::get_chosen_place(int index) {
	selected_index = index;
	return saved_places.at(index);
}

output_t Model::request_place_description(const place_t& place) {
	WeatherRequest wr(*conn);
	std::future<weather_t> weather_request = std::async(std::launch::async, &WeatherRequest::get_weather, &wr, place);
	std::vector<std::future<description_t>> descr_requests;

	GeoIdRequest gir(*conn);
	DescriptionRequest dr(*conn);
	_geo_ids interest_places = gir.get_places(place);

	
	boost::system::error_code err;
	boost::asio::thread_pool pool(interest_places.size());
	int counter = 0;
	for (auto id : interest_places) {
		std::packaged_task<description_t()> packaged_task([&dr, id]() {
			return dr.get_description(id);
		});
		descr_requests.push_back(packaged_task.get_future()); 

		boost::asio::post(pool, [task = std::move(packaged_task)]() mutable {
			task(); 
		}); 
	}
	output_t output;

	output.weather = weather_request.get();
	pool.join();
	for (auto& future : descr_requests) {
		description_t d = future.get();
		std::cout << d.text;
		if (d.text != "") {
			output.interest_places.push_back(d);
		}
	}
	return output;
}


_str_places Model::request_places(const std::wstring& input) {
	try {
		PlacesRequest pr(*conn);
		_places places = pr.request_places(input);
		saved_places = places;
		_str_places result;
		for (auto place : places) {
			std::cout << place.to_string() << std::endl << std::endl;
			result.push_back(place.to_string());
		}
		return result;
	}
	catch (std::exception& e) {
		std::cout << e.what();
	}
}