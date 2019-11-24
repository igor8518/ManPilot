#pragma once
#include "AirportData.h"
#include <SimConnect.h>
//#include "Airport.h"
#if !defined(AIRPORTAPI)
#define AIRPORTAPI extern "C" IAirportData* __stdcall
#endif


#pragma pack(push, 1)

struct DATA_RUNWAY
{
	double  sLatitude;
	double  sLongitude;
	double  sHeading;
	double  eLatitude;
	double  eLongitude;
	double  eHeading;
	std::string Name;
};
struct BGLData
{
	WORD ID;
	DWORD SizeOfRecord;
};
struct AirportInfo: public BGLData
{
	BYTE RunwayCount;
	BYTE COMCount;
	BYTE StartCount;
	BYTE ApproachCount;
	BYTE ApronCount;
	BYTE HelipadCount;
	DWORD Lon;
	DWORD Lat;
	DWORD Alt;
	DWORD TLon;
	DWORD TLat;
	DWORD TAlt;
	FLOAT MagDeg;
	DWORD ICAO;
	DWORD Region;
	DWORD FuelType;
	BYTE Unk1;
	BYTE TrafficScalar;
	WORD Unk2;
};

struct AirportInfoFS9 : public BGLData
{
	BYTE RunwayCount;
	BYTE COMCount;
	BYTE StartCount;
	BYTE ApproachCount;
	BYTE ApronCount;
	BYTE HelipadCount;
	DWORD Lon;
	DWORD Lat;
	DWORD Alt;
	DWORD TLon;
	DWORD TLat;
	DWORD TAlt;
	FLOAT MagDeg;
	DWORD ICAO;
	DWORD Region;
	DWORD FuelType;
};

struct TaxiwayPoints 
{
	BYTE Type;
	BYTE Flag;
	WORD Unk1;
	DWORD Lon;
	DWORD Lat;
};
struct TaxiwayPointsHeadrer : public BGLData
{

	WORD CountTaxiwayPoints;
};
struct TaxiwayParksCodes
{
	char AirlineDesignator[4];
};
struct TaxiwayParks 
{
	DWORD TaxiParkInfo;
	FLOAT Radius;
	FLOAT Heading;
	FLOAT TeeOffset1;
	FLOAT TeeOffset2;
	FLOAT TeeOffset3;
	FLOAT TeeOffset4;
	DWORD Lon;
	DWORD Lat;
	std::vector<TaxiwayParksCodes>* PTaxiwayParksCodes;
};
struct TaxiwayParksFS2004
{
	DWORD TaxiParkInfo;
	FLOAT Radius;
	FLOAT Heading;
	DWORD Lon;
	DWORD Lat;
	std::vector<TaxiwayParksCodes>* PTaxiwayParksCodes;
};


struct TaxiwayParksHeader : public BGLData
{

	WORD CountTaxiwayParks;
};

struct TaxiwayPaths 
{
	WORD IndexStartPoint;
	WORD IndexEndPoint;
	BYTE Type;
	BYTE TaxiNameIndex;
	BYTE Dashed;
	BYTE Surface;
	FLOAT Width;
	FLOAT WeightLimit;
	DWORD Unk1;
};
struct TaxiwayPathsHeader : public BGLData
{
	WORD CountTaxiwayParks;
};

struct FIXX
{
	std::string Name;
	double Lat;
	double Lon;
};

struct SRNW
{
	std::string Name;
};
struct WayPointA
{
	std::string TypeName;
	DWORD TYPE;
	double TRK;
	std::string FIXName;
	double ALT;
	double ALT2;
	double SPEED;
	double RADIAL;
	double LegTime;
	double LegDist;
};

struct TRANSITION
{
	std::string Name;
	std::vector<WayPointA>* WayPoints;
};
struct Points
{
	std::string Name;
	std::vector<std::string>* Runways;
	std::vector<WayPointA>* WayPoints;
	//std::vector<TRANSITION>* WayPoints;
	std::vector<TRANSITION>* TRANSITIONS;
};

