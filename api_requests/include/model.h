#pragma once
#include <string>
#include <vector>
#include "request_structs.h"

namespace winforms {
	using namespace types;

	class RequestHTTPS;

	class Model {
	public:
		Model();
		_str_places request_places(const std::wstring& input);
		output_t request_place_description(const place_t& place);
		place_t get_chosen_place(int index);
	private:
		int selected_index;
		_places saved_places;
		RequestHTTPS* conn;
	};
}