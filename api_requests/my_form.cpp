#include "my_form.h"
#include "presenter.h"


using winforms::MyForm;


void MyForm::set_presenter(winforms::Presenter ^p) {
	this->presenter = p;
}


System::Void MyForm::searchButton_Click(System::Object^ sender, System::EventArgs^ e) {
	this->nextButton->Enabled = true;
	std::wstring input = msclr::interop::marshal_as<std::wstring>(textBox1->Text);

	presenter->show_places(input);
}


System::Void MyForm::nextButton_Click(System::Object^ sender, System::EventArgs^ e) {
	presenter->show_description(listBox->SelectedIndex);
}


void MyForm::setText(const std::wstring& text) {
	String^ managedString = msclr::interop::marshal_as<String^>(text);
	richTextBox1->Text = managedString;
}


void MyForm::addItem(const std::wstring& item) {
	String^ managedString = msclr::interop::marshal_as<String^>(item);
	 listBox->Items->Add(managedString);
}


void MyForm::clearListbox() {
	listBox->Items->Clear();
}