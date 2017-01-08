#include "MyForm.h"
#include "PMDG_NGX.h"
namespace PMDG_TEST {
using namespace System;
using namespace System::Windows::Forms;

[STAThreadAttribute]
int main(array<System::String ^> ^args)
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR
//lpcmdline, int ncmdshow)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	MyForm form;
	Application::Run(%form);
	return 0;
}

System::Void MyForm::button1_Click(System::Object^  sender, System::EventArgs^  e) {
	button1->Text = "";
	PMDG_TEST::Connect();
	MyForm::mainForm->comboBox1->SelectedIndex = 1;
};
System::Void MyForm::MyForm_FormClosed(System::Object^  sender, System::Windows::Forms::FormClosedEventArgs^  e) {
	System::IntPtr pDrawHandle = MyForm::mainForm->panel1->Handle;
	HWND PDraw = (HWND)pDrawHandle.ToPointer();
	quit = 1;
	if (hDC != NULL)
	{
		// Выбираем старый Pen в DC (освобождаем свой Pen из DC)
		SelectObject(hDC, hOldPen);
		// Удаляем Pen
		DeleteObject(hPenSolidRed);
		DeleteObject(hPenSolid2Blue);
		DeleteObject(hPenSolid2Green);
		DeleteObject(hPenDottedRed);
		// Освобождаем DC
		ReleaseDC(PDraw, hDC);
		hDC = NULL;
	}
};
System::Void MyForm::vScrollBar1_ValueChanged(System::Object^  sender, System::EventArgs^  e) {
			 //SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_THROTTLE_SET, vScrollBar1->Value,
				// SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
};
System::Void MyForm::comboBox1_SelectedIndexChanged(System::Object^  sender, System::EventArgs^  e)
{
	switch (MyForm::mainForm->comboBox1->SelectedIndex)
	{
	case 0:
	{
			  SelectedPid = &pidBank;
			  break;
	}
	case 1:
	{
			  SelectedPid = &pidPitch;
			  break;
	}
	case 2:
	{
			  SelectedPid = &pidVSpeed;
			  break;
	}
	default:
	{
			   SelectedPid = &pidBank;
			   break;
	}
	};
	textBox2->Text = SelectedPid->pGain.ToString();
	textBox3->Text = SelectedPid->iGain.ToString();
	textBox4->Text = SelectedPid->dGain.ToString();
};
System::Void MyForm::button2_Click(System::Object^  sender, System::EventArgs^  e) {
	System::IntPtr pDrawHandle = MyForm::mainForm->panel1->Handle;
	HWND PDraw = (HWND)pDrawHandle.ToPointer();
	if (hDC == NULL)
	{

		
		hDC = GetDC(PDraw);

		RECT rc;
		GetClientRect(PDraw, &rc);

		// Создаем Pen
		hPenSolidRed = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
		hPenSolid2Blue = CreatePen(PS_SOLID, 2, RGB(0, 0, 255));
		hPenSolid2Green = CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
		hPenDottedRed = CreatePen(PS_DOT, 1, RGB(255, 0, 0));

		// Выбираем свой Pen в DC, запоминаем старый Pen
		hOldPen = (HPEN)SelectObject(hDC, hPenSolid2Blue);


	}
	else
	{
		// Выбираем старый Pen в DC (освобождаем свой Pen из DC)
		SelectObject(hDC, hOldPen);
		
		// Удаляем Pen
		DeleteObject(hPenSolidRed);
		DeleteObject(hPenSolid2Blue);
		DeleteObject(hPenSolid2Green);
		DeleteObject(hPenDottedRed);

		// Освобождаем DC
		ReleaseDC(PDraw, hDC);
		hDC = NULL;
	}

	
}
System::Void MyForm::timer1_Tick(System::Object^  sender, System::EventArgs^  e) {
	MyForm::mainForm->label1->Text = PMDG_TEST::AircraftData.throttle1.ToString();
			 if (hDC != NULL)
			 {
				 static long yds = 0;
				 if (TestControl)
				 {
					 SelectedPid->inVal = -(double)MyForm::mainForm->vScrollBar1->Value*100;
				 }
				 //pidVSpeed.pGain = (double)MyForm::mainForm->vScrollBar2->Value;
				 //pidVSpeed.iGain = (double)MyForm::mainForm->vScrollBar3->Value;
				 //pidVSpeed.dGain = (double)MyForm::mainForm->vScrollBar4->Value;
				 //pidVSpeed.iMin = -5000.0;
				 //pidVSpeed.iMax = 5000.0;
				 yds++;
				 long d = yds;

				 clock_t currTime = clock();
				 double val = sin((double)((double)currTime / 1800.0 * M_PI));
				 GraphArray[CurrentGraphIndex].value = SelectedPid->val;
				 //GraphArray[CurrentGraphIndex].value = -AircraftData.VS*60/100;
				 GraphArray[CurrentGraphIndex].time = currTime;

				 GraphArray2[CurrentGraphIndex].value = SelectedPid->Term;
				 //GraphArray2[CurrentGraphIndex].value = AircraftData.pitch;
				 GraphArray2[CurrentGraphIndex].time = currTime;

				 GraphArray3[CurrentGraphIndex].value = -SelectedPid->inVal;
				 //GraphArray3[CurrentGraphIndex].value = -ExportData.HorBar;
				 GraphArray3[CurrentGraphIndex].time = currTime;

				 RECT rc;
				 HWND PDraw = (HWND)MyForm::mainForm->panel1->Handle.ToPointer();
				 GetClientRect(PDraw, &rc);
				 MyForm::mainForm->panel1->Refresh();
				 SelectObject(hDC, hPenSolid2Blue);
				 // Перемещаем точку рисования в левый верхний угол окна
				 MoveToEx(hDC, rc.left, rc.bottom / 2, NULL);
				 // Рисуем линию в правый нижний угол
				 LineTo(hDC, rc.right, rc.bottom / 2);
				 SelectObject(hDC, hPenSolid2Green);
				 MoveToEx(hDC, rc.left + 5, rc.top, NULL);
				 LineTo(hDC, rc.left + 5, rc.bottom);
				 int minGraphIndex = CurrentGraphIndex - MaxIndexGraph;
				 if (minGraphIndex < 0)
				 {
					 minGraphIndex = CurrentGraphIndex + 1;
				 }

				 SelectObject(hDC, hPenSolidRed);
				 MoveToEx(hDC, rc.left + 5, (int)(((double)((double)rc.bottom * (double)YScaleGraph) * (double)GraphArray[minGraphIndex].value / 2.0) + ((double)rc.bottom / 2.0)), NULL);
				 for (int i = 0; i < MaxIndexGraph; i++)
				 {

					 int IndexArray = i + minGraphIndex;
					 if (IndexArray > MaxIndexGraph)
					 {
						 IndexArray = i + minGraphIndex - MaxIndexGraph;
					 }
					 LineTo(hDC, ((int)((double)((double)rc.right - 5.0) / (double)XScaleGraph*(double)((double)(GraphArray[IndexArray].time - (double)GraphArray[minGraphIndex].time) / 1000.0))) + 5.0, (int)((((double)(rc.bottom) / YScaleGraph) * (double)GraphArray[IndexArray].value / 2.0) + ((double)rc.bottom / 2.0)));
				 }

				 SelectObject(hDC, hPenDottedRed);
				 MoveToEx(hDC, rc.left + 5, (int)(((double)((double)rc.bottom * (double)YScaleGraph) * (double)GraphArray2[minGraphIndex].value / 2.0) + ((double)rc.bottom / 2.0)), NULL);
				 for (int i = 0; i < MaxIndexGraph; i++)
				 {

					 int IndexArray = i + minGraphIndex;
					 if (IndexArray > MaxIndexGraph)
					 {
						 IndexArray = i + minGraphIndex - MaxIndexGraph;
					 }
					 LineTo(hDC, ((int)((double)((double)rc.right - 5.0) / (double)XScaleGraph*(double)((double)(GraphArray2[IndexArray].time - (double)GraphArray2[minGraphIndex].time) / 1000.0))) + 5.0, (int)((((double)(rc.bottom) / YScaleGraph) * (double)GraphArray2[IndexArray].value / 2.0) + ((double)rc.bottom / 2.0)));
				 }

				 SelectObject(hDC, hPenSolid2Green);
				 MoveToEx(hDC, rc.left + 5, (int)(((double)((double)rc.bottom * (double)YScaleGraph) * (double)GraphArray3[minGraphIndex].value / 2.0) + ((double)rc.bottom / 2.0)), NULL);
				 for (int i = 0; i < MaxIndexGraph; i++)
				 {

					 int IndexArray = i + minGraphIndex;
					 if (IndexArray > MaxIndexGraph)
					 {
						 IndexArray = i + minGraphIndex - MaxIndexGraph;
					 }
					 LineTo(hDC, ((int)((double)((double)rc.right - 5.0) / (double)XScaleGraph*(double)((double)(GraphArray3[IndexArray].time - (double)GraphArray3[minGraphIndex].time) / 1000.0))) + 5.0, (int)((((double)(rc.bottom) / YScaleGraph) * (double)GraphArray3[IndexArray].value / 2.0) + ((double)rc.bottom / 2.0)));
				 }
				 
				 
				 if (((GraphArray[CurrentGraphIndex].time - GraphArray[minGraphIndex].time)) < ((XScaleGraph-2)*1000))
				 {
					 MaxIndexGraph++;
				 }
				 else if (((GraphArray[CurrentGraphIndex].time - GraphArray[minGraphIndex].time)) > ((XScaleGraph ) * 1000))
				 {
					 MaxIndexGraph--;
				 }
				 CurrentGraphIndex++;
				 if (CurrentGraphIndex >(MaxIndexGraph))
				 {
					 CurrentGraphIndex = 0;
				 }
				 
			 }

}
System::Void MyForm::button3_Click(System::Object^  sender, System::EventArgs^  e) {
			 MyForm::mainForm->vScrollBar1->Enabled = !MyForm::mainForm->vScrollBar1->Enabled;
			 TestControl = MyForm::mainForm->vScrollBar1->Enabled;
			 textBox2->Text = SelectedPid->pGain.ToString();
			 textBox3->Text = SelectedPid->iGain.ToString();
			 textBox4->Text = SelectedPid->dGain.ToString();
}
System::Void MyForm::panel1_Paint(System::Object^  sender, System::Windows::Forms::PaintEventArgs^  e) {

}
System::Void MyForm::textBox2_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	String^ ps = MyForm::mainForm->textBox2->Text;
	
	SelectedPid->pGain = Convert::ToDouble(ps);
}

System::Void MyForm::textBox3_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	String^ ps = MyForm::mainForm->textBox3->Text;

	SelectedPid->iGain = Convert::ToDouble(ps);
}

System::Void MyForm::textBox4_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	String^ ps = MyForm::mainForm->textBox4->Text;

	SelectedPid->dGain = Convert::ToDouble(ps);
}

System::Void MyForm::textBox5_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	String^ ps = MyForm::mainForm->textBox5->Text;

	pidVSpeed.iMin = Convert::ToDouble(ps);
}

System::Void MyForm::textBox6_TextChanged(System::Object^  sender, System::EventArgs^  e) {
	String^ ps = MyForm::mainForm->textBox6->Text;

	pidVSpeed.iMax = Convert::ToDouble(ps);
}



};


