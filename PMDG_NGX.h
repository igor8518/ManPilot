#include "PMDG_NGX_SDK.h"
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include "SimConnect.h"
#include "MyForm.h"
#include "Structs.h"
#include <time.h>
#include "Simmath.h"
#include "ManPilot.h"
#include "AlternatePilot.h"
#include "INITVARS.h"
#include "WayLoader.h"
#include "Route.h"
//#include <functional>
//#include <graphics.h>
//#include <gdiplus.h>
#pragma once

namespace PMDG_TEST{

#define PMDG_ADD_DATA_NAME			"PMDG_Additional_Data"
#define PMDG_ADD_DATA_ID			0x4E877777
#define PMDG_ADD_DATA_DEFINITION	0x4E877778

	HANDLE  hSimConnect = NULL; //������������� ����������� SimConnect
	HDC hDC = NULL;
	HPEN hPenSolidRed;
	HPEN hPenSolid2Blue;
	HPEN hPenSolid2Green;
	HPEN hPenDottedRed;
	HPEN hOldPen;
	HRESULT hr; //��������� ���������� ������ SimConnect

	PMDG_NGX_Control Control; //���������� ��� ���������� B737NGX
	PMDG_NGX_Data NGX; //���������� ��� ������ ������ B737NGX
	bool GetNGXData = FALSE; //��������� ������� ������ ������ B737NGX
	bool AircraftRunning = false; //��������� �������� B737NGX

	DSHEH *Ways[500]; //
	
	DSHEH CurrentPos;  //������� ��������� ��������
	int CountWays; //���������� �����
	int CurrentWay = 0; //������� ���� 

	bool flare = true;
	bool TOGA = FALSE; //��������� ���������� ��������� ������ TO/GA

	bool Connected = FALSE; //��������� ����� � SimConnect
	int     quit = 0; //��������� ��������� ������ ���������

	bool SetDataPitch = true; //��������� ��������� ����������� ������ � �������
	bool SetDataBank = true; //��������� ��������� ����������� ������ � �����

	double YScaleGraph = 30;
	double XScaleGraph = 60;
	long CurrentGraphIndex = 0;
	long MaxIndexGraph = 0;
	GraphPair* GraphArray = new GraphPair[MaxIndexGraphS];
	GraphPair* GraphArray2 = new GraphPair[MaxIndexGraphS];
	GraphPair* GraphArray3 = new GraphPair[MaxIndexGraphS];
	bool TestControl = FALSE;
	alterPid VSPID = { -0.0008, 0.0, 0, 0.1, 1.0, 0.0, 1.5, 0.001, 0.0, 0.0, 0.0, 0, -15.0, 15.0, true };
	alterPid PPID = { -0.2, 0.0, 0, 0.1, 1.0, 0.0, 2.0, 0.001, 0.0, 0.0, 0.0, 0, -1.0, 1.0, true };
	SPid pidBank = { 0.0, 0.0, 1500.0, 15.0, 5.0, 0.0, 0.0, 0.0, 0.0 };
	SPid pidPitch = { 0.0, 0.0, PPID.Kp, PPID.Ti, PPID.Td, 0.0, 0.0, 0.0, 0.0 };
	SPid pidVSpeed = { 0.0, 0.0, VSPID.Kp, VSPID.Ti, VSPID.Td, 0.0, 0.0, 0.0, 0.0 };
	SPid * SelectedPid;
	//������������ ������� ������
	static enum MODE {
		PREPARE,
		START,
		TAXI_OUT,
		TAKE_OFF,
		ABORT_TAKE_OFF,
		CIRCLE_TAXI,
		CLIMB,
		CRUISE,
		DESCENT,
		LANDING,
		ABORT_LANDING,
		BRAKE,
		TAXI_IN,
		PARK,
		SHUTDOWN,
		SECURE
	};
	MODE mode = initmode; //��������� ����� (������������ � INITVARS.h)
	//////////////////////////////////////

