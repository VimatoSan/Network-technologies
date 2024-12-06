#pragma once

#include "request.h"
#include "request_structs.h"

namespace winforms {
	using namespace types;
	class DescriptionRequest : public RequestHTTPS {
	public:
		DescriptionRequest(RequestHTTPS& conn);
		description_t get_description(const geo_id_t& place);
		inline static const std::string ADDRESS = "api.opentripmap.com";
		inline static const std::string API_URL = "/0.1/ru/places/xid/";

	private:
		const std::string api_key = "5ae2e3f221c38a28845f05b617df0086810cb56892002b1a0cd81c0d";
	};
}


