#include "TestEventsForm.h"
#include "MainForm.h"
#include "PMDGEventsArray.h"


namespace PMDGAuto
{
	// Send mouse event to the PMDG
	HRESULT PMDGAuto::TestEventsForm::SendMouseEvent(int eventMouse)
	{
		try
		{
			if (MainForm::PFSLink)
			{
				if (MainForm::PFSLink->hSimConnect)
				{	
					TESTHR(SimConnect_TransmitClientEvent(MainForm::PFSLink->hSimConnect, 0, evemtList->SelectedIndex, eventMouse,
						SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY),"SimConnect_TransmitClientEvent");
					return NOERROR;
				}
			}
		}
		catch (HRESULT hr)
		{
			return hr;
		}
		return 1;
	}
}