struct SIDSTAR 
{
	std::vector<FIXX>* FIXES;
	std::vector<SRNW>* RNWS;
	std::vector<Points>* SIDS;
	std::vector<Points>* STARS;
	std::vector<Points>* APPROACHES;
};

struct TaxiwayNames 
{
	char TaxiName[8];
};
struct TaxiwayNamesHeader : public BGLData
{
	WORD CountTaxiwayNames;
};

struct RunwayPaths
{
	WORD Runway;
	std::string RunwayName1;
	std::string RunwayName2;
	std::vector<WORD> Path;
};

struct AirportList
{
	DWORD Version;
	std::string ICAO;
	std::wstring File;
	DWORD FileOffset;
	double lat;
	double Lon;
	WORD Layer;

};
struct TPath
{
	DWORD index;
	std::string name;
	double Lat;
	double Lon;
	WORD Type;
};

struct AirportPaths
{
	std::vector<int>* Starts;
	AirportInfo* PAirportInformation = NULL;
	TaxiwayPointsHeadrer* PHTaxiwayPoints = NULL;
	std::vector<TaxiwayPoints>* PTaxiwayPoints = NULL;
	TaxiwayParksHeader* PHTaxiwayParks = NULL;
	std::vector<TaxiwayParks>* PTaxiwayParks = NULL;
	TaxiwayPathsHeader* PHTaxiwayPaths = NULL;
	std::vector<TaxiwayPaths>* PTaxiwayPaths = NULL;
	TaxiwayNamesHeader* PHTaxiwayNames = NULL;
	std::vector<TaxiwayNames>* PTaxiwayNames = NULL;
	std::vector<RunwayPaths>* RPth;
};

#pragma pack(pop)

struct IAirport
{

public:
	std::vector<int>* Starts;
	AirportInfo* PAirportInformation = NULL;
	TaxiwayPointsHeadrer* PHTaxiwayPoints = NULL;
	std::vector<TaxiwayPoints>* PTaxiwayPoints = NULL;
	TaxiwayParksHeader* PHTaxiwayParks = NULL;
	std::vector<TaxiwayParks>* PTaxiwayParks = NULL;
	TaxiwayPathsHeader* PHTaxiwayPaths = NULL;
	std::vector<TaxiwayPaths>* PTaxiwayPaths = NULL;
	TaxiwayNamesHeader* PHTaxiwayNames = NULL;
	std::vector<TaxiwayNames>* PTaxiwayNames = NULL;
	std::vector<RunwayPaths>* RPth;
	SIDSTAR* sidstar = NULL;
	
};

struct IAirportData
{
	virtual UINT FillDB() = 0;
	virtual int GetProgress() = 0;
	virtual void Release() = 0;
	virtual AirportList GetNearAirport(double Lat, double Lon) = 0;
	virtual std::vector<RunwayPaths>* GetRunways() = 0;
	virtual std::vector<int>* GetRunwayStart(double lat, double lon, double direction) = 0;
	virtual std::vector<TPath> GetPath(DWORD start, DWORD heading, double direction) = 0;
	virtual DATA_RUNWAY GetStartCoord(int index) = 0;
	virtual DWORD GetNearTaxiwayPoint(double Lat, double Lon) = 0;
	virtual DWORD GetNearTaxiwayPath(double Lat, double Lon, double heading) = 0;
	virtual std::vector<TPath> GetPathGate(DWORD path, DWORD heading, double radius, TaxiwayParks * park) = 0;
	virtual void SetHSim(HANDLE hSim) = 0;
	virtual void DispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void * pContext) = 0;
	//std::vector<AirportList>* PAirpotList;
	virtual IAirport * GetAirport() = 0;
	std::vector<double>* d = NULL;
	std::vector<TPath>* ReturnPath;
	std::wstring RootSim;
};


AIRPORTAPI GetAirportData();




