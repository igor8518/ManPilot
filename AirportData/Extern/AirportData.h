#pragma once
#include "AirportData.h"
#include <SimConnect.h>
//#include "Airport.h"
#if !defined(AIRPORTAPI)
#define AIRPORTAPI extern "C" IAirportData* __stdcall
#endif


#pragma pack(push, 1)

class DATA_RUNWAY {
	public:
		double		sLatitude;
		double		sLongitude;
		double		sHeading;
		double		eLatitude;
		double		eLongitude;
		double		eHeading;
		std::string Name;
};

class BGLData {
	public:
		WORD	ID;
		DWORD	SizeOfRecord;
};

class AirportInfo: public BGLData {
	public:
		BYTE	RunwayCount;
		BYTE	COMCount;
		BYTE	StartCount;
		BYTE	ApproachCount;
		BYTE	ApronCount;
		BYTE	HelipadCount;
		DWORD	Lon;
		DWORD	Lat;
		DWORD	Alt;
		DWORD	TLon;
		DWORD	TLat;
		DWORD	TAlt;
		FLOAT	MagDeg;
		DWORD	ICAO;
		DWORD	Region;
		DWORD	FuelType;
		BYTE	Unk1;
		BYTE	TrafficScalar;
		WORD	Unk2;
};

class AirportInfoFS9 : public BGLData {
	public:
		BYTE	RunwayCount;
		BYTE	COMCount;
		BYTE	StartCount;
		BYTE	ApproachCount;
		BYTE	ApronCount;
		BYTE	HelipadCount;
		DWORD	Lon;
		DWORD	Lat;
		DWORD	Alt;
		DWORD	TLon;
		DWORD	TLat;
		DWORD	TAlt;
		FLOAT	MagDeg;
		DWORD	ICAO;
		DWORD	Region;
		DWORD	FuelType;
};

class TaxiwayPoints {
	public:
		BYTE	Type;
		BYTE	Flag;
		WORD	Unk1;
		DWORD	Lon;
		DWORD	Lat;
};

class TaxiwayPointsHeadrer : public BGLData {
	public:
		WORD	CountTaxiwayPoints;
};

class TaxiwayParksCodes {
	public:
		char	AirlineDesignator[4];
};

class TaxiwayParks {
	public:
		DWORD								TaxiParkInfo;
		FLOAT								Radius;
		FLOAT								Heading;
		FLOAT								TeeOffset1;
		FLOAT								TeeOffset2;
		FLOAT								TeeOffset3;
		FLOAT								TeeOffset4;
		DWORD								Lon;
		DWORD								Lat;
		std::vector<TaxiwayParksCodes>*		PTaxiwayParksCodes;
};

class TaxiwayParksFS2004 {
	public:
		DWORD								TaxiParkInfo;
		FLOAT								Radius;
		FLOAT								Heading;
		DWORD								Lon;
		DWORD								Lat;
		std::vector<TaxiwayParksCodes>*		PTaxiwayParksCodes;
};


class TaxiwayParksHeader : public BGLData {
	public:
		WORD	CountTaxiwayParks;
};

class TaxiwayPaths {
	public:
		WORD	IndexStartPoint;
		WORD	IndexEndPoint;
		BYTE	Type;
		BYTE	TaxiNameIndex;
		BYTE	Dashed;
		BYTE	Surface;
		FLOAT	Width;
		FLOAT	WeightLimit;
		DWORD	Unk1;
};

class TaxiwayPathsHeader : public BGLData {
	public:
		WORD	CountTaxiwayParks;
};

class FIXX {
	public:
		std::string		Name;
		double			Lat;
		double			Lon;
};

class SRNW {
	public:
		std::string		Name;
};

class WayPointA {
	public:
		std::string		TypeName;
		DWORD			TYPE;
		double			TRK;
		std::string		FIXName;
		double			ALT;
		double			ALT2;
		double			SPEED;
		double			RADIAL;
		double			LegTime;
		double			LegDist;
};

class TRANSITION {
	public:
		std::string					Name;
		std::vector<WayPointA>*		WayPoints;
};

class Points {
	public:
		std::string					Name;
		std::vector<std::string>*	Runways;
		std::vector<WayPointA>*		WayPoints;
		std::vector<TRANSITION>*	TRANSITIONS;
};

