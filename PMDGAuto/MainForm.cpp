#include "MainForm.h"


using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
{

	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	PMDGAuto::MainForm mainForm;
	Application::Run(%mainForm);
	return 0;
}
