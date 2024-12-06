#pragma once

#include "request.h"
#include "request_structs.h"


namespace winforms {
	using namespace types;
	class GeoIdRequest : public RequestHTTPS {
	public:
		GeoIdRequest(RequestHTTPS& conn);
		_geo_ids get_places(const place_t& point);
		inline static const std::string ADDRESS = "api.opentripmap.com";
		inline static const std::string API_URL = "/0.1/ru/places/radius";


	private:
		const int DEFAULT_RADIUS = 100000;
		const int DEFAULT_LIMIT = 10;
		const std::string api_key = "5ae2e3f221c38a28845f05b617df0086810cb56892002b1a0cd81c0d";
	};
}