
#pragma once
#include "Windows.h"

#using <mscorlib.dll>

namespace PMDG_TEST {
//#include <graphics.h>
	using namespace System;
	using namespace System::Reflection;
	using namespace System::Reflection::Emit;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;


	public ref class MainForm : public System::Windows::Forms::Form
	{

	public: static MainForm^ mainForm;
	public:
		MainForm(void)
		{
			InitializeComponent();
			mainForm = this;
		}

	protected:
		/// <summary>
		/// ќсвободить все используемые ресурсы.
		/// </summary>
		~MainForm()
		{
			if (components)
			{
				delete components;
			}
		}
	public:  System::Windows::Forms::Button^  ConnectButton;
	public:  System::Windows::Forms::TextBox^  textBox1;
	public:  System::Windows::Forms::VScrollBar^  vScrollBar1;
	public:  System::Windows::Forms::Label^  label1;
	public: System::Windows::Forms::ComboBox^  comboBox1;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown1;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown2;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown3;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown4;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown5;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown6;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown7;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown8;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown9;
	public: System::Windows::Forms::NumericUpDown^  numericUpDown10;
	public: System::Windows::Forms::VScrollBar^  vScrollBar2;
	public: System::Windows::Forms::VScrollBar^  vScrollBar3;
	public: System::Windows::Forms::VScrollBar^  vScrollBar4;
	public: System::Windows::Forms::VScrollBar^  vScrollBar5;
	public: System::Windows::Forms::VScrollBar^  vScrollBar6;
	public:  System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Timer^  timer1;
	public: System::Windows::Forms::Label^  label2;
	public: System::Windows::Forms::Label^  label3;
	public: System::Windows::Forms::Label^  label4;
	public: System::Windows::Forms::Label^  label5;
	public: System::Windows::Forms::Label^  label6;
	private: System::Windows::Forms::Button^  button3;
	public: System::Windows::Forms::TextBox^  textBox2;
	public: System::Windows::Forms::TextBox^  textBox3;
	public: System::Windows::Forms::TextBox^  textBox4;
	public: System::Windows::Forms::TextBox^  textBox5;
	public: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::Button^  button4;
	public: System::Windows::Forms::Panel^  AirportImage;
	private: System::Windows::Forms::Button^  button1;
	public:
	private:
	public:
	public:
	private: System::ComponentModel::IContainer^  components;

	public:


	public:

	public:
	public:

	protected:

	private:
		/// <summary>
		/// “ребуетс€ переменна€ конструктора.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// ќб€зательный метод дл€ поддержки конструктора - не измен€йте
		/// содержимое данного метода при помощи редактора кода.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->ConnectButton = (gcnew System::Windows::Forms::Button());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->vScrollBar1 = (gcnew System::Windows::Forms::VScrollBar());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->numericUpDown1 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown2 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown3 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown4 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown5 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown6 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown7 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown8 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown9 = (gcnew System::Windows::Forms::NumericUpDown());
			this->numericUpDown10 = (gcnew System::Windows::Forms::NumericUpDown());
			this->vScrollBar2 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar3 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar4 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar5 = (gcnew System::Windows::Forms::VScrollBar());
			this->vScrollBar6 = (gcnew System::Windows::Forms::VScrollBar());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->AirportImage = (gcnew System::Windows::Forms::Panel());
			this->button1 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown4))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown5))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown6))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown7))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown8))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown9))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown10))->BeginInit();
			this->SuspendLayout();
			// 
			// ConnectButton
			// 
			this->ConnectButton->Cursor = System::Windows::Forms::Cursors::Hand;
			this->ConnectButton->ForeColor = System::Drawing::SystemColors::ControlText;
			this->ConnectButton->Location = System::Drawing::Point(12, 12);
			this->ConnectButton->Name = L"ConnectButton";
			this->ConnectButton->Size = System::Drawing::Size(260, 231);
			this->ConnectButton->TabIndex = 0;
			this->ConnectButton->Text = L"Connect to the simulator";
			this->ConnectButton->UseVisualStyleBackColor = true;
			this->ConnectButton->Click += gcnew System::EventHandler(this, &MainForm::button1_Click);
			// 
			// textBox1
			// 
			this->textBox1->Enabled = false;
			this->textBox1->Location = System::Drawing::Point(12, 249);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->ScrollBars = System::Windows::Forms::ScrollBars::Vertical;
			this->textBox1->Size = System::Drawing::Size(260, 0);
			this->textBox1->TabIndex = 1;
			// 
			// vScrollBar1
			// 
			this->vScrollBar1->Enabled = false;
			this->vScrollBar1->LargeChange = 1;
			this->vScrollBar1->Location = System::Drawing::Point(275, 12);
			this->vScrollBar1->Maximum = 30;
			this->vScrollBar1->Minimum = -30;
			this->vScrollBar1->Name = L"vScrollBar1";
			this->vScrollBar1->Size = System::Drawing::Size(46, 211);
			this->vScrollBar1->TabIndex = 2;
			this->vScrollBar1->ValueChanged += gcnew System::EventHandler(this, &MainForm::vScrollBar1_ValueChanged);
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 8.26F, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(204)));
			this->label1->Location = System::Drawing::Point(272, 230);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(49, 15);
			this->label1->TabIndex = 3;
			this->label1->Text = L"Thr 0 %";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"Bank", L"Pitch", L"VSSpeed" });
			this->comboBox1->Location = System::Drawing::Point(324, 12);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(262, 21);
			this->comboBox1->TabIndex = 4;
			this->comboBox1->SelectedIndexChanged += gcnew System::EventHandler(this, &MainForm::comboBox1_SelectedIndexChanged);
			// 
			// numericUpDown1
			// 
			this->numericUpDown1->Location = System::Drawing::Point(324, 51);
			this->numericUpDown1->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->numericUpDown1->Name = L"numericUpDown1";
			this->numericUpDown1->Size = System::Drawing::Size(44, 20);
			this->numericUpDown1->TabIndex = 5;
			// 
			// numericUpDown2
			// 
			this->numericUpDown2->Location = System::Drawing::Point(374, 51);
			this->numericUpDown2->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->numericUpDown2->Name = L"numericUpDown2";
			this->numericUpDown2->Size = System::Drawing::Size(44, 20);
			this->numericUpDown2->TabIndex = 5;
			// 
			// numericUpDown3
			// 
			this->numericUpDown3->Location = System::Drawing::Point(424, 51);
			this->numericUpDown3->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->numericUpDown3->Name = L"numericUpDown3";
			this->numericUpDown3->Size = System::Drawing::Size(44, 20);
			this->numericUpDown3->TabIndex = 5;
			// 
			// numericUpDown4
			// 
			this->numericUpDown4->Location = System::Drawing::Point(492, 51);
			this->numericUpDown4->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->numericUpDown4->Name = L"numericUpDown4";
			this->numericUpDown4->Size = System::Drawing::Size(44, 20);
			this->numericUpDown4->TabIndex = 5;
			// 
			// numericUpDown5
			// 
			this->numericUpDown5->Location = System::Drawing::Point(542, 51);
			this->numericUpDown5->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->numericUpDown5->Name = L"numericUpDown5";
			this->numericUpDown5->Size = System::Drawing::Size(44, 20);
			this->numericUpDown5->TabIndex = 5;
			// 
			// numericUpDown6
			// 
			this->numericUpDown6->Location = System::Drawing::Point(324, 222);
			this->numericUpDown6->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->numericUpDown6->Name = L"numericUpDown6";
			this->numericUpDown6->Size = System::Drawing::Size(44, 20);
			this->numericUpDown6->TabIndex = 5;
			// 
			// numericUpDown7
			// 
			this->numericUpDown7->Location = System::Drawing::Point(374, 222);
			this->numericUpDown7->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->numericUpDown7->Name = L"numericUpDown7";
			this->numericUpDown7->Size = System::Drawing::Size(44, 20);
			this->numericUpDown7->TabIndex = 5;
			// 
			// numericUpDown8
			// 
			this->numericUpDown8->Location = System::Drawing::Point(424, 222);
			this->numericUpDown8->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->numericUpDown8->Name = L"numericUpDown8";
			this->numericUpDown8->Size = System::Drawing::Size(44, 20);
			this->numericUpDown8->TabIndex = 5;
			// 
			// numericUpDown9
			// 
			this->numericUpDown9->Location = System::Drawing::Point(492, 222);
			this->numericUpDown9->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->numericUpDown9->Name = L"numericUpDown9";
			this->numericUpDown9->Size = System::Drawing::Size(44, 20);
			this->numericUpDown9->TabIndex = 5;
			// 
			// numericUpDown10
			// 
			this->numericUpDown10->Location = System::Drawing::Point(542, 222);
			this->numericUpDown10->Maximum = System::Decimal(gcnew cli::array< System::Int32 >(4) { 10000, 0, 0, 0 });
			this->numericUpDown10->Name = L"numericUpDown10";
			this->numericUpDown10->Size = System::Drawing::Size(44, 20);
			this->numericUpDown10->TabIndex = 5;
			// 
			// vScrollBar2
			// 
			this->vScrollBar2->LargeChange = 1;
			this->vScrollBar2->Location = System::Drawing::Point(335, 74);
			this->vScrollBar2->Maximum = 10000;
			this->vScrollBar2->Name = L"vScrollBar2";
			this->vScrollBar2->Size = System::Drawing::Size(16, 145);
			this->vScrollBar2->TabIndex = 6;
			this->vScrollBar2->Value = 10000;
			this->vScrollBar2->Scroll += gcnew System::Windows::Forms::ScrollEventHandler(this, &MainForm::vScrollBar2_Scroll);
			this->vScrollBar2->ValueChanged += gcnew System::EventHandler(this, &MainForm::vScrollBar2_ValueChanged);
			// 
			// vScrollBar3
			// 
			this->vScrollBar3->LargeChange = 1;
			this->vScrollBar3->Location = System::Drawing::Point(386, 74);
			this->vScrollBar3->Maximum = 500;
			this->vScrollBar3->Name = L"vScrollBar3";
			this->vScrollBar3->Size = System::Drawing::Size(16, 145);
			this->vScrollBar3->TabIndex = 6;
			this->vScrollBar3->Value = 1;
			this->vScrollBar3->ValueChanged += gcnew System::EventHandler(this, &MainForm::vScrollBar3_ValueChanged);
			// 
			// vScrollBar4
			// 
			this->vScrollBar4->LargeChange = 1;
			this->vScrollBar4->Location = System::Drawing::Point(435, 74);
			this->vScrollBar4->Name = L"vScrollBar4";
			this->vScrollBar4->Size = System::Drawing::Size(16, 145);
			this->vScrollBar4->TabIndex = 6;
			this->vScrollBar4->ValueChanged += gcnew System::EventHandler(this, &MainForm::vScrollBar4_ValueChanged);
			// 
			// vScrollBar5
			// 
			this->vScrollBar5->LargeChange = 1;
			this->vScrollBar5->Location = System::Drawing::Point(504, 74);
			this->vScrollBar5->Maximum = 500;
			this->vScrollBar5->Name = L"vScrollBar5";
			this->vScrollBar5->Size = System::Drawing::Size(16, 145);
			this->vScrollBar5->TabIndex = 6;
			this->vScrollBar5->ValueChanged += gcnew System::EventHandler(this, &MainForm::vScrollBar5_ValueChanged);
			// 
			// vScrollBar6
			// 
			this->vScrollBar6->LargeChange = 1;
			this->vScrollBar6->Location = System::Drawing::Point(553, 74);
			this->vScrollBar6->Maximum = 500;
			this->vScrollBar6->Name = L"vScrollBar6";
			this->vScrollBar6->Size = System::Drawing::Size(16, 145);
			this->vScrollBar6->TabIndex = 6;
			this->vScrollBar6->ValueChanged += gcnew System::EventHandler(this, &MainForm::vScrollBar6_ValueChanged);
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(12, 286);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(574, 225);
			this->panel1->TabIndex = 7;
			this->panel1->Paint += gcnew System::Windows::Forms::PaintEventHandler(this, &MainForm::panel1_Paint);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(12, 517);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(574, 23);
			this->button2->TabIndex = 8;
			this->button2->Text = L"Start test";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &MainForm::button2_Click);
			// 
			// timer1
			// 
			this->timer1->Enabled = true;
			this->timer1->Tick += gcnew System::EventHandler(this, &MainForm::timer1_Tick);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(321, 245);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(35, 13);
			this->label2->TabIndex = 9;
			this->label2->Text = L"label2";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(371, 245);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(35, 13);
			this->label3->TabIndex = 10;
			this->label3->Text = L"label3";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(421, 245);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(35, 13);
			this->label4->TabIndex = 11;
			this->label4->Text = L"label4";
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(489, 245);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(35, 13);
			this->label5->TabIndex = 12;
			this->label5->Text = L"label5";
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(539, 245);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(35, 13);
			this->label6->TabIndex = 13;
			this->label6->Text = L"label6";
			// 
			// button3
			// 
			this->button3->Location = System::Drawing::Point(275, 245);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(46, 23);
			this->button3->TabIndex = 14;
			this->button3->Text = L"Control";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &MainForm::button3_Click);
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(324, 261);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(44, 20);
			this->textBox2->TabIndex = 15;
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &MainForm::textBox2_TextChanged);
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(374, 261);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(44, 20);
			this->textBox3->TabIndex = 16;
			this->textBox3->TextChanged += gcnew System::EventHandler(this, &MainForm::textBox3_TextChanged);
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(424, 260);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(44, 20);
			this->textBox4->TabIndex = 17;
			this->textBox4->TextChanged += gcnew System::EventHandler(this, &MainForm::textBox4_TextChanged);
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(492, 261);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(44, 20);
			this->textBox5->TabIndex = 18;
			this->textBox5->TextChanged += gcnew System::EventHandler(this, &MainForm::textBox5_TextChanged);
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(542, 261);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(44, 20);
			this->textBox6->TabIndex = 19;
			this->textBox6->TextChanged += gcnew System::EventHandler(this, &MainForm::textBox6_TextChanged);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(12, 261);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(257, 20);
			this->button4->TabIndex = 20;
			this->button4->Text = L"ShowWings";
			this->button4->UseVisualStyleBackColor = true;
			this->button4->Click += gcnew System::EventHandler(this, &MainForm::button4_Click);
			// 
			// AirportImage
			// 
			this->AirportImage->Location = System::Drawing::Point(592, 12);
			this->AirportImage->Name = L"AirportImage";
			this->AirportImage->Size = System::Drawing::Size(831, 499);
			this->AirportImage->TabIndex = 8;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(593, 518);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(286, 23);
			this->button1->TabIndex = 21;
			this->button1->Text = L"button1";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &MainForm::button1_Click_1);
			// 
			// MainForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1435, 552);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->AirportImage);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->textBox6);
			this->Controls->Add(this->textBox5);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->vScrollBar6);
			this->Controls->Add(this->vScrollBar5);
			this->Controls->Add(this->vScrollBar4);
			this->Controls->Add(this->vScrollBar3);
			this->Controls->Add(this->vScrollBar2);
			this->Controls->Add(this->numericUpDown10);
			this->Controls->Add(this->numericUpDown5);
			this->Controls->Add(this->numericUpDown9);
			this->Controls->Add(this->numericUpDown4);
			this->Controls->Add(this->numericUpDown8);
			this->Controls->Add(this->numericUpDown7);
			this->Controls->Add(this->numericUpDown3);
			this->Controls->Add(this->numericUpDown6);
			this->Controls->Add(this->numericUpDown2);
			this->Controls->Add(this->numericUpDown1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->vScrollBar1);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->ConnectButton);
			this->Name = L"MainForm";
			this->Text = L"MyForm";
			this->FormClosed += gcnew System::Windows::Forms::FormClosedEventHandler(this, &MainForm::MyForm_FormClosed);
			this->Load += gcnew System::EventHandler(this, &MainForm::MyForm_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown2))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown4))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown5))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown6))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown7))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown8))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown9))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->numericUpDown10))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button1_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button2_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void MyForm_Load(System::Object^  sender, System::EventArgs^  e) {
	}
	private: System::Void MyForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e);
	private: System::Void vScrollBar1_ValueChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e);
	public: void SetControlSafe(String^ nc, String^ np, Object^ v)
	{
		Control^ c = FindControl(nc);
		PropertyDescriptor^ p = FindProperty(c, np);
		if (c->InvokeRequired)
		{
			c->BeginInvoke(gcnew  Action<String^, String^, Object^>(this, &MainForm::SetControlSafe), nc, np, v);
		}
		else
		{
			p->SetValue(c, v);
		}
	}
	public: Object ^ GetControlSafe(String^ nc, String^ np)
	{
		Control^ c = FindControl(nc);
		PropertyDescriptor^ p = FindProperty(c, np);
		if (c->InvokeRequired)
		{
			return c->Invoke(gcnew Func<String^, String^, Object^>(this, &MainForm::GetControlSafe), nc, np);
		}
		else
		{
			return p->GetValue(c);
		}
	}



	public: Control ^ FindControl(String^ NameControl)
	{
		return MainForm::Controls->Find(NameControl, true)[0];
	}
	public: PropertyDescriptor ^ FindProperty(Control^ control, String^ NameProperty)
	{
		PropertyDescriptorCollection^ properties = TypeDescriptor::GetProperties(control);
		PropertyDescriptor^ myProperty = properties->Find(NameProperty, true);
		return myProperty;
	}






	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);


	private: System::Void vScrollBar2_Scroll(System::Object^  sender, System::Windows::Forms::ScrollEventArgs^  e) {
	}
	private: System::Void vScrollBar2_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
		MainForm::mainForm->label2->Text = this->vScrollBar2->Value.ToString();
	}
	private: System::Void vScrollBar3_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
		MainForm::mainForm->label3->Text = this->vScrollBar3->Value.ToString();
	}
	private: System::Void vScrollBar4_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
		MainForm::mainForm->label4->Text = this->vScrollBar4->Value.ToString();
	}
	private: System::Void vScrollBar5_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
		MainForm::mainForm->label5->Text = this->vScrollBar5->Value.ToString();
	}
	private: System::Void vScrollBar6_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
		MainForm::mainForm->label6->Text = this->vScrollBar6->Value.ToString();
	}
	private: System::Void button3_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void textBox4_TextChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void textBox5_TextChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void textBox6_TextChanged(System::Object^  sender, System::EventArgs^  e);
	private: System::Void comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e);

	private: System::Void button4_Click(System::Object^  sender, System::EventArgs^  e);


	private: System::Void button1_Click_1(System::Object^  sender, System::EventArgs^  e);
	};
}
