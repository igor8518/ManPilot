// LED.cpp: определяет экспортированные функции для приложения DLL.
//


#include "stdafx.h"
#include "LED.h"
#include "simconnect.h"
#include <stdio.h>
#include <tchar.h>
#include <strsafe.h>
#include <tchar.h>
#include <string>
#include <stdio.h>
#include <gdiplus.h>
#include <gdiplusflat.h>
#include <stdio.h>

//#include <gdiplustypes.h>

Status WINGDIPAPI GdipDrawLineLED(GpGraphics *graphics, GpPen *pen, REAL x1, REAL y1, REAL x2, REAL y2)
{
	ARGB color;	
	DllExports::GdipGetPenColor(pen, &color);
	Gdiplus::Color cPen = Gdiplus::Color(color);

	
	WCHAR  s[1024];
	if (cPen.GetGreen() == 141)
	{
		
		
		swprintf(s, 100, L"DrawLine (%d, %d, %d)", cPen.GetRed(), cPen.GetGreen(), cPen.GetBlue());
		PrintOnConsole(s);
		swprintf(s, 100, L" - POS: (%f,%f) - (%f,%f)\n", x1, y1, x2, y2);
		PrintOnConsole(s);
		/*PData= *PPData;
		swprintf(s, 100, L" - FD HOR: (%.2f)\n", PData->HorBar);
		PrintOnConsole(s);*/
	}
	//return graphics->DrawLine(pen, x1, y1, x2, y2);
	//return Graphics::GdipDrawLine()
	//return GdipDrawLine(graphics, pen, x1, y1, x2, y2);
	return DllExports::GdipDrawLine(graphics,
		pen, x1, y1, x2,
		y2);
}
void CALLBACK MyDispatchProcDLL(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
	i++;
	DWORD op;
	//wsprintfW(s, L"Connected %d", i);
	//PrintOnConsole(s);
	//LoadLibraryExW(L"C:\\GAMES\\Lockheed Martin\\Prepar3D v3\\gauges\\PMDG_737NGX.dll", NULL, 0);
	
	if (GetModuleHandleExW(0, L"PMDG_737NGX.dll", &PPMDG))
	{

		//MessageBoxExW(NULL, L"PMDG 737 Loaded: ", L"DLL Module", MB_ICONINFORMATION,LANG_ENGLISH);
		PimpGDIP = (impGDIP*)((int)PPMDG + 0x2dd760);
		PPData = (Data**)((int)PPMDG + 0x3950c4);
		PPMainData = (sMainData**)((int)PPMDG + 0x394618);
		VirtualProtect(&PimpGDIP->DrawLine1,4,PAGE_READWRITE,&op);
		PimpGDIP->DrawLine1 = &GdipDrawLineLED;
		//PimpGDIP->GDIStart = 
		//WriteProcessMemory()
		VirtualProtect(&PimpGDIP->DrawLine1, 4, op, &op);

		
		switch (pData->dwID)
		{
		case SIMCONNECT_RECV_ID_EVENT:
		{
										 SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

										 switch (evt->uEventID)
										 {
										 case EVENT_6HZ:	// Track aircraft changes
										 {

																PMainData = *PPMainData;
																PData = *PPData;
																int MainA0 = PMainData->a0;
																ExportData.HorEnabled = *(byte*)((int)PData + (4 * ((3 * MainA0) + 0x210)));
																ExportData.VerEnabled = *(byte*)((int)PData + (4 * ((3 * MainA0) + 0x216)));
																ExportData.HorBar = PData->HorBar;
																ExportData.VerBar = PData->VerBar;
																hr = SimConnect_SetClientData(hSimConnect, PMDG_ADD_DATA_ID, PMDG_ADD_DATA_DEFINITION, 0, 0, sizeof(ExportData), &ExportData);
																break;
										 }
										 default:
										 {
													break;
										 }
										 }
		}

		default:
		{
				   break;
		}
		}
		
		
	}
}


int __stdcall DLLStart(void)
{
	WCHAR  s[1024];
	HRESULT hr;
	if (SUCCEEDED(SimConnect_Open(&hSimConnect, "DLL name", NULL, 0, NULL, 0)))
	{
		float someFloat = 0.434553;
		swprintf(s, 50, L"Connected... %.2f", someFloat );
		
		CreateConsole();
		PrintOnConsole(s);
		

		// Place all initialization code for the client in this function
		hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_KEYBOARD_A);
		hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYBOARD, EVENT_KEYBOARD_A);
		hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP_KEYBOARD, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
		hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+ctrl+a", EVENT_KEYBOARD_A);
		hr = SimConnect_SetInputGroupState(hSimConnect, INPUT0, SIMCONNECT_STATE_ON);
		hr = SimConnect_CallDispatch(hSimConnect, MyDispatchProcDLL, NULL);
		hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_ADD_DATA_NAME, PMDG_ADD_DATA_ID);
		hr = SimConnect_CreateClientData(hSimConnect, PMDG_ADD_DATA_ID, sizeof(ExportData), SIMCONNECT_CREATE_CLIENT_DATA_FLAG_READ_ONLY);
		hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_ADD_DATA_DEFINITION, 0, sizeof(ExportData));

		hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_6HZ, "6Hz");
	}
	return 0;
}



void __stdcall DLLStop(void)
{

}

