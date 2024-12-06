#include "presenter.h"
#include "model.h"
#include "my_form.h"

using namespace winforms;

[STAThreadAttribute]
int main(array<String^>^ args) {
	Application::EnableVisualStyles();
	winforms::MyForm form;
	Model model;
	Presenter p(%form, model);
	Application::Run(% form);

	return 0;
}