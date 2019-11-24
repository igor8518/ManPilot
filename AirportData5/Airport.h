#pragma once
#include "BGLXData.h"
#include "AirportData.h"

class Airport : public IAirport
{
public:
	Airport(BGLXData* BGLX, DWORD AirportDataOffset);
	Airport(std::wstring BGLFile, DWORD AirportDataOffset);
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

	//ReadStream* BGLXFile = NULL;
	BGLXData* BGLX = NULL;
	DWORD AirportDataOffset;
	HRESULT GetAirportData(bool full);
	HRESULT GetTaxiwayInformation();


};
