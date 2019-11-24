#pragma once
#include "BGLXData.h"
#include "AirportData.h"
#include "sidstar.h"





class Airport : public IAirport
{
public:
	
	
	Airport(BGLXData * BGLX, DWORD AirportDataOffset, IAirportData * AirportData);
	Airport(std::wstring BGLFile, DWORD AirportDataOffset, IAirportData * AirportData);
	~Airport();

	/*std::vector<int>* Starts;
	AirportInfo* PAirportInformation = NULL;
	TaxiwayPointsHeadrer* PHTaxiwayPoints = NULL;
	std::vector<TaxiwayPoints>* PTaxiwayPoints = NULL;
	TaxiwayParksHeader* PHTaxiwayParks = NULL;
	std::vector<TaxiwayParks>* PTaxiwayParks = NULL;
	TaxiwayPathsHeader* PHTaxiwayPaths = NULL;
	std::vector<TaxiwayPaths>* PTaxiwayPaths = NULL;
	TaxiwayNamesHeader* PHTaxiwayNames = NULL;
	std::vector<TaxiwayNames>* PTaxiwayNames = NULL;
	std::vector<RunwayPaths>* RPth;*/


	std::vector<BGLXData::Record>* Records = NULL;
	bool mode = false;
	IAirportData* AirportData = NULL;
	std::string GetTypeNameFromPoint(sroute point);
	//ReadStream* BGLXFile = NULL;
	BGLXData* BGLX = NULL;
	DWORD AirportDataOffset;
	HRESULT GetAirportData(bool full);
	HRESULT GetFixes(ReadStreamText * SIDSTARFile);
	HRESULT GetSids(ReadStreamText * SIDSTARFile);
	HRESULT GetSIDSTAR();
	HRESULT GetTaxiwayInformation();


};
