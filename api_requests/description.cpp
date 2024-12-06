#pragma once

#include <iostream>
#include "description.h"
#include <boost/format.hpp>

using winforms::DescriptionRequest;
using namespace winforms::types;

DescriptionRequest::DescriptionRequest(RequestHTTPS& conn) : RequestHTTPS(conn) {}


description_t DescriptionRequest::get_description(const geo_id_t& place) {
	std::cout << place.id << std::endl;
	std::string target = (boost::format("%1%%2%?apikey=%3%")
		% API_URL % place.id % api_key).str();
	auto json_req = get_request(ADDRESS, target);
	auto json_obj = json_req.as_object();
	description_t d;
	d.text = "";
	
	if (json_obj.if_contains("info") && json_obj["info"].as_object().if_contains("descr")) {
		d.text = json_obj["info"].as_object()["descr"].as_string().c_str();
	}
	else if (json_obj.if_contains("wikipedia_extracts") && json_obj["wikipedia_extracts"].as_object().if_contains("text")) {
		d.text = json_obj["wikipedia_extracts"].as_object()["text"].as_string().c_str();
	}

	return d;
}
