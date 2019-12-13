#include "stdafx.h"

#include "LED.h"

// Main functions

int __stdcall
DLLStart(void)
{
	::led = new Led();
	return 0;
}




void __stdcall
DLLStop(void)
{

}








// Callbacks

void CALLBACK 
::myDispatchProcDll(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
	if (::led) {
		::led->setCount(::led->getCount() + 1);
		

		if (GetModuleHandleExW(0, L"PMDG_737NGX.dll", ::led->getPmdgPoint()))
		{
			#ifdef DEBUG

				if (::led->getFirstRunStatus())
				{
					MessageBoxExW(NULL, L"PMDG 737 Loaded: ", L"DLL Module", MB_ICONINFORMATION, LANG_ENGLISH);
					::led->invertFirstRunStatus();
				}
				//PimpGDIP = (impGDIP*)((INT64)PPMDG + 0x3A3F88); 0x19C592
				::led->setImpGdipPoint((::Led::ImpGdip*)((INT64)::led->getPmdgPoint() + 0x3A4F88));
				//.idata:00000001803A4F88 ; Imports from gdiplus.dll
				//.idata:00000001803A4F88;
				//.idata:00000001803A4F88                 extrn __imp_GdipAddPathLine2 : qword
				//	.idata : 00000001803A4F88; DATA XREF : GdipAddPathLine2r
				//	.idata:00000001803A4F88;.rdata:000000018041C938o
				//PPData = (Data**)((INT64)PPMDG + 0x4a4510);

				//led->GetDataPointPoint = (Led::Data**)((INT64)led->getPmdgPoint + 0x4A44F0);
				//.text:0000000180051689                 nop
				//.text:000000018005168A                 mov     rcx, [rdi + 308h]; Memory
				//	.text:0000000180051691                 call    sub_180084850

				//lea     rdx, a58; "58 "
				//	.text:000000018005116C                 call    sub_180226110
				//	.text:0000000180051171                 call    sub_180084890
				//PPMainData = (sMainData**)((INT64)PPMDG + 0x4a3a10);

				//led->getMainDataPointPoint = (Led::MainData**)((INT64)led->getPmdgPoint + 0x4A39F0);
				//.text:00000001800511C5                 lea     rdx, a61; "61 "
				//	.text:00000001800511CC                 call    sub_180226110
				//	.text:00000001800511D1                 call    sub_18007DF40
				DWORD op;
				VirtualProtect(&::led->getImpGdipPoint()->DrawLine1, 8, PAGE_READWRITE, &op);
				::led->getImpGdipPoint()->DrawLine1 = &::Led::gdipDrawLineLed;
				//PimpGDIP->GDIStart = 
				//WriteProcessMemory()
				//VirtualProtect(&PimpGDIP->DrawLine1, 8, op, &op);

			#endif // DEBUG

			::led->setDataPointPoint((Led::Data**)((INT64)::led->getPmdgPoint() + 0x4A44F0));
			::led->setMainDataPointPoint((Led::MainData**)((INT64)::led->getPmdgPoint() + 0x4A39F0));

			switch (pData->dwID)
			{
				case SIMCONNECT_RECV_ID::SIMCONNECT_RECV_ID_EVENT:
				{
					SIMCONNECT_RECV_EVENT* evt = (SIMCONNECT_RECV_EVENT*)pData;

					switch (evt->uEventID)
					{
						case Led::EVENT_ID::E_6HZ:	// Track aircraft changes
						{
							Led::ExportData ed;
							::led->setMainDataPoint(*::led->getMainDataPointPoint());
							::led->setDataPoint(*::led->getDataPointPoint());
							INT64 MainB0 = ::led->getMainDataPoint()->b0;
							ed.HorEnabled = *(byte*)((INT64)::led->getDataPoint() + (12 * MainB0) + 0x880);
							ed.VerEnabled = *(byte*)((INT64)::led->getDataPoint() + (12 * MainB0) + 0x898);
							ed.HorBar = ::led->getDataPoint()->HorBar;
							ed.VerBar = ::led->getDataPoint()->VerBar;
							::led->setExportData(ed);
							led->setHr(SimConnect_SetClientData(hSimConnect, ::PMDG_ADD_DATA_ID, ::PMDG_ADD_DATA_DEFINITION, 0, 0, sizeof(ed), &ed));
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
}






// Methods of led

Led::Led()
{
	

	if (setHr(SimConnect_Open(&::hSimConnect, "LED.DLL", 0, 0, 0, 0)))
	{
		
		/*LPWSTR s;
		swprintf(s, 50, L"Connected... %.2f", 0.454553);

		CreateConsole();
		PrintOnConsole(s);*/

		setHr(SimConnect_MapClientEventToSimEvent(::hSimConnect, EVENT_ID::KEYBOARD_A));
		setHr(SimConnect_AddClientEventToNotificationGroup(::hSimConnect, GROUP_ID::KEYBOARD, EVENT_ID::KEYBOARD_A));
		setHr(SimConnect_SetNotificationGroupPriority(::hSimConnect, GROUP_ID::KEYBOARD, SIMCONNECT_GROUP_PRIORITY_HIGHEST));
		setHr(SimConnect_MapInputEventToClientEvent(::hSimConnect, INPUT_ID::I_0, "shift+ctrl+a", EVENT_ID::KEYBOARD_A));
		setHr(SimConnect_SetInputGroupState(::hSimConnect, INPUT_ID::I_0, SIMCONNECT_STATE_ON));
		setHr(SimConnect_CallDispatch(::hSimConnect, myDispatchProcDll, NULL));
		setHr(SimConnect_MapClientDataNameToID(::hSimConnect, ::PMDG_ADD_DATA_NAME, ::PMDG_ADD_DATA_ID));
		setHr(SimConnect_CreateClientData(::hSimConnect, ::PMDG_ADD_DATA_ID, sizeof(ExportData), SIMCONNECT_CREATE_CLIENT_DATA_FLAG_READ_ONLY));
		setHr(SimConnect_AddToClientDataDefinition(::hSimConnect, PMDG_ADD_DATA_DEFINITION, 0, sizeof(ExportData)));
		setHr(SimConnect_SubscribeToSystemEvent(::hSimConnect, EVENT_ID::E_6HZ, "6Hz"));
	}
}



#ifdef DEBUG

Gdiplus::Status WINGDIPAPI
Led::gdipDrawLineLed(GpGraphics* graphics, GpPen* pen, REAL x1, REAL y1, REAL x2, REAL y2)
{
	ARGB color;
	DllExports::GdipGetPenColor(pen, &color);
	Gdiplus::Color cPen = Gdiplus::Color(color);

	return DllExports::GdipDrawLine(graphics, pen, x1, y1, x2, y2);
}
#endif // DEBUG




COUNT 
Led::getCount()
{
	return iCount_;
}




void
Led::setCount(COUNT iCount)
{
	iCount_ = iCount;
}

bool 
Led::setHr(HRESULT hr)
{
	hr_ = hr;
	return SUCCEEDED(hr_);
}




#ifdef DEBUG
bool
Led::getFirstRunStatus()
{
	return isFirstRun_;
}




void
Led::invertFirstRunStatus()
{
	isFirstRun_ = !isFirstRun_;
}




Led::ImpGdip* 
Led::getImpGdipPoint()
{
	return pImpGdip_;
}




void 
Led::setImpGdipPoint(ImpGdip* pImpGdip)
{
	pImpGdip_ = pImpGdip;
}
#endif // DEBUG




Led::Data** 
Led::getDataPointPoint()
{
	return ppData_;
}




Led::Data* 
Led::getDataPoint()
{
	return pData_;
}




void
Led::setDataPoint(Data* pData)
{
	pData_ = pData;
}





void 
Led::setDataPointPoint(Data** ppData)
{
	ppData_ = ppData;
}




Led::MainData**
Led::getMainDataPointPoint()
{
	return ppMainData_;
}




Led::MainData* 
Led::getMainDataPoint()
{
	return pMainData_;
}




void
Led::setMainDataPoint(MainData* pMainData)
{
	pMainData_ = pMainData;
}




void 
Led::setMainDataPointPoint(MainData** ppMainData)
{
	ppMainData_ = ppMainData;
}




HMODULE*
Led::getPmdgPoint()
{
	return &pPmdg_;
}




void 
Led::setExportData(ExportData exportData)
{
	exportData_ = exportData;
}



#ifdef DEBUG
void
Led::CreateConsole_()
{
	if (!hOut) {
		AllocConsole();
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	}

}




void
Led::DestroyConsole_()
{
	if (hOut) {
		FreeConsole();
		hOut = NULL;
	}

}




void
Led::PrintOnConsole_(LPCWSTR pTxt)
{
	if (hOut) {
		DWORD NumberOfCharsWritten;
		WriteConsole(hOut, pTxt, lstrlen(pTxt), &NumberOfCharsWritten, NULL);
	}
}
#endif

