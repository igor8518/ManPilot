#pragma once
#include "CSWrap.h"
#include "SimConnect.h"
namespace CSWrap
{
	static ref class Util
	{
	public:
		static const char* GetPMDGEventNameFromID(int ID);
		static HRESULT SimConnect_MapClientEventToPMDGEvent(HANDLE hSimConnect, int ID);
	};
}

