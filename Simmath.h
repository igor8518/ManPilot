#include "SimConnect.h"
#define _USE_MATH_DEFINES 
#include <math.h>
namespace PMDG_TEST
{
	struct DSHEH
	{
		SIMCONNECT_DATA_LATLONALT Slla;
		SIMCONNECT_DATA_LATLONALT Ella;
		double D;
		double SH;
		double EH;
		double speedLimit;
		int typePath;
		char * nameEndPoint;
	};
	void DOrtoKM(DSHEH* dsheh)
	{
		double SLatitude = dsheh->Slla.Latitude / 180 * M_PI;
		double SLongitude = dsheh->Slla.Longitude / 180 * M_PI;
		double ELatitude = dsheh->Ella.Latitude / 180 * M_PI;
		double ELongitude = dsheh->Ella.Longitude / 180 * M_PI;
		double dO = (acos(sin(SLatitude)*sin(ELatitude) + cos(SLatitude)*cos(ELatitude)*cos(ELongitude - SLongitude))) * 180 / M_PI*111.12;
		dsheh->D = dO;
		dsheh->SH = M_PI_2 - atan(cos(SLatitude)*tan(ELatitude) / sin(ELongitude - SLongitude) - sin(SLatitude) / tan(ELongitude - SLongitude));
		dsheh->EH = M_PI_2 - atan(sin(ELatitude) / tan(ELongitude - SLongitude) - cos(ELatitude)*tan(SLatitude) / sin(ELongitude - SLongitude));
		dsheh->SH = dsheh->SH * 180 / M_PI;
		dsheh->EH = dsheh->EH * 180 / M_PI;
		if ((dsheh->Ella.Longitude - dsheh->Slla.Longitude) == 0)
		{
			dsheh->SH = 0;
			dsheh->EH = 0;
		}
		if (((dsheh->Ella.Longitude - dsheh->Slla.Longitude)<0))
		{
			dsheh->SH = dsheh->SH + 180;
			dsheh->EH = dsheh->EH + 180;
		}
		else if (((dsheh->Ella.Latitude - dsheh->Slla.Latitude) == 0) && ((dsheh->Ella.Longitude - dsheh->Slla.Longitude) < 0))
		{
			dsheh->SH = dsheh->SH + 180;
			dsheh->EH = dsheh->EH + 180;
		}
		else if (((dsheh->Ella.Longitude - dsheh->Slla.Longitude) == 0) && ((dsheh->Ella.Latitude - dsheh->Slla.Latitude) < 0))
		{
			dsheh->SH = dsheh->SH + 180;
			dsheh->EH = dsheh->EH + 180;
		}
		if (dsheh->SH > 360)
		{
			dsheh->SH = dsheh->SH - 360;
		}
		if (dsheh->SH < -360)
		{
			dsheh->SH = dsheh->SH + 360;
		}
		if (dsheh->EH > 360)
		{
			dsheh->EH = dsheh->EH - 360;
		}
		if (dsheh->EH < -360)
		{
			dsheh->EH = dsheh->EH + 360;
		}
		
	}
}