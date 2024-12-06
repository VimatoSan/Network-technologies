#pragma once
#include <string>
//#include "presenter.h"
#include <msclr\marshal_cppstd.h>


namespace winforms {
	ref class Presenter;


	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	public:
		void set_presenter(Presenter ^p);
		void setText(const std::wstring& text);
		void addItem(const std::wstring& item);
		void clearListbox();
		MyForm(void) {
			InitializeComponent();
		//
		//TODO: Add the constructor code here
		//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm() {
			if (components) {
				delete components;
			}
		}
	private: 
		Presenter^ presenter;
		System::Windows::Forms::Button^ searchButton;
		System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Button^ nextButton;



	private: System::Windows::Forms::ListBox^ listBox;
	private: System::Windows::Forms::RichTextBox^ richTextBox1;


		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->searchButton = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->nextButton = (gcnew System::Windows::Forms::Button());
			this->listBox = (gcnew System::Windows::Forms::ListBox());
			this->richTextBox1 = (gcnew System::Windows::Forms::RichTextBox());
			this->SuspendLayout();
			// 
			// searchButton
			// 
			this->searchButton->Location = System::Drawing::Point(609, 26);
			this->searchButton->Name = L"searchButton";
			this->searchButton->Size = System::Drawing::Size(115, 31);
			this->searchButton->TabIndex = 0;
			this->searchButton->Text = L"Найти";
			this->searchButton->UseVisualStyleBackColor = true;
			this->searchButton->Click += gcnew System::EventHandler(this, &MyForm::searchButton_Click);
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(27, 30);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(517, 22);
			this->textBox1->TabIndex = 1;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &MyForm::textBox1_TextChanged);
			// 
			// nextButton
			// 
			this->nextButton->Enabled = false;
			this->nextButton->Location = System::Drawing::Point(609, 86);
			this->nextButton->Name = L"nextButton";
			this->nextButton->Size = System::Drawing::Size(115, 31);
			this->nextButton->TabIndex = 3;
			this->nextButton->Text = L"Далее";
			this->nextButton->UseVisualStyleBackColor = true;
			this->nextButton->Click += gcnew System::EventHandler(this, &MyForm::nextButton_Click);
			// 
			// listBox
			// 
			this->listBox->FormattingEnabled = true;
			this->listBox->HorizontalScrollbar = true;
			this->listBox->ItemHeight = 16;
			this->listBox->Location = System::Drawing::Point(27, 84);
			this->listBox->Name = L"listBox";
			this->listBox->ScrollAlwaysVisible = true;
			this->listBox->Size = System::Drawing::Size(517, 180);
			this->listBox->TabIndex = 4;
			// 
			// richTextBox1
			// 
			this->richTextBox1->Location = System::Drawing::Point(27, 301);
			this->richTextBox1->Name = L"richTextBox1";
			this->richTextBox1->ReadOnly = true;
			this->richTextBox1->Size = System::Drawing::Size(701, 335);
			this->richTextBox1->TabIndex = 6;
			this->richTextBox1->Text = L"";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(8, 16);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(785, 670);
			this->Controls->Add(this->richTextBox1);
			this->Controls->Add(this->listBox);
			this->Controls->Add(this->nextButton);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->searchButton);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void searchButton_Click(System::Object^ sender, System::EventArgs^ e);
	
	private: System::Void searchButton_Click_1(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void textBox1_TextChanged(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void button1_Click(System::Object^ sender, System::EventArgs^ e) {
	}
	private: System::Void nextButton_Click(System::Object^ sender, System::EventArgs^ e);

};
}