class SIDSTAR {
	public:
		std::vector<FIXX>*			FIXES;
		std::vector<SRNW>*			RNWS;
		std::vector<Points>*		SIDS;
		std::vector<Points>*		STARS;
		std::vector<Points>*		APPROACHES;
};

class TaxiwayNames {
	public:
		char	TaxiName[8];
};

class TaxiwayNamesHeader : public BGLData {
	public:
		WORD	CountTaxiwayNames;
};

class RunwayPaths {
	public:
		WORD				Runway;
		std::string			RunwayName1;
		std::string			RunwayName2;
		std::vector<WORD>	Path;
};

class AirportList {
	public:
		DWORD			Version;
		std::string		ICAO;
		std::wstring	File;
		DWORD			FileOffset;
		double			lat;
		double			Lon;
		WORD			Layer;
};

class TPath {
	public:
		DWORD			index;
		std::string		name;
		double			Lat;
		double			Lon;
		WORD			Type;
};

class AirportPaths {
	public:
		std::vector<int>*				Starts;
		AirportInfo*					PAirportInformation		= 0;
		TaxiwayPointsHeadrer*			PHTaxiwayPoints			= 0;
		std::vector<TaxiwayPoints>*		PTaxiwayPoints			= 0;
		TaxiwayParksHeader*				PHTaxiwayParks			= 0;
		std::vector<TaxiwayParks>*		PTaxiwayParks			= 0;
		TaxiwayPathsHeader*				PHTaxiwayPaths			= 0;
		std::vector<TaxiwayPaths>*		PTaxiwayPaths			= 0;
		TaxiwayNamesHeader*				PHTaxiwayNames			= 0;
		std::vector<TaxiwayNames>*		PTaxiwayNames			= 0;
		std::vector<RunwayPaths>*		RPth;
};

#pragma pack(pop)

class IAirport
{

public:
	std::vector<int>*				Starts;
	AirportInfo*					PAirportInformation	= 0;
	TaxiwayPointsHeadrer*			PHTaxiwayPoints		= 0;
	std::vector<TaxiwayPoints>*		PTaxiwayPoints		= 0;
	TaxiwayParksHeader*				PHTaxiwayParks		= 0;
	std::vector<TaxiwayParks>*		PTaxiwayParks		= 0;
	TaxiwayPathsHeader*				PHTaxiwayPaths		= 0;
	std::vector<TaxiwayPaths>*		PTaxiwayPaths		= 0;
	TaxiwayNamesHeader*				PHTaxiwayNames		= 0;
	std::vector<TaxiwayNames>*		PTaxiwayNames		= 0;
	std::vector<RunwayPaths>*		RPth;
	SIDSTAR*						sidstar				= 0;
	
};

class IAirportData
{
	public: 
		virtual UINT						FillDB()																	= 0;
		virtual int							GetProgress()																= 0;
		virtual void						Release()																	= 0;
		virtual AirportList					GetNearAirport(double Lat, double Lon)										= 0;
    virtual AirportList         GetAirportByIcao(std::string icao) = 0;
		virtual std::vector<RunwayPaths>*	GetRunways()																= 0;
		virtual std::vector<int>*			GetRunwayStart(double lat, double lon, double direction)					= 0;
		virtual std::vector<TPath>			GetPath(DWORD start, DWORD heading, double direction)						= 0;
		virtual DATA_RUNWAY					GetStartCoord(int index)													= 0;
		virtual DWORD						GetNearTaxiwayPoint(double Lat, double Lon)									= 0;
		virtual DWORD						GetNearTaxiwayPath(double Lat, double Lon, double heading)					= 0;
		virtual std::vector<TPath>			GetPathGate(DWORD path, DWORD heading, double radius, TaxiwayParks * park)	= 0;
		virtual void						SetHSim(HANDLE hSim)														= 0;
		virtual void						DispatchProc(SIMCONNECT_RECV * pData, DWORD cbData, void * pContext)		= 0;
		virtual IAirport *					GetAirport()																= 0;
		std::vector<double>*				d																			= 0;
		std::vector<TPath>*					ReturnPath;
		std::wstring						RootSim;
	};


AIRPORTAPI GetAirportData();




