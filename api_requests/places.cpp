#include <iostream>
#include "places.h"
#include <boost/format.hpp>
#include <boost/beast.hpp>
#include <request_structs.h>
#include <boost/url.hpp> // Для URL-энкодинга
#include <boost/url/encoding_opts.hpp> // Для URL-энкодинга
#include <boost/url/pct_string_view.hpp>
#include <codecvt>
#include <boost/url.hpp>
#include "utils.h"

namespace http = boost::beast::http;
using namespace winforms::types;
using winforms::utils::get_str_from_json;

using winforms::PlacesRequest;


PlacesRequest::PlacesRequest(RequestHTTPS &conn) : RequestHTTPS(conn) {
}


_places PlacesRequest::request_places(const std::wstring& location) {
	std::wstring_convert<std::codecvt_utf8<wchar_t>> u8;
	
	std::string encoded_query = boost::urls::encode(u8.to_bytes(location), boost::urls::unreserved_chars);
	
	std::string target = (boost::format("%1%?q=%2%&limit=%3%&key=%4%")
		% API_URL % encoded_query % std::to_string(LIMIT) % API_KEY).str();
	auto parsed_json = get_request(ADDRESS, target);
	auto hits = parsed_json.as_object()["hits"].as_array();
	
	_places result;
	if (hits.size() > 0) {
	    for (auto hit : hits) {
			auto obj = hit.as_object();
			std::string name, country, state, city, osm = "";
			name = country = state = osm = city = "";
			if (obj.if_contains("name"))
				name = get_str_from_json(obj, "name");
			
			if (obj.if_contains("country"))
				country = get_str_from_json(obj, "country");

			if (obj.if_contains("state"))
				state = get_str_from_json(obj, "state");

			if (obj.if_contains("city"))
				city = get_str_from_json(obj, "city");

			if (obj.if_contains("osm_value"))
				osm = get_str_from_json(obj, "osm_value");

	        auto point = hit.as_object()["point"].as_object();
			place_t place(point["lat"].as_double(), point["lng"].as_double(), name, country, state, city, osm);
			std::cout << place.to_string() << std::endl << std::endl;
	        result.push_back(place);
	    }
	    // auto el = hits[0].as_object()["point"];
	}
	return result;

}
