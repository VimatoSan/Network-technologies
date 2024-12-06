#pragma once

#include <iostream>
#include "geo_id.h"
#include <boost/format.hpp>
#include "request_structs.h"
#include "utils.h"

using winforms::GeoIdRequest;
using winforms::utils::double_to_str;
using namespace winforms::types;

GeoIdRequest::GeoIdRequest(RequestHTTPS& conn) : RequestHTTPS(conn) {
}


_geo_ids GeoIdRequest::get_places(const place_t& point) {
	std::string target = (boost::format("%1%?radius=%2%&lon=%3%&lat=%4%&limit=%5%&format=json&apikey=%6%")
		% API_URL % std::to_string(DEFAULT_RADIUS) % double_to_str(point.lng)
		% double_to_str(point.lat) % std::to_string(DEFAULT_LIMIT) % api_key).str();
	auto parsed_json = get_request(ADDRESS, target);
	auto hits = parsed_json.as_array();
	_geo_ids object_ids;
	for (auto hit : hits) {
		geo_id_t ob;
		ob.id = hit.as_object()["xid"].as_string().c_str();
		object_ids.push_back(ob);
	}
	return object_ids;
}


