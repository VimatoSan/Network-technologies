#pragma once
#include "my_form.h"

#include "model.h"
#include <string>
#include "request_structs.h"

using namespace System;

namespace winforms {
	ref class Presenter {

	public:
		Presenter(winforms::MyForm^ form, Model& model);
		void show_places(const std::wstring& input);
		void show_description(int index);

	private:
		winforms::MyForm^ view;
		Model& model;
	};
}