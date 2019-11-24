#include "stdafx.h"
#include "Airport.h"
#include "BGLXData.h"





Airport::Airport(BGLXData* BGLX, DWORD AirportDataOffset)
{
	this->BGLX = BGLX;
	this->AirportDataOffset = AirportDataOffset;
	GetAirportData(false);
	mode = false;
}

Airport::Airport(std::wstring BGLFile, DWORD AirportDataOffset)
{
	BGLX = new BGLXData(BGLFile);
	this->AirportDataOffset = AirportDataOffset;
	GetAirportData(true);
	mode = true;
}

HRESULT Airport::GetAirportData(bool full)
{
	if (full)
	{
		Records = BGLX->GetAirportRecordHierarhy(AirportDataOffset);
	}
	else
	{
		PAirportInformation = new AirportInfo;

		BGLX->BGLXFile->Read(PAirportInformation, AirportDataOffset, sizeof(*PAirportInformation));
		if ((PAirportInformation->ID != 0x03) && (PAirportInformation->ID != 0x3C))
		{
			PAirportInformation = NULL;
			return E_NOTIMPL;
		}
	}
	return NOERROR;
}

HRESULT Airport::GetTaxiwayInformation()
{
	if (Records)
	{
		for (int i = 0; i < Records->size(); i++)
		{
			if (Records->at(i).ID == 0x1A)
			{
				DWORD Offset = Records->at(i).Offset;
				if (!PHTaxiwayPoints)
				{
					PHTaxiwayPoints = new TaxiwayPointsHeadrer;
				}
				if (PTaxiwayPoints)
				{
					PTaxiwayPoints->clear();
				}
				else
				{
					PTaxiwayPoints = new std::vector<TaxiwayPoints>();
				}
				BGLX->BGLXFile->Read(PHTaxiwayPoints, Offset, sizeof(*PHTaxiwayPoints));
				Offset = Offset + sizeof(*PHTaxiwayPoints);
				for (int j = 0; j < PHTaxiwayPoints->CountTaxiwayPoints; j++)
				{
					TaxiwayPoints TaxiwayPoint;
					BGLX->BGLXFile->Read(&TaxiwayPoint, Offset, sizeof(TaxiwayPoint));
					PTaxiwayPoints->push_back(TaxiwayPoint);
					Offset = Offset + sizeof(TaxiwayPoint);

				}
			}
			if (Records->at(i).ID == 0x1B)
			{
				DWORD Offset = Records->at(i).Offset;
				if (!PHTaxiwayParks)
				{
					PHTaxiwayParks = new TaxiwayParksHeader;
				}
				if (PTaxiwayParks)
				{
					PTaxiwayParks->clear();
				}
				else
				{
					PTaxiwayParks = new std::vector<TaxiwayParks>();
				}
				BGLX->BGLXFile->Read(PHTaxiwayParks, Offset, sizeof(*PHTaxiwayParks));
				Offset = Offset + sizeof(*PHTaxiwayParks);
				for (int j = 0; j < PHTaxiwayParks->CountTaxiwayParks; j++)
				{
					TaxiwayParksFS2004 TaxiwayPark2004;
					TaxiwayParks TaxiwayPark;
					BGLX->BGLXFile->Read(&TaxiwayPark2004, Offset, sizeof(TaxiwayPark2004));

					Offset = Offset + sizeof(TaxiwayPark2004) - 8;
					WORD CountCodes = (TaxiwayPark2004.TaxiParkInfo & 0xFF000000) >> 24;
					TaxiwayPark2004.PTaxiwayParksCodes = new std::vector<TaxiwayParksCodes>();
					for (int k = 0; k < CountCodes; k++)
					{
						TaxiwayParksCodes TaxiwayParkCodes;
						BGLX->BGLXFile->Read(&TaxiwayParkCodes, Offset, sizeof(TaxiwayParkCodes));
						TaxiwayPark2004.PTaxiwayParksCodes->push_back(TaxiwayParkCodes);
						Offset = Offset + sizeof(TaxiwayParkCodes);
					}
					TaxiwayPark.Heading = TaxiwayPark2004.Heading;
					TaxiwayPark.Lat = TaxiwayPark2004.Lat;
					TaxiwayPark.Lon = TaxiwayPark2004.Lon;
					TaxiwayPark.PTaxiwayParksCodes = TaxiwayPark2004.PTaxiwayParksCodes;
					TaxiwayPark.Radius = TaxiwayPark2004.Radius;
					TaxiwayPark.TaxiParkInfo = TaxiwayPark2004.TaxiParkInfo;
					PTaxiwayParks->push_back(TaxiwayPark);
				}
			}
			if (Records->at(i).ID == 0x1C)
			{
				DWORD Offset = Records->at(i).Offset;
				if (!PHTaxiwayPaths)
				{
					PHTaxiwayPaths = new TaxiwayPathsHeader;
				}
				if (PTaxiwayPaths)
				{
					PTaxiwayPaths->clear();
				}
				else
				{
					PTaxiwayPaths = new std::vector<TaxiwayPaths>();
				}				
				BGLX->BGLXFile->Read(PHTaxiwayPaths, Offset, sizeof(*PHTaxiwayPaths));
				Offset = Offset + sizeof(*PHTaxiwayPaths);
				for (int j = 0; j < PHTaxiwayPaths->CountTaxiwayParks; j++)
				{
					TaxiwayPaths TaxiwayPath;
					BGLX->BGLXFile->Read(&TaxiwayPath, Offset, sizeof(TaxiwayPath));
					PTaxiwayPaths->push_back(TaxiwayPath);
					Offset = Offset + sizeof(TaxiwayPath);

				}
			}
			if (Records->at(i).ID == 0x1D)
			{
				DWORD Offset = Records->at(i).Offset;
				if (!PHTaxiwayNames)
				{
					PHTaxiwayNames = new TaxiwayNamesHeader;
				}
				if (PTaxiwayNames)
				{
					PTaxiwayNames->clear();
				}
				else
				{
					PTaxiwayNames = new std::vector<TaxiwayNames>();
				}
				BGLX->BGLXFile->Read(PHTaxiwayNames, Offset, sizeof(*PHTaxiwayNames));
				Offset = Offset + sizeof(*PHTaxiwayNames);
				for (int j = 0; j < PHTaxiwayNames->CountTaxiwayNames; j++)
				{
					TaxiwayNames TaxiwayName;
					BGLX->BGLXFile->Read(&TaxiwayName, Offset, sizeof(TaxiwayName));
					PTaxiwayNames->push_back(TaxiwayName);
					Offset = Offset + sizeof(TaxiwayName);

				}
			}
			if (Records->at(i).ID == 0x3D)
			{
				DWORD Offset = Records->at(i).Offset;
				if (!PHTaxiwayParks)
				{
					PHTaxiwayParks = new TaxiwayParksHeader;
				}
				if (PTaxiwayParks)
				{
					PTaxiwayParks->clear();
				}
				else
				{
					PTaxiwayParks = new std::vector<TaxiwayParks>();
				}
				BGLX->BGLXFile->Read(PHTaxiwayParks, Offset, sizeof(*PHTaxiwayParks));
				Offset = Offset + sizeof(*PHTaxiwayParks);
				for (int j = 0; j < PHTaxiwayParks->CountTaxiwayParks; j++)
				{
					TaxiwayParks TaxiwayPark;
					BGLX->BGLXFile->Read(&TaxiwayPark, Offset, sizeof(TaxiwayPark));
					
					Offset = Offset + sizeof(TaxiwayPark) - 8;
					WORD CountCodes = (TaxiwayPark.TaxiParkInfo & 0xFF000000) >> 24;
					TaxiwayPark.PTaxiwayParksCodes = new std::vector<TaxiwayParksCodes>();
					for (int k = 0; k < CountCodes; k++)
					{
						TaxiwayParksCodes TaxiwayParkCodes;
						BGLX->BGLXFile->Read(&TaxiwayParkCodes, Offset, sizeof(TaxiwayParkCodes));
						TaxiwayPark.PTaxiwayParksCodes->push_back(TaxiwayParkCodes);
						Offset = Offset + sizeof(TaxiwayParkCodes);
					}
					PTaxiwayParks->push_back(TaxiwayPark);
				}
			}
			
		}
		return NOERROR;
	}
}


Airport::~Airport()
{
	if (BGLX && mode)
	{
		delete BGLX;
		BGLX = NULL;
	}
	if (PAirportInformation)
	{
		delete PAirportInformation;
		PAirportInformation = NULL;
	}
	if (Records)
	{
		delete Records;
		Records = NULL;
	}
}



