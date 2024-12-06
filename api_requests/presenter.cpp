#include "presenter.h"
#include "my_form.h"
#include "model.h"
#include <iostream>
#include <codecvt>

using winforms::Presenter;
using namespace winforms::types;



Presenter::Presenter(winforms::MyForm^ form, Model& model) : view(form), model(model) {
	view->set_presenter(this);
}

void Presenter::show_description(int index) {
	place_t selected_place = model.get_chosen_place(index);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	std::wstring text = converter.from_bytes(selected_place.to_string()) + L"\n\n";
	text += model.request_place_description(selected_place).to_wstring();
	view->setText(text);
}


void Presenter::show_places(const std::wstring& input) {
	_str_places _places = model.request_places(input);
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;

	view->clearListbox();
	for (auto place : _places) {
		view->addItem(converter.from_bytes(place));
	}
}