	//������������ ��������������� �������� � SimConnect
	static enum DATA_REQUEST_ID {
		DATA_REQUEST, //������ ������ B737NGX
		CONTROL_REQUEST, //������ �������� B737NGX
		AIR_PATH_REQUEST, //������ ���� � ����� *.air ������������ ��������
		REQ_AIRCRAFT_USER, //������ ���������� ����������� ��� ��������� �� ���� ������� ������
		REQ_SPEEDS, //������ ������ ��������
		REQ_THRUST, //������ ������ ����
		ADD_DATA_REQUEST, //������ �������������� ������, ����������� LED.DLL
		REQ_ELEVATOR //������ ��������� ���� ������
	};
	//////////////////////////////////////////////////

	//������������ ��������������� ������������� ������
	static enum DATA_DEFINE_ID {
		DEF_AIRCRAFT_USER, //��������� aircraftData ����������� ��� ��������� �� ���� ������� ������
		DEF_SPEEDS, //��������� tspeeds ������ ��������
		DEF_THRUST, //��������� tthrust ������ ����
		DEF_THROTTLE, //��������� tthrottle ������ ���
		DEF_ELEVATOR, //���������� ��������� ���� ������
		DEF_AILERON, ////���������� ��������� ���� �����
		DEF_SPOILER,
		DEF_BRAKE
	};
	//////////////////////////////////////////////////

	//����������� ������� ����������� ����������, ���� �����������
	static enum EVENT_ID {
		EVENT_SIM_START,	// ��� ��������

		EVENT_LOGO_LIGHT_SWITCH, //�������� �� ���
		EVENT_FLIGHT_DIRECTOR_SWITCH, //�������� �� ���
		EVENT_HEADING_SELECTOR, //�������� �� ���

		EVENT_KEYBOARD_A, //������ ���������� ctrl+a
		EVENT_KEYBOARD_B, //������ ���������� ctrl+b
		EVENT_KEYBOARD_C, //������ ���������� ctrl+c
		EVENT_KEYBOARD_D, //������ ���������� ctrl+d

		EVENT_THROTTLE_SET, //��������� ���
		EVENT_RUDDER_SET, //��������� ���� �����������
		EVENT_ELEVATOR_SET, //��������� ���� ������
		EVENT_AILERON_SET, //��������� ���� �����

		EVENT_CONTROL_STAND_FLAPS_LEVER_30, //��������� ��������� �������� 30 ��������
		EVENT_CONTROL_STAND_FLAPS_LEVER_20, //��������� ��������� �������� 5 ��������
		EVENT_CONTROL_STAND_FLAPS_LEVER_15, //��������� ��������� �������� 5 ��������
		EVENT_CONTROL_STAND_FLAPS_LEVER_5, //��������� ��������� �������� 5 ��������
		EVENT_CONTROL_STAND_FLAPS_LEVER_1, //��������� ��������� �������� 1 ������
		EVENT_CONTROL_STAND_FLAPS_LEVER_0, //��������� ��������� �������� 0 ��������

		EVENT_TOGA, //��������� ������ TO/GA ������/�����

		EVENT_TEXT,

		EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN,
		EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP
	};
	//////////////////////////////////////////////////

	//������������ ������ ������� �����
	static enum INPUT_ID {
		INPUT0			
	};
	//////////////////////////////////////////////////
	//������������ ����� ��� �������� �� �������
	static enum GROUP_ID {
		GROUP_KEYBOARD,		// ������� ������
		GROUP0, // ������ �������
		GROUP_ELEVATOR // ������� ������������ ���� ������
	};
	//////////////////////////////////////////////////

	static enum TYPE_PATH {
		TAXIWAY,
		RUNWAY,
		WAYPOINT,
		HEADINGUNTILALT,
		HEADINGUNTILRADIALTOFIX,
		FIXATORABOVE,
		TURNLEFTDIRECTFIX,
		TURNRIGHTDIRECTFIX,
		FIX,
		GSWAY,
	};

