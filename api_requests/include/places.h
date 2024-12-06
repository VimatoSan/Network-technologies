#pragma once
#include "request.h"
#include "request_structs.h"

namespace winforms {
	using namespace types;

	class PlacesRequest : public RequestHTTPS {
	public:
		PlacesRequest(RequestHTTPS& conn);
		_places request_places(const std::wstring& location);
		inline static const std::string ADDRESS = "graphhopper.com";
		inline static const std::string API_URL = "/api/1/geocode";

	private:
		const int LIMIT = 20;
		const std::string API_KEY = "565c2384-4836-4609-a59d-1b2e336a7f70";
	};
}