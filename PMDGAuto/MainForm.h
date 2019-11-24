#pragma once

#include <windows.h>
#include "FSLink.h"
#include "TestEventsForm.h"

namespace PMDGAuto {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Reflection;
	[assembly:AssemblyVersionAttribute("0.1.0611.1")];
	
	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MainForm : public System::Windows::Forms::Form
	{
	public:
		TestEventsForm^ testEventsForm = gcnew TestEventsForm();;
		// Link to the mainForm
		
		static MainForm^ PMainForm;
		MainForm(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
			PMainForm = this;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: 
		System::Windows::Forms::Button^  ConnectButton;
	private: System::Windows::Forms::Button^  testEventsButton;

	
	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;
		
		
	private:
#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->ConnectButton = (gcnew System::Windows::Forms::Button());
			this->testEventsButton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// ConnectButton
			// 
			this->ConnectButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(125)),
				static_cast<System::Int32>(static_cast<System::Byte>(125)));
			this->ConnectButton->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->ConnectButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
				static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(30)));
			this->ConnectButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)),
				static_cast<System::Int32>(static_cast<System::Byte>(80)), static_cast<System::Int32>(static_cast<System::Byte>(80)));
			this->ConnectButton->FlatStyle = System::Windows::Forms::FlatStyle::Flat;
			this->ConnectButton->Font = (gcnew System::Drawing::Font(L"Lucida Sans Unicode", 8.25F, System::Drawing::FontStyle::Bold));
			this->ConnectButton->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
				static_cast<System::Int32>(static_cast<System::Byte>(0)));
			this->ConnectButton->Location = System::Drawing::Point(12, 12);
			this->ConnectButton->Name = L"ConnectButton";
			this->ConnectButton->Size = System::Drawing::Size(143, 23);
			this->ConnectButton->TabIndex = 0;
			this->ConnectButton->Text = L"CONNECT";
			this->ConnectButton->UseVisualStyleBackColor = false;
			this->ConnectButton->Click += gcnew System::EventHandler(this, &MainForm::ConnectButton_Click);
			// 
			// testEventsButton
			// 
			this->testEventsButton->Location = System::Drawing::Point(12, 338);
			this->testEventsButton->Name = L"testEventsButton";
			this->testEventsButton->Size = System::Drawing::Size(143, 23);
			this->testEventsButton->TabIndex = 1;
			this->testEventsButton->Text = L"Test Events";
			this->testEventsButton->UseVisualStyleBackColor = true;
			this->testEventsButton->Click += gcnew System::EventHandler(this, &MainForm::testEventsButton_Click);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(745, 373);
			this->Controls->Add(this->testEventsButton);
			this->Controls->Add(this->ConnectButton);
			this->Name = L"MainForm";
			this->Text = L"PMDG AutoYoke";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::MainForm_FormClosed);
			this->ResumeLayout(false);

		}
#pragma endregion
	public: 
		static FSLink* PFSLink = NULL;
		System::Void ConnectButton_Click(System::Object^  sender, System::EventArgs^  e) {
			PFSLink = new FSLink();
	}
			 
	public:

		// Change any control
		HRESULT ChangeControl(char * control, char * controlAttribute, char * attributeValue)
		{
			/*try
			{
				array<Control^> ^FindControls = this->Controls->Find(gcnew String(control), true);
				if (FindControls->Length == 0)
				{
					//TO DO Разобраться, почему не проходит throw
					throw("Control " + gcnew String(control) + " not found");
				}
				FieldInfo^ Field = this->GetType()->GetField(gcnew String(control), BindingFlags::Instance | BindingFlags::NonPublic);
				if (Field == nullptr)
				{
					//TO DO Разобраться, почему не проходит throw
					throw("Control " + gcnew String(control) + " not found");
				}
				PropertyInfo^ controlAttribute1 = Field->FieldType->GetProperty(gcnew String(controlAttribute));
				if (controlAttribute1 == nullptr)
				{
					//TO DO Разобраться, почему не проходит throw
					throw("Property " + gcnew String(controlAttribute) + " not found");
				}	
				Object^ Color = controlAttribute1->GetValue(FindControls[0]);
				String^ ColorText = Color->ToString();
				Type^ T = controlAttribute1->PropertyType;
				controlAttribute1->SetValue(FindControls[0], gcnew String(attributeValue));
				return E_NOTIMPL;
			}
			catch (Exception^ ex)
			{
               	return STG_E_PATHNOTFOUND;
			}*/
			return E_NOTIMPL;
		}

		// Set state connecting to the sim
		HRESULT SetConnectState(int State)
		{
			switch (State)
			{
			case FSLink::DISCONNECTED:
			{
				ConnectButton->Text = "CONNECT";
				this->ConnectButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(125)),
					static_cast<System::Int32>(static_cast<System::Byte>(125)));
				this->ConnectButton->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)),
					static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
				this->ConnectButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)),
					static_cast<System::Int32>(static_cast<System::Byte>(30)), static_cast<System::Int32>(static_cast<System::Byte>(30)));
				this->ConnectButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)),
					static_cast<System::Int32>(static_cast<System::Byte>(80)), static_cast<System::Int32>(static_cast<System::Byte>(80)));
				this->ConnectButton->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(0)),
					static_cast<System::Int32>(static_cast<System::Byte>(0)));
				this->ConnectButton->Enabled = true;
				break;
			}
			case FSLink::CONNECTING:
			{
				ConnectButton->Text = "CONNECTING...";
				this->ConnectButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
					static_cast<System::Int32>(static_cast<System::Byte>(30)));
				this->ConnectButton->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)),
					static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
				this->ConnectButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)),
					static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(30)));
				this->ConnectButton->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(125)),
					static_cast<System::Int32>(static_cast<System::Byte>(0)));
				this->ConnectButton->Enabled = false;
				break;
			}
			case FSLink::CONNECTED:
			{
				ConnectButton->Text = "DISCONNECT";
				this->ConnectButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
					static_cast<System::Int32>(static_cast<System::Byte>(125)));
				this->ConnectButton->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)),
					static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
				this->ConnectButton->FlatAppearance->MouseDownBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(30)),
					static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(30)));
				this->ConnectButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(80)),
					static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(80)));	
				this->ConnectButton->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(0)), static_cast<System::Int32>(static_cast<System::Byte>(125)),
					static_cast<System::Int32>(static_cast<System::Byte>(0)));
				this->ConnectButton->Enabled = true;
				break;
			}
			case FSLink::DISCONNECTING:
			{
				ConnectButton->Text = "DISCONNECTING...";
				this->ConnectButton->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(255)), static_cast<System::Int32>(static_cast<System::Byte>(255)),
					static_cast<System::Int32>(static_cast<System::Byte>(30)));
				this->ConnectButton->FlatAppearance->BorderColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)),
					static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(0)));
				this->ConnectButton->FlatAppearance->MouseOverBackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)),
					static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(30)));
				this->ConnectButton->ForeColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(125)), static_cast<System::Int32>(static_cast<System::Byte>(125)),
					static_cast<System::Int32>(static_cast<System::Byte>(0)));
				this->ConnectButton->Enabled = false;
				break;
			}
			default:
			{
				return E_ABORT;
				break;
			}
			}
			return 0;
		}
private: System::Void MainForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
	PFSLink->~FSLink();
}
private: System::Void testEventsButton_Click(System::Object^  sender, System::EventArgs^  e) {
	
	testEventsForm->ShowDialog();
}
};
}

