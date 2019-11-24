#include "PMDG_NGX.h"

void ManPitchWithFD(double NPitch)
{
	static clock_t sTime = 0; //Начальное время
	clock_t eTime = clock();
	//Начальный тангаж
	static double sPitch;
	//Конечный тангаж
	double ePitch = PaircraftData->pitch;

	double timeOff = eTime - sTime;
	double pitchOff = ePitch - sPitch;
	double pitchA = (1000 / timeOff)*pitchOff;
	//TO DO
	double PitchRel;

	/*if ((PaircraftData->simOnGround == 0) && (PaircraftData->FDAct == 0))
	{
	PitchRel = PaircraftData->FDPitch - PaircraftData->pitch;
	AddText("PaircraftData->FDPitch: " + PaircraftData->FDPitch + "\n");
	}
	else*/
	{
		PitchRel = NPitch - PaircraftData->pitch;
		AddText("NPitch: " + NPitch + "\n");
	}
	if (PitchRel > 180)
	{
		PitchRel = PitchRel - 360;
	}
	else if (PitchRel < -180)
	{
		PitchRel = PitchRel + 360;
	}
	AddText("PitchRel: " + PitchRel + "\n");
	double pparameter = PitchRel / 5;
	////////////////////////////////////////////////////////
	AddText("pparameter: " + pparameter + "\n");
	AddText("pitchA: " + pitchA + "\n");
	//Расчет необходимого угла руля направления для удержания ускорения
	double elev = PaircraftData->elev * 16383;
	AddText("elev: " + elev + "\n");
	elev = -elev;
	double rel = ((pparameter - pitchA) * 7000);
	if (rel < -500)
	{
		rel = -500;
	}
	else if (rel > 500)
	{
		rel = 500;
	}
	elev = elev + rel;

	if (elev > 16383)
	{
		elev = 16383;
	}
	else if (elev < -16383)
	{
		elev = -16383;
	}
	//Инвертирование знака
	int intpParameter = elev;

	SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ELEVATOR_SET, intpParameter, \
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	sTime = clock();
	sPitch = PaircraftData->pitch;

}