#include "Util.h"
namespace CSWrap
{
	const char* Util::GetPMDGEventNameFromID(int ID)
	{
		return CSWrap::Format("#" + Convert::ToString(ID));
	}
	HRESULT Util::SimConnect_MapClientEventToPMDGEvent(HANDLE hSimConnect, int ID)
	{
		return SimConnect_MapClientEventToSimEvent(hSimConnect, ID, Util::GetPMDGEventNameFromID(ID));
	}
}
