#include "PMDG_NGX.h"
#include "WayLoader.h"
namespace PMDG_TEST{
	void FillWays()
	{
		//DSHEH *Ways[500];
		CountWays = 0;
		AddWayPoint(60.780871, 56.744834);
		AddWayPoint(60.780881, 56.748455);
		AddWayPoint(60.781651, 56.748680);
		AddWayPoint(60.795913, 56.748686);
		AddWayPoint(60.796385, 56.748564);
		AddWayPoint(60.805392, 56.748574);
		AddWayPoint(60.805990, 56.748653);
		AddWayPoint(60.817327, 56.748654);
		AddWayPoint(60.817987, 56.748630);
		AddWayPoint(60.821127, 56.747771);
		AddWayPoint(60.821593, 56.747442);
		//AddWayPoint(60.821604, 56.744757);
		//AddWayPoint(60.780871, 56.744834);
		AddWayPoint(60.821604, 56.744757,764, RUNWAY);
		AddWayPoint(60.780871, 56.744834,CruiseLevel, RUNWAY, "26R");
		AddWayPoint(61.0484, 56.6726, CruiseLevel, WAYPOINT, "EKB01");
		AddWayPoint(61.091, 56.7174, CruiseLevel, WAYPOINT, "EKB02");
		AddWayPoint(61.0557, 56.744, 2410, WAYPOINT,"CF26R");
		AddWayPoint(60.9521, 56.7447, 2410, WAYPOINT, "FN26R");
		AddWayPoint(60.8594, 56.7447, 1262, GSWAY, "MN26R");
		AddWayPoint(60.821604, 56.744757, 764, GSWAY, "26R");
		AddWayPoint(60.780871, 56.744834, 764, RUNWAY, "26R");
		Ways[CountWays] = new DSHEH;
		Ways[CountWays]->Slla.Longitude = Ways[CountWays - 1]->Ella.Longitude;
		Ways[CountWays]->Slla.Latitude = Ways[CountWays - 1]->Ella.Latitude;
		Ways[CountWays]->Slla.Altitude = Ways[CountWays - 1]->Ella.Altitude;
		Ways[CountWays]->Ella.Longitude = 0;
		Ways[CountWays]->Ella.Latitude = 0;
		Ways[CountWays]->Ella.Altitude = 0;
		Ways[CountWays]->typePath = 0;
		DOrtoKM(Ways[CountWays]);
	}
	void AddWayPoint(double lon, double lat, double alt, int pointType, char* PointName)
	{
		static int point = 0;
		if ((point == 0) && (CountWays == 0))
		{
			Ways[CountWays] = new DSHEH;
			Ways[CountWays]->Slla.Longitude = lon;
			Ways[CountWays]->Slla.Latitude = lat;
			Ways[CountWays]->Slla.Altitude = alt;
			point = 1;
		}
		else if ((point == 1) && (CountWays == 0))
		{
			Ways[CountWays]->Ella.Longitude = lon;
			Ways[CountWays]->Ella.Latitude = lat;
			Ways[CountWays]->Ella.Altitude = alt;
			Ways[CountWays]->typePath = pointType;
			Ways[CountWays]->nameEndPoint = PointName;
			DOrtoKM(Ways[CountWays]);
			CountWays++;
			point = 0;
		}
		else if (CountWays != 0)
		{
			Ways[CountWays] = new DSHEH;
			Ways[CountWays]->Slla.Longitude = Ways[CountWays-1]->Ella.Longitude;
			Ways[CountWays]->Slla.Latitude = Ways[CountWays-1]->Ella.Latitude;
			Ways[CountWays]->Slla.Altitude = Ways[CountWays-1]->Ella.Altitude;
			Ways[CountWays]->Ella.Longitude = lon;
			Ways[CountWays]->Ella.Latitude = lat;
			Ways[CountWays]->Ella.Altitude = alt;
			Ways[CountWays]->typePath = pointType;
			Ways[CountWays]->nameEndPoint = PointName;
			DOrtoKM(Ways[CountWays]);
			CountWays++;
		}
	}
}