	//��������� �������
	void Connect(void);
	void Disconnect();
	void CALLBACK MyDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
	//////////////////////////////////////////////////
	//�������� ��� ������������������
	delegate void SetText(String^ text,bool simDisplay);
	delegate void disconnectDelegate();
	//delegate void PositionControl(int percent);
	delegate void DSetFormControl(System::Windows::Forms::Control^ ControlObject, Object^ ControlProperty, Object value);
	//////////////////////////////////////////////////
	//������� ���������� ���� � ������ -180 +180 ��������
	double GetAnglePM180(double angle)
	{
		double Tangle;
		Tangle = angle;
		while (Tangle > 180)
		{
			Tangle = Tangle - 360;
		}
		while (Tangle < -180)
		{
			Tangle = Tangle + 360;
		}
		return Tangle;
	}
	//������ ���������� �� ����� � ����� wayType
	double GetDistanceToWayType(int wayType)
	{
		int icw = CurrentWay;
		double D = CurrentPos.D;
		while ((Ways[icw]->typePath != GSWAY) && (icw < (CountWays - 1)))
		{
			D += Ways[icw + 1]->D;
			icw++;
		}
		return D;
	}

	//������ ��������� �� ����� ����
	double CalcToNewWay(bool changeWay = true)
	{
		double REangle = 0.045;
		if ((CurrentWay + 1) < CountWays)
		{
			REangle = Ways[CurrentWay + 1]->SH - Ways[CurrentWay]->EH;
		}
		REangle = GetAnglePM180(REangle);
		REangle = -REangle * 5 * PMDG_TEST::speeds.AS / 100000;
		//������ ���� ���������� (EangleRel) � ����� �������� �� ���������
		double EangleRel = CurrentPos.EH - Ways[CurrentWay]->EH;
		EangleRel = GetAnglePM180(EangleRel);

		//�������� ������� ���� ���� ���������� ������ 60 ��������, ���� �������� ����� ��������,
		// �� ������������� �� ��������� ����. 
		if (changeWay)
		{
			if ((EangleRel > 60) || (EangleRel < -60) || (CurrentPos.D < REangle))
			{
				CurrentWay = CurrentWay + 1;


			}
		}
		return REangle;
	}
	//������� ������� ������� 
	//���������� true ���� ����� �������
	//������� ���������:
	//	IDREQ - ������������� �������
	//	TimeOffset - ����� ������� � ������������	
	bool SetTimeOff(int IDREQ, int TimeOffset)
	{
		int i = 0;
		int Find = -1;
		clock_t eTime = clock();
		struct sTimerOffs
		{
			int IDReq;
			clock_t sTime;
			int TimeOff;
		};
		static sTimerOffs IDs[100];
		for (i = 0; i < 100; i++)
		{
			if (IDs[i].IDReq == IDREQ)
			{
				Find = i;
				break;
			}
		}
		
		if (Find == -1)
		{
			for (i = 0; i < sizeof(IDs); i++)
			{
				if (IDs[i].IDReq == 0)
				{
					Find = i;
					break;
				}
			}
			IDs[Find].IDReq = IDREQ;
			IDs[Find].sTime = clock();
			IDs[Find].TimeOff = TimeOffset;
			return false;
		}
		else
		{
			if ((IDs[Find].sTime + IDs[Find].TimeOff) < eTime)
			{
				IDs[Find].IDReq = 0;
				return true;
			}
			else
			{
				return false;
			}
		}
	}
	// SetTimeOff

