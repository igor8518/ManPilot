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
bool f = false;
Status WINGDIPAPI GdipDrawLineLED(GpGraphics *graphics, GpPen *pen, REAL x1, REAL y1, REAL x2, REAL y2)
{
	ARGB color;	
	DllExports::GdipGetPenColor(pen, &color);
	Gdiplus::Color cPen = Gdiplus::Color(color);

	
	WCHAR  s[1024];
	if ((cPen.GetGreen() <= 133) && (cPen.GetGreen() >= 124))
	{
		
		
		swprintf(s, 100, L"DrawLine (%d, %d, %d)", cPen.GetRed(), cPen.GetGreen(), cPen.GetBlue());
		//PrintOnConsole(s);
		swprintf(s, 100, L" - POS: (%f,%f) - (%f,%f)\n", x1, y1, x2, y2);
		//PrintOnConsole(s);
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
		if (!f)
		{
			MessageBoxExW(NULL, L"PMDG 737 Loaded: ", L"DLL Module", MB_ICONINFORMATION, LANG_ENGLISH);
			f = true;
		}
		//PimpGDIP = (impGDIP*)((INT64)PPMDG + 0x3A3F88); 0x19C592
		PimpGDIP = (impGDIP*)((INT64)PPMDG + 0x3A4F88); 
		//.idata:00000001803A4F88 ; Imports from gdiplus.dll
		//.idata:00000001803A4F88;
		//.idata:00000001803A4F88                 extrn __imp_GdipAddPathLine2 : qword
		//	.idata : 00000001803A4F88; DATA XREF : GdipAddPathLine2r
		//	.idata:00000001803A4F88;.rdata:000000018041C938o
		//PPData = (Data**)((INT64)PPMDG + 0x4a4510);
		//PPData = (Data**)((INT64)PPMDG + 0x4A44F0);
    PPData = (Data**)((INT64)PPMDG + 0x607928);
		//.text:0000000180051689                 nop
		//.text:000000018005168A                 mov     rcx, [rdi + 308h]; Memory
		//	.text:0000000180051691                 call    sub_180084850

		//lea     rdx, a58; "58 "
		//	.text:000000018005116C                 call    sub_180226110
		//	.text:0000000180051171                 call    sub_180084890
		//PPMainData = (sMainData**)((INT64)PPMDG + 0x4a3a10);
		PPMainData = (sMainData**)((INT64)PPMDG + 0x4A39F0);
		//.text:00000001800511C5                 lea     rdx, a61; "61 "
		//	.text:00000001800511CC                 call    sub_180226110
		//	.text:00000001800511D1                 call    sub_18007DF40
		VirtualProtect(&PimpGDIP->DrawLine1,8,PAGE_READWRITE,&op);
		PimpGDIP->DrawLine1 = &GdipDrawLineLED;
		//PimpGDIP->GDIStart = 
		//WriteProcessMemory()
		//VirtualProtect(&PimpGDIP->DrawLine1, 8, op, &op);

		
		switch (pData->dwID)
		{
		case SIMCONNECT_RECV_ID_EVENT:
		{
										 SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

										 switch (evt->uEventID)
										 {
										 case EVENT_6HZ:	// Track aircraft changes
										 {

																//PMainData = *PPMainData;
																PData = *PPData;
																//INT64 MainB0 = PMainData->b0;
																//ExportData.HorEnabled = *(byte*)((INT64)PData + (12 * MainB0) + 0x880);
																//ExportData.VerEnabled = *(byte*)((INT64)PData + (12 * MainB0) + 0x898);
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
  if (GetModuleHandleExW(0, L"PMDG_737NGXu.dll", &PPMDG))
  {
    if (!f)
    {
      MessageBoxExW(NULL, L"PMDG 737NGXu Loaded: ", L"DLL Module", MB_ICONINFORMATION, LANG_ENGLISH);
      f = true;
    }
    //PimpGDIP = (impGDIP*)((INT64)PPMDG + 0x3A3F88); //0x19C592
    //bool hrb = GetModuleHandleExW(0, L"GDIPlus.dll", &GDIP);
    //lpfnDrawLinelOrig = (LPDRAWLINEL)GetProcAddress(GDIP, "GdipDrawLinel");
    
    PimpGDIP = (impGDIP*)((INT64)PPMDG + 0x455800);
    //.idata:00000001803A4F88 ; Imports from gdiplus.dll
    //.idata:00000001803A4F88;
    //.idata:00000001803A4F88                 extrn __imp_GdipAddPathLine2 : qword
    //	.idata : 00000001803A4F88; DATA XREF : GdipAddPathLine2r
    //	.idata:00000001803A4F88;.rdata:000000018041C938o
    //PPData = (Data**)((INT64)PPMDG + 0x4a4510);
    //PPData = (Data**)((INT64)PPMDG + 0x4A44F0);
    PPData = (Data**)((INT64)PPMDG + 0x607928);
    //.text:0000000180051689                 nop
    //.text:000000018005168A                 mov     rcx, [rdi + 308h]; Memory
    //	.text:0000000180051691                 call    sub_180084850

    //lea     rdx, a58; "58 "
    //	.text:000000018005116C                 call    sub_180226110
    //	.text:0000000180051171                 call    sub_180084890
    //PPMainData = (sMainData**)((INT64)PPMDG + 0x4a3a10);
    PPMainData = (sMainData**)((INT64)PPMDG + 0x607878);
    //.text:00000001800511C5                 lea     rdx, a61; "61 "
    //	.text:00000001800511CC                 call    sub_180226110
    //	.text:00000001800511D1                 call    sub_18007DF40
    VirtualProtect(&PimpGDIP->DrawLine1, 8, PAGE_READWRITE, &op);
    PimpGDIP->DrawLine1 = &GdipDrawLineLED;
    //PimpGDIP->GDIStart = 
    //WriteProcessMemory()
    //VirtualProtect(&PimpGDIP->DrawLine1, 8, op, &op);


    switch (pData->dwID)
    {
    case SIMCONNECT_RECV_ID_EVENT:
    {
      SIMCONNECT_RECV_EVENT* evt = (SIMCONNECT_RECV_EVENT*)pData;

      switch (evt->uEventID)
      {
      case EVENT_6HZ:	// Track aircraft changes
      {

        PMainData = *PPMainData;
        PData = *PPData;
        INT64 MainB0 = PMainData->b0;
        ExportData.HorEnabled = *(byte*)((INT64)PData + (12 * MainB0) + 0x8D0);
        ExportData.VerEnabled = *(byte*)((INT64)PData + (12 * MainB0) + 0x8E8);
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
		float someFloat = 0.454553;
		swprintf(s, 50, L"Connected... %.2f", someFloat );
		
		//CreateConsole();
		//PrintOnConsole(s);
		

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

