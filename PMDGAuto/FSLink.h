#pragma once

#include <gcroot.h>
#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <strsafe.h>
#include "SimConnect.h"
#include "PMDG_NGX_SDK.h"
#include "PMDG_NGX_SDK_ADD.h"
#include "InitializeVars.h"
#include <time.h>
#include <algorithm>
#include "ErrorCatch.h"
//#include <string>
#include "CSWrap.h"
#include "Util.h"

namespace PMDGAuto
{
	using CSWrap::Util;
	ref class MainForm;

#define MForm MainForm::PMainForm
#define TESTHR(hr, title) \
	if (FAILED(hr)) \
	{\
		ErrorCatch::ShowError(title, hr);\
		throw(hr);\
	};	
	
	
	public class FSLink
	{
		public:
			static PMDG_NGX_Data GenNGXData;
			static PMDG_ADD_Data AddNGXData;
			static PMDG_NGX_Control GenNGXControl;
			int quit;
			
			//SimConnect handle
			HANDLE  hSimConnect = NULL;
			enum ConnectState
			{
				DISCONNECTED,
				CONNECTING,
				CONNECTED,
				DISCONNECTING
			} static;
			FSLink();
			~FSLink();
			HRESULT Dispatch();
			void Process(SIMCONNECT_RECV *pData, DWORD cbData);
			static void CALLBACK ReceiveCallBack(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext);
			static void CALLBACK TimerProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ UINT_PTR idEvent, _In_ DWORD dwTime);
			
	protected:
			
	private:
		
		typedef class {
		public:
			byte index;
			//int var;
		} SAVED_ARRAY;
		// //Sets map client data names to IDs
		UINT_PTR Timer;
		HRESULT InitSimConnect();
		bool firstReceive = true;
		byte savedArray[sizeof(PMDG_NGX_Data)];
		std::vector<int> currentChangedCounter = std::vector<int>(0);
		std::vector<int> currentChangedValue = std::vector<int>(0);;
		std::vector<int>::iterator it;
		int savedArrayVars[sizeof(PMDG_NGX_Data)];
		
		
		
		
		bool Connected = false;
		
	};
}