	//������� ������ ��� ������
	void AddText(String^ text, bool simDisplay=false)
	{	
		if (MyForm::mainForm->textBox1->InvokeRequired)
		{
			SetText^ d = gcnew SetText(AddText);
			MyForm::mainForm->Invoke(d, gcnew array<Object^> {text,simDisplay});
		}
		else
		{
			// ������� c richTextBox1 ��� ������
			if (simDisplay)
			{
				//const char ctext[] = text;
				char* ctext = (char*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(text).ToPointer();
				int szz = text->Length;
				SimConnect_Text(hSimConnect, SIMCONNECT_TEXT_TYPE_PRINT_GREEN, 10, EVENT_TEXT, text->Length + 1, ctext);
			}
			MyForm::mainForm->textBox1->AppendText(text);
		}
	}
	// AddText

	//������� ��������� ��������, ��������, �������� � ��. � B737NGX (���������� ������������. �������� �� ������ ����)
	bool setNGXData(int Event, int Parameter, int* DefaultParameter)
	{
		while (*DefaultParameter != Parameter)
		{

			if (Control.Event == 0)
			{
				if (Parameter < *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
				}
				else if (Parameter > *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
				}
				Control.Event = Event;		// = 69749

				SimConnect_SetClientData(hSimConnect, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION,
					0, 0, sizeof(PMDG_NGX_Control), &Control);
			}
			while (Control.Event != 0)
			{
				SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
				Sleep(10);
			}
		}
		return TRUE;
	}
	bool setNGXDataBool(int Event, bool Parameter, bool* DefaultParameter)
	{
		static int Counter = 0;
		while (*DefaultParameter != Parameter)
		{

			if (Control.Event == 0)
			{
				if (Parameter < *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
				}
				else if (Parameter > *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
				}
				Control.Event = Event;		// = 69749

				SimConnect_SetClientData(hSimConnect, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION,
					0, 0, sizeof(PMDG_NGX_Control), &Control);
			}
			while (Control.Event != 0)
			{
				SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
				Sleep(10);
			}
			Counter++;
			if (Counter > 0)
			{
				Counter = 0;
				return FALSE;
			}
		}
		return TRUE;
	}
	bool setNGXDataFloat(int Event, float Parameter, float* DefaultParameter)
	{
		while (*DefaultParameter != Parameter)
		{

			if (Control.Event == 0)
			{
				if (Parameter < *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
				}
				else if (Parameter > *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
				}
				Control.Event = Event;		// = 69749

				SimConnect_SetClientData(hSimConnect, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION,
					0, 0, sizeof(PMDG_NGX_Control), &Control);
			}
			while (Control.Event != 0)
			{
				SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
				Sleep(10);
			}
		}
		return TRUE;
	}
	bool setNGXDataDouble(int Event, double Parameter, double* DefaultParameter, double limU = 0.05, double limD = -0.05, bool direct = FALSE)
	{
		while (((*DefaultParameter - Parameter) <= limD) || ((*DefaultParameter - Parameter) >= limU))
		{

			if (Control.Event == 0)
			{
				if (direct)
				{
					if (Parameter < *DefaultParameter)
					{
						Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
					}
					else if (Parameter > *DefaultParameter)
					{
						Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
					}
				}
				else
				{
					if (Parameter < *DefaultParameter)
					{
						Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
					}
					else if (Parameter > *DefaultParameter)
					{
						Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
					}
				}
				Control.Event = Event;		// = 69749

				SimConnect_SetClientData(hSimConnect, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION,
					0, 0, sizeof(PMDG_NGX_Control), &Control);

			}
			while (Control.Event != 0)
			{
				SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
				Sleep(10);
			}
			Sleep(100);
			hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
			SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
			aircraftData* PaircraftData = &AircraftData;
			AddText("AircraftData.elevTrim: " + AircraftData.elevTrim + "\n");
		}
		return TRUE;
	}
	bool setNGXDataUShort(int Event, unsigned short Parameter, unsigned short* DefaultParameter)
	{
		while (*DefaultParameter != Parameter)
		{

			if (Control.Event == 0)
			{
				if (Parameter < *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
				}
				else if (Parameter > *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
				}
				Control.Event = Event;		// = 69749

				SimConnect_SetClientData(hSimConnect, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION,
					0, 0, sizeof(PMDG_NGX_Control), &Control);
			}
			while (Control.Event != 0)
			{
				SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
				Sleep(10);
			}
		}
		return TRUE;
	}

	bool setNGXDataShort(int Event, short Parameter, short* DefaultParameter)
	{
		static int Counter = 0;
		while (*DefaultParameter != Parameter)
		{

			if (Control.Event == 0)
			{
				if (Parameter > *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
				}
				else if (Parameter < *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
				}
				Control.Event = Event;		// = 69749

				SimConnect_SetClientData(hSimConnect, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION,
					0, 0, sizeof(PMDG_NGX_Control), &Control);
			}
			while (Control.Event != 0)
			{
				SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
				Sleep(10);
			}
			Counter++;
			if (Counter > 0)
			{
				Counter = 0;
				return FALSE;
			}
		}
		return TRUE;
	}

	bool setNGXHeadShort(int Event, unsigned short Parameter, unsigned short* DefaultParameter)
	{
		while (*DefaultParameter != Parameter)
		{
			
			if (Control.Event == 0)
			{
				if ((Parameter - *DefaultParameter) > 180)
				{
					Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
				}
				else if ((Parameter - *DefaultParameter) < -180)
				{
					Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
				}
				else if (Parameter < *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
				}
				else if (Parameter > *DefaultParameter)
				{
					Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
				}
				Control.Event = Event;		// = 69749

				SimConnect_SetClientData(hSimConnect, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION,
					0, 0, sizeof(PMDG_NGX_Control), &Control);
			}
			while (Control.Event != 0)
			{
				SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
				Sleep(10);
			}
		}
		return TRUE;
	}

	bool setNGXDataUChar(int Event, unsigned char Parameter, unsigned char* DefaultParameter, bool direct = FALSE)
	{
		while (*DefaultParameter != Parameter)
		{
			if (direct)
			{
				if (Control.Event == 0)
				{
					if (Parameter > *DefaultParameter)
					{
						Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
					}
					else if (Parameter < *DefaultParameter)
					{
						Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
					}
					Control.Event = Event;		// = 69749

					SimConnect_SetClientData(hSimConnect, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION,
						0, 0, sizeof(PMDG_NGX_Control), &Control);
				}
			}
			else
			{
				if (Control.Event == 0)
				{
					if (Parameter > *DefaultParameter)
					{
						Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
					}
					else if (Parameter < *DefaultParameter)
					{
						Control.Parameter = MOUSE_FLAG_RIGHTSINGLE;
					}
					Control.Event = Event;		// = 69749

					SimConnect_SetClientData(hSimConnect, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION,
						0, 0, sizeof(PMDG_NGX_Control), &Control);
				}
			}
			while (Control.Event != 0)
			{
				SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
				Sleep(1);
			}
		}
		return TRUE;
	}
	// ������� ��������� ��������

	//������� ����������������� ���������� ����� � SimConnect
	void disconnectSafe()
	{
		if (MyForm::mainForm->InvokeRequired)
		{
			disconnectDelegate^ d = gcnew disconnectDelegate(Disconnect);
			MyForm::mainForm->Invoke(d);
		}
		else
		{
			Disconnect();
		}
	}
	//disconnectSafe

	//������� ����������������� ��������� ���������� ���
	void SetPositionControl(int percent)
	{
		MyForm::mainForm->SetControlSafe("vScrollBar1", "Value", percent);
		/*if (MyForm::mainForm->vScrollBar1->InvokeRequired)
		{
			PositionControl^ d = gcnew PositionControl(SetPositionControl);
			MyForm::mainForm->vScrollBar1->Invoke(d, gcnew int{ percent });
		}
		else
		{
			MyForm::mainForm->vScrollBar1->Value = percent;
		}*/
	}
}