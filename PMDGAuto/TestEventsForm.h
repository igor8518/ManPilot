#pragma once
#include "PMDGEventsArray.h"
#include <windows.h>
namespace PMDGAuto {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// —водка дл€ TestEventsForm
	/// </summary>
	public ref class TestEventsForm : public System::Windows::Forms::Form
	{
	public:
		TestEventsForm(void)
		{
			InitializeComponent();
			//
			//TODO: добавьте код конструктора
			//
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~TestEventsForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::ComboBox^  evemtList;
	private: System::Windows::Forms::Label^  mouseEvent;
	public: System::Windows::Forms::TextBox^  outBox;
	public: System::Windows::Forms::TextBox^  CurrentChangedBox;
	public: System::Windows::Forms::ListBox^  ChangedVarsList;
	public:
	public:

	protected:

	protected:

	private:
		/// <summary>
		/// ќб€зательна€ переменна€ конструктора.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// “ребуемый метод дл€ поддержки конструктора Ч не измен€йте 
		/// содержимое этого метода с помощью редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->evemtList = (gcnew System::Windows::Forms::ComboBox());
			this->mouseEvent = (gcnew System::Windows::Forms::Label());
			this->outBox = (gcnew System::Windows::Forms::TextBox());
			this->CurrentChangedBox = (gcnew System::Windows::Forms::TextBox());
			this->ChangedVarsList = (gcnew System::Windows::Forms::ListBox());
			this->SuspendLayout();
			// 
			// evemtList
			// 
			this->evemtList->DropDownStyle = System::Windows::Forms::ComboBoxStyle::DropDownList;
			this->evemtList->FormattingEnabled = true;
			this->evemtList->Location = System::Drawing::Point(12, 12);
			this->evemtList->Name = L"evemtList";
			this->evemtList->Size = System::Drawing::Size(382, 21);
			this->evemtList->TabIndex = 0;
			this->evemtList->DropDown += gcnew System::EventHandler(this, &TestEventsForm::evemtList_DropDown);
			// 
			// mouseEvent
			// 
			this->mouseEvent->AutoSize = true;
			this->mouseEvent->BackColor = System::Drawing::SystemColors::Control;
			this->mouseEvent->Cursor = System::Windows::Forms::Cursors::Hand;
			this->mouseEvent->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 30, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->mouseEvent->ForeColor = System::Drawing::SystemColors::HotTrack;
			this->mouseEvent->Location = System::Drawing::Point(400, 0);
			this->mouseEvent->Name = L"mouseEvent";
			this->mouseEvent->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->mouseEvent->Size = System::Drawing::Size(44, 46);
			this->mouseEvent->TabIndex = 1;
			this->mouseEvent->Text = L"+";
			this->mouseEvent->MouseClick += gcnew System::Windows::Forms::MouseEventHandler(this, &TestEventsForm::mouseEvent_MouseClick);
			// 
			// outBox
			// 
			this->outBox->BackColor = System::Drawing::SystemColors::Control;
			this->outBox->BorderStyle = System::Windows::Forms::BorderStyle::None;
			this->outBox->Location = System::Drawing::Point(12, 39);
			this->outBox->Multiline = true;
			this->outBox->Name = L"outBox";
			this->outBox->Size = System::Drawing::Size(432, 494);
			this->outBox->TabIndex = 2;
			// 
			// CurrentChangedBox
			// 
			this->CurrentChangedBox->Location = System::Drawing::Point(450, 39);
			this->CurrentChangedBox->Multiline = true;
			this->CurrentChangedBox->Name = L"CurrentChangedBox";
			this->CurrentChangedBox->Size = System::Drawing::Size(345, 222);
			this->CurrentChangedBox->TabIndex = 3;
			// 
			// ChangedVarsList
			// 
			this->ChangedVarsList->FormattingEnabled = true;
			this->ChangedVarsList->Location = System::Drawing::Point(450, 268);
			this->ChangedVarsList->Name = L"ChangedVarsList";
			this->ChangedVarsList->Size = System::Drawing::Size(345, 264);
			this->ChangedVarsList->TabIndex = 4;
			this->ChangedVarsList->DrawItem += gcnew System::Windows::Forms::DrawItemEventHandler(this, &TestEventsForm::ChangedVarsList_DrawItem);
			// 
			// TestEventsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(807, 545);
			this->Controls->Add(this->ChangedVarsList);
			this->Controls->Add(this->CurrentChangedBox);
			this->Controls->Add(this->outBox);
			this->Controls->Add(this->mouseEvent);
			this->Controls->Add(this->evemtList);
			this->Name = L"TestEventsForm";
			this->Text = L"Test PMDG events";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void evemtList_DropDown(System::Object^  sender, System::EventArgs^  e) {
		int lastIndex = evemtList->SelectedIndex;
		evemtList->Items->Clear();
		for (int i = 0; i < PMDGEventArray.size(); i++)
		{
			evemtList->Items->Add(gcnew String(PMDGEventArray[i].szEventName));
		}
		evemtList->SelectedIndex = lastIndex;
	}

	private: System::Void mouseEvent_Click(System::Object^  sender, System::EventArgs^  e) {
	}

	private: System::Void mouseEvent_MouseClick(System::Object^  sender, System::Windows::Forms::MouseEventArgs^  e) {
		SendMouseEvent(MOUSE_FLAG_LEFTSINGLE);
	}

			 // Send mouse event to the PMDG
	private: HRESULT SendMouseEvent(int eventMouse);
	private: System::Void ChangedVarsList_DrawItem(System::Object^  sender, System::Windows::Forms::DrawItemEventArgs^  e) {
		ChangedVarsList->ForeColor.FromArgb(255,127,127);
	}
};
}


