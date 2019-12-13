#include "PMDG_NGX.h"
#include "WayLoader.h"
#include "AirportData.h"
#include <algorithm>
#include <msclr/marshal_cppstd.h>

namespace PMDG_TEST {
	using namespace System;
	using namespace System::Text::RegularExpressions;
	using namespace msclr::interop;
	//using namespace System.IO;
	std::string AddSID(IAirportData* PAirportData, aircraftData* PaircraftData, TPath* RWEnd, double RWHead)
	{
		std::string SIDName = "";
		std::vector<WayPointA>* wayPoint = new std::vector<WayPointA>();
		int j, r, t;
		bool FindSid = false;
		for (j = 0; j < PAirportData->GetAirport()->sidstar->SIDS->size(); j++)
		{
			for (r = 0; r < PAirportData->GetAirport()->sidstar->SIDS->at(j).Runways->size(); r++)
			{
				//if (PAirportData->GetAirport()->sidstar->SIDS->at(j).Runways->at(r) == Path->at(Path->size() - 1).name)
				if (PAirportData->GetAirport()->sidstar->SIDS->at(j).Runways->at(r) == RWEnd->name)
				{
					if (PAirportData->GetAirport()->sidstar->SIDS->at(j).WayPoints->at(PAirportData->GetAirport()->sidstar->SIDS->at(j).WayPoints->size() - 1).FIXName == WP->at(1).ICAO)
					{
						t = -1;
						FindSid = true;
						break;
					}
					else
					{
						if (PAirportData->GetAirport()->sidstar->SIDS->at(j).TRANSITIONS != NULL)
						{
							for (t = 0; t < PAirportData->GetAirport()->sidstar->SIDS->at(j).TRANSITIONS->size(); t++)
							{
								if (PAirportData->GetAirport()->sidstar->SIDS->at(j).TRANSITIONS->at(t).WayPoints->at(PAirportData->GetAirport()->sidstar->SIDS->at(j).TRANSITIONS->at(t).WayPoints->size() - 1).FIXName == WP->at(1).ICAO)
								{
									FindSid = true;
									break;
								}
							}
						}
					}
				}
			}
			if (FindSid)
			{
				break;
			}
		}
		if (FindSid)
		{


			for (int i = 0; i < PAirportData->GetAirport()->sidstar->SIDS->at(j).WayPoints->size(); i++)
			{
				wayPoint->push_back(PAirportData->GetAirport()->sidstar->SIDS->at(j).WayPoints->at(i));
				wayPoint->at(wayPoint->size() - 1).TYPE = FIX;
			}
			if (t >= 0)
			{
				for (int i = 0; i < PAirportData->GetAirport()->sidstar->SIDS->at(j).TRANSITIONS->at(t).WayPoints->size(); i++)
				{
					wayPoint->push_back(PAirportData->GetAirport()->sidstar->SIDS->at(j).TRANSITIONS->at(t).WayPoints->at(i));
					wayPoint->at(wayPoint->size() - 1).TYPE = WAYPOINT;
				}
			}
			SIDName = PAirportData->GetAirport()->sidstar->SIDS->at(j).Name;
		}
		else
		{
			//AddWayPoint(RWEnd->Lon, RWEnd->Lat, 1750, HEADINGUNTILALT, "(1750)", HeadTakeOff.EH - PaircraftData->MagVar, 0, 0.0);
			AddWayPoint(RWEnd->Lon, RWEnd->Lat, 1750, HEADINGUNTILALT, "(1750)", RWHead - PaircraftData->MagVar, 0, 0.0);
			AddWayPoint(WP->at(1).lon, WP->at(1).lat, CruiseLevel, FIX, WP->at(1).ICAO);
		}
		for (int i = 0; i < wayPoint->size(); i++)
		{
			FIXX fixx = {};
			bool FindFIX = false;
			for (int fix = 0; fix < PAirportData->GetAirport()->sidstar->FIXES->size(); fix++)
			{
				if (PAirportData->GetAirport()->sidstar->FIXES->at(fix).Name == wayPoint->at(i).FIXName)
				{
					FindFIX = true;
					fixx = PAirportData->GetAirport()->sidstar->FIXES->at(fix);
					break;
				}
			}
			AddSidStarTrack(&wayPoint->at(i), RWEnd, &fixx);

			
		}
		delete(wayPoint);
		wayPoint = NULL;
		return SIDName;
	}
	std::string AddSTAR(IAirportData* PAirportData, aircraftData* PaircraftData, DATA_RUNWAY* runway)
	{
		std::string STARName = "";
		std::vector<WayPointA>* wayPoint = new std::vector<WayPointA>();
		int i, j, j1, r, r2, t, t2;
		bool FindStar = false;
		bool FindAppr = false;
		for (j = 0; j < PAirportData->GetAirport()->sidstar->APPROACHES->size(); j++)
		{
			for (r = 0; r < PAirportData->GetAirport()->sidstar->APPROACHES->at(j).Runways->size(); r++)
			{
				if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j).Runways->at(r) == runway->Name)
				{
					if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j).WayPoints->at(0).FIXName == WP->at(WP->size() - 2).ICAO)
					{
						t = -1;
						FindAppr = true;
						break;
					}
					else
					{
						if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j).TRANSITIONS != NULL)
						{
							for (t = 0; t < PAirportData->GetAirport()->sidstar->APPROACHES->at(j).TRANSITIONS->size(); t++)
							{
								if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j).TRANSITIONS->at(t).WayPoints->at(0).FIXName == WP->at(WP->size() - 2).ICAO)
								{
									FindAppr = true;
									break;
								}
							}
						}
					}
				}
			}
			if (FindAppr)
			{
				break;
			}
		}

		for (i = 0; i < PAirportData->GetAirport()->sidstar->STARS->size(); i++)
		{
			for (r2 = 0; r2 < PAirportData->GetAirport()->sidstar->STARS->at(i).Runways->size(); r2++)
			{
				if (PAirportData->GetAirport()->sidstar->STARS->at(i).Runways->at(r2) == runway->Name)
				{
					if (PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->at(0).FIXName == WP->at(WP->size() - 2).ICAO)
					{
						t2 = -1;
						for (j = 0; j < PAirportData->GetAirport()->sidstar->APPROACHES->size(); j++)
						{
							for (r = 0; r < PAirportData->GetAirport()->sidstar->APPROACHES->at(j).Runways->size(); r++)
							{
								if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j).Runways->at(r) == runway->Name)
								{
									std::string ss1 = PAirportData->GetAirport()->sidstar->APPROACHES->at(j).WayPoints->at(0).FIXName;
									std::string ss2 = PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->at(PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->size() - 1).FIXName;
									if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j).WayPoints->at(0).FIXName == 
										PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->at(PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->size() - 1).FIXName)
									{
										t = -1;
										FindAppr = true;
										FindStar = true;
										break;
									}
									else
									{
										if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j).TRANSITIONS != NULL)
										{
											for (t = 0; t < PAirportData->GetAirport()->sidstar->APPROACHES->at(j).TRANSITIONS->size(); t++)
											{
												if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j).TRANSITIONS->at(t).WayPoints->at(0).FIXName ==
													PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->at(PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->size() - 1).FIXName)
												{
													FindAppr = true;
													FindStar = true;
													break;
												}
											}
										}
									}
								}
							}
							if (FindAppr)
							{
								break;
							}
						}
						break;
					}
					else
					{
						if (PAirportData->GetAirport()->sidstar->STARS->at(i).TRANSITIONS != NULL)
						{
							for (t2 = 0; t2 < PAirportData->GetAirport()->sidstar->STARS->at(i).TRANSITIONS->size(); t2++)
							{
								if (PAirportData->GetAirport()->sidstar->STARS->at(i).TRANSITIONS->at(t2).WayPoints->at(0).FIXName == WP->at(WP->size() - 2).ICAO)
								{
									for (j1 = 0; j1 < PAirportData->GetAirport()->sidstar->APPROACHES->size(); j1++)
									{
										for (r = 0; r < PAirportData->GetAirport()->sidstar->APPROACHES->at(j1).Runways->size(); r++)
										{
											if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j1).Runways->at(r) == runway->Name)
											{
												if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j1).WayPoints->at(0).FIXName ==
													PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->at(PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->size() - 1).FIXName)
												{
													t = -1;
													FindAppr = true;
													FindStar = true;
													break;
												}
												else
												{
													if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j1).TRANSITIONS != NULL)
													{
														for (t = 0; t < PAirportData->GetAirport()->sidstar->APPROACHES->at(j1).TRANSITIONS->size(); t++)

														{
															std::string ss1 = PAirportData->GetAirport()->sidstar->APPROACHES->at(j1).TRANSITIONS->at(t).WayPoints->at(0).FIXName;
															std::string ss2 = PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->at(PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->size() - 1).FIXName;
															if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j1).TRANSITIONS != NULL)
															{
																if (PAirportData->GetAirport()->sidstar->APPROACHES->at(j1).TRANSITIONS->at(t).WayPoints->at(0).FIXName ==
																	PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->at(PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->size() - 1).FIXName)
																{
																	FindAppr = true;
																	FindStar = true;
																	break;
																}
															}
														}
													}
												}
											}
										}
										if (FindAppr)
										{
											break;
										}
									}
									break;
								}
							}
						}
					}
				}
			}
			if (FindStar)
			{
				break;
			}

		}
		if (FindStar)
		{

			if (t2 >= 0)
			{
				for (int ii = 0; ii < PAirportData->GetAirport()->sidstar->STARS->at(i).TRANSITIONS->at(t2).WayPoints->size(); ii++)
				{
					wayPoint->push_back(PAirportData->GetAirport()->sidstar->STARS->at(i).TRANSITIONS->at(t2).WayPoints->at(ii));
					wayPoint->at(wayPoint->size() - 1).TYPE = WAYPOINT;
				}
			}
			for (int ii = 0; ii < PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->size()-1; ii++)
			{
				wayPoint->push_back(PAirportData->GetAirport()->sidstar->STARS->at(i).WayPoints->at(ii));
				wayPoint->at(wayPoint->size() - 1).TYPE = FIX;
			}
			STARName = PAirportData->GetAirport()->sidstar->STARS->at(i).Name;

		}
		if (FindAppr)
		{

			if (t >= 0)
			{
				for (int ii = 0; ii < PAirportData->GetAirport()->sidstar->APPROACHES->at(j).TRANSITIONS->at(t).WayPoints->size(); ii++)
				{
					wayPoint->push_back(PAirportData->GetAirport()->sidstar->APPROACHES->at(j).TRANSITIONS->at(t).WayPoints->at(ii));
					wayPoint->at(wayPoint->size() - 1).TYPE = WAYPOINT;
				}
			}
			AppPoint = wayPoint->size();
			for (int ii = 0; ii < PAirportData->GetAirport()->sidstar->APPROACHES->at(j).WayPoints->size(); ii++)
			{
				wayPoint->push_back(PAirportData->GetAirport()->sidstar->APPROACHES->at(j).WayPoints->at(ii));
				wayPoint->at(wayPoint->size() - 1).TYPE = FIX;
			}
			STARName = STARName + " - " + PAirportData->GetAirport()->sidstar->APPROACHES->at(j).Name;

		}

		else
		{
			SIMCONNECT_DATA_LATLONALT latlon = SIMMATH::GetDALatLon(runway->sLatitude, runway->sLongitude, runway->eHeading, 18.0);
			AddWayPoint(latlon.Longitude, latlon.Latitude, WP->at(WP->size() - 1).alt + 2000.0, FIX, "GS 18.0 " + runway->Name, 0, FIXALT);
			AddWayPoint(runway->sLongitude, runway->sLatitude, WP->at(WP->size() - 1).alt + 2000.0, WAYPOINT, "FINAL " + runway->Name, 0, FIXALT);
			AddWayPoint(runway->sLongitude, runway->sLatitude, WP->at(WP->size() - 1).alt, GSWAY, runway->Name, 0, FIXALT);
			AddWayPoint(runway->eLongitude, runway->eLatitude, WP->at(WP->size() - 1).alt, RUNWAY, runway->Name, 0, FIXALT);
			STARName = "GS 18.0 ";
		}

		for (int i = 0; i < wayPoint->size(); i++)
		{
			FIXX fixx = {};
			bool FindFIX = false;
			for (int fix = 0; fix < PAirportData->GetAirport()->sidstar->FIXES->size(); fix++)
			{
				if (PAirportData->GetAirport()->sidstar->FIXES->at(fix).Name == wayPoint->at(i).FIXName)
				{
					FindFIX = true;
					fixx = PAirportData->GetAirport()->sidstar->FIXES->at(fix);
					
					break;
				}
			}
			/*if (fixx.Lat == 0.0)
			{
				for (int fix = 0; fix < PAirportData->GetAirport()->sidstar->FIXES->size(); fix++)
				{
					if (PAirportData->GetAirport()->sidstar->FIXES->at(fix).Name == wayPoint->at(i-1).FIXName)
					{
						FindFIX = true;
						fixx.Lat = PAirportData->GetAirport()->sidstar->FIXES->at(fix).Lat;
						break;
					}
				}
			}
			if (fixx.Lon == 0.0)
			{
				for (int fix = 0; fix < PAirportData->GetAirport()->sidstar->FIXES->size(); fix++)
				{
					if (PAirportData->GetAirport()->sidstar->FIXES->at(fix).Name == wayPoint->at(i - 1).FIXName)
					{
						FindFIX = true;
						fixx.Lon = PAirportData->GetAirport()->sidstar->FIXES->at(fix).Lon;
						break;
					}
				}
			}*/
			if (i == AppPoint)
			{
				AppPoint = CountWays;
			}
			AddSidStarTrack(&wayPoint->at(i), NULL, &fixx, runway);

			
		}
		delete(wayPoint);
		wayPoint = NULL;
		return STARName;
	}
	void AddSidStarTrack(WayPointA* wayPoint, TPath* RWEnd, FIXX* fixx, DATA_RUNWAY* runway)
	{
		if (wayPoint->TypeName == "TRKUNTIL")
		{
			if (RWEnd == NULL)
			{
				AddWayPoint(runway->eLongitude, runway->eLatitude, wayPoint->ALT, HEADINGUNTILALT, std::string("(" + std::to_string((DWORD)wayPoint->ALT) + ")"), wayPoint->TRK, 0, 0.0);
			}
			else
			{
				AddWayPoint(RWEnd->Lon, RWEnd->Lat, wayPoint->ALT, HEADINGUNTILALT, std::string("(" + std::to_string((DWORD)wayPoint->ALT) + ")"), wayPoint->TRK, 0, 0.0);
			}
		}
		else if (wayPoint->TypeName == "TRKINTERCEPTRADIALTOFIX")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, 0.0, TRKINTERCEPTRADIAL, "(INTRAD) " + wayPoint->FIXName, wayPoint->TRK, 0, 0.0, wayPoint->RADIAL);
		}
		else if (wayPoint->TypeName == "FIXATORABOVE")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, ALTORABOVE, 0.0);
		}
		else if (wayPoint->TypeName == "FIXOVERFLYATORABOVE")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, ALTORABOVE, 0.0);
		}
		else if (wayPoint->TypeName == "FIXATORBELOW")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, ALTORBELOW, 0.0);
		}
		else if (wayPoint->TypeName == "FIXATORBELOWATORABOVE")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, ALTORBELOW, 0.0);
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT2, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, ALTORABOVE, 0.0);
		}
		else if (wayPoint->TypeName == "FIXATORBELOWATORABOVE")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, ALTORBELOW, 0.0);
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT2, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, ALTORABOVE, 0.0);
		}
		else if (wayPoint->TypeName == "TURNRIGHTDIRECTFIXATORBELOWATORABOVE")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, TURNRIGHTDIRECTFIX, wayPoint->FIXName, wayPoint->TRK, ALTORABOVE, 0.0);
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT2, TURNRIGHTDIRECTFIX, wayPoint->FIXName, wayPoint->TRK, ALTORBELOW, 0.0);
		}
		else if (wayPoint->TypeName == "FIX")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, 0.0, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, 0, 0.0);
		}
		else if (wayPoint->TypeName == "FIXSPEED")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, 0.0, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, 0, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "FIXOVERFLY")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, 0.0, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, 0, 0.0);
		}
		else if (wayPoint->TypeName == "FIXFALT")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, FIXALT, 0.0);
		}
		else if (wayPoint->TypeName == "FIXFALTSPEED")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, FIXALT, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "FIXOVERFLYFALT")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, FIXALT, 0.0);
		}
		else if (wayPoint->TypeName == "RNW")
		{
			AddWayPoint(runway->sLongitude, runway->sLatitude, WP->at(WP->size() - 1).alt, GSWAY, runway->Name, 0, FIXALT);
			AddWayPoint(runway->eLongitude, runway->eLatitude, WP->at(WP->size() - 1).alt, RUNWAY, runway->Name, 0, FIXALT);
		}
		else if (wayPoint->TypeName == "FIXOVERFLYATORABOVESPEED")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, ALTORABOVE, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "HOLDATFIXRIGHTTURNINBOUNDCOURSEALTSPEEDLEGTIME")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, FIXALT, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "HOLDATFIXRIGHTTURNINBOUNDCOURSEALTSPEED")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, wayPoint->TYPE, wayPoint->FIXName, wayPoint->TRK, FIXALT, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "TURNLEFTDIRECTFIX")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, 0.0, TURNLEFTDIRECTFIX, wayPoint->FIXName, 0, 0, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "TURNRIGHTDIRECTFIX")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, 0.0, TURNRIGHTDIRECTFIX, wayPoint->FIXName, 0, 0, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "TURNRIGHTDIRECTFIXOVERFLYFALT")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, TURNRIGHTDIRECTFIX, wayPoint->FIXName, 0, FIXALT, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "TURNLEFTDIRECTFIXOVERFLYFALT")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, TURNLEFTDIRECTFIX, wayPoint->FIXName, 0, FIXALT, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "TURNLEFTDIRECTFIXOVERFLY")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, 0.0, TURNLEFTDIRECTFIX, wayPoint->FIXName, 0, 0, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "TURNRIGHTDIRECTFIXFALT")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, TURNRIGHTDIRECTFIX, wayPoint->FIXName, 0, FIXALT, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "TURNLEFTDIRECTFIXFALT")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, TURNLEFTDIRECTFIX, wayPoint->FIXName, 0, FIXALT, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "TURNLEFTDIRECTFIXATORABOVE")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, wayPoint->ALT, TURNLEFTDIRECTFIX, wayPoint->FIXName, 0, ALTORABOVE, wayPoint->SPEED);
		}
		else if (wayPoint->TypeName == "TRKUNTILFROMFIXSPEED")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, 0.0, HEADINGUNTILFROMDIST, wayPoint->FIXName, wayPoint->TRK, 0, wayPoint->SPEED, wayPoint->LegDist);
		}
		else if (wayPoint->TypeName == "TRKUNTILFROMFIX")
		{
			AddWayPoint(fixx->Lon, fixx->Lat, 0.0, HEADINGUNTILFROMDIST, wayPoint->FIXName, wayPoint->TRK, 0, 0.0, wayPoint->LegDist);
		}
		else
		{
			std::cout << wayPoint->TypeName;
		}

	}
	void FillWays()
	{
		//Flight.cruiseLevel = CruiseLevel;
		Stream^ myStream;
		OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog;
		int countWP = 0;
		StreamReader^ sr;
		//openFileDialog1->InitialDirectory = "c:\\";
		openFileDialog1->Filter = "PMDG rte files (*.rte)|*.rte|All files (*.*)|*.*";
		openFileDialog1->FilterIndex = 1;
		openFileDialog1->RestoreDirectory = true;

		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK)
		{
			try
			{   // Open the text file using a stream reader.
				sr = gcnew StreamReader(openFileDialog1->FileName);
				WP = new std::vector<WayPoint>();
				while (!sr->EndOfStream)
				{
					String^ line = sr->ReadLine();
					try
					{
						countWP = Convert::ToInt32(line);
						WayPoint wp;
						
						for (int i = 0; i < countWP; i++)
						{
							int j = 0;
							line = sr->ReadLine();
							while (line == "")
							{
								line = sr->ReadLine();
							}
							wp.ICAO = marshal_as<std::string>(line);
							line = sr->ReadLine();
							wp.Type = Convert::ToInt32(line);
							line = sr->ReadLine();
							wp.AirWay = marshal_as<std::string>(line);
							line = sr->ReadLine();
							String^ LatLon = line;
							
							//String^ Pattern = "\d+\s*[NS]\s*\d*\.\d+\s*[EW]\s*\d+\.\d*\s*\d+";
							//String^ Pattern = "\d+\s*";
							//Regex^ re = gcnew Regex(Pattern);
							Regex^ re = gcnew Regex(
								"(\\d+)\\s*([NS])\\s*(\\d+\\.\\d*)\\s*([EW])\\s*(\\d+\\.\\d*)"
								"\\s*(-?\\d+)",
								(RegexOptions)
								(RegexOptions::CultureInvariant
									| RegexOptions::Compiled
									));
							Match^ ms = re->Match(line);
							//MatchCollection^ ms = re->Matches(line);
							
							if (ms->Groups[2]->Value == "S")
							{
								wp.lat = -Convert::ToDouble(ms->Groups[3]->Value);
							}
							else
							{
								wp.lat = Convert::ToDouble(ms->Groups[3]->Value);
							}
							if (ms->Groups[4]->Value == "W")
							{
								wp.lon = -Convert::ToDouble(ms->Groups[5]->Value);
							}
							else
							{
								wp.lon = Convert::ToDouble(ms->Groups[5]->Value);
							}
							wp.alt = Convert::ToDouble(ms->Groups[6]->Value);
							
							if (wp.Type == 1)
							{
								j = 10;
							}
							else
							{
								j = 4;
							}
							for (int k = 0; k < j; k++)
							{
								line = sr->ReadLine();
							}
							WP->push_back(wp);
						}
					}
					catch (Exception ^e)
					{
						continue;
					}


				}
			}
			catch (Exception ^e)
			{

			}
			
			sr->Close();
			
		}

		
		
		//DSHEH *Ways[500];
		//CountWays = 0;
		
		/*AddWayPoint(60.780871, 56.744834, 0.0, 0, "RD A");
		AddWayPoint(60.780881, 56.748455, 0.0, 0, "RD A");
		AddWayPoint(60.781651, 56.748680, 0.0, 0, "RD A");
		AddWayPoint(60.795913, 56.748686, 0.0, 0, "RD MAIN");
		AddWayPoint(60.796385, 56.748564, 0.0, 0, "RD MAIN");
		AddWayPoint(60.805392, 56.748574, 0.0, 0, "RD MAIN");
		AddWayPoint(60.805990, 56.748653, 0.0, 0, "RD MAIN");
		AddWayPoint(60.817327, 56.748654, 0.0, 0, "RD MAIN");
		AddWayPoint(60.817987, 56.748630, 0.0, 0, "RD E");
		AddWayPoint(60.821127, 56.747771, 0.0, 0, "RD E");
		AddWayPoint(60.821593, 56.747442, 0.0, 0, "RD E");*/

		//AddWayPoint(60.821604, 56.744757, 0.0, RUNWAY, "READY 26R");
		//AddWayPoint(60.780871, 56.744834,Flight.cruiseLevel, RUNWAY, "26R");
		//AddWayPoint(60.780871, 56.744834, 1750, HEADINGUNTILALT, std::string("(1750)"),256.0,0,180.0);
		//AddWayPoint(60.797, 56.74319443333333, Flight.cruiseLevel, TURNLEFTDIRECTFIX, std::string("EKB"),0,0,180.0);
		//AddWayPoint(61.17750557, 57.08958057, 5000, FIX, "EK24C", 0, ALTORABOVE);
		//AddWayPoint(61.400277783, 57.288888883, 7000, FIX, "DOSUM", 0, ALTORABOVE);
		//AddWayPoint(61.797, 57.637, Flight.cruiseLevel, WAYPOINT, "NAMOL");
		//AddWayPoint(63.844, 58.714, Flight.cruiseLevel, FIX, "UTORA");
		//AddWayPoint(61.929722217, 57.337, 8000, WAYPOINT, "AR", 0, ALTORABOVE);
		//AddWayPoint(62.00194443, 57.135277783, 8000, WAYPOINT, "SOPUS", 0, ALTORABOVE);
		/*AddWayPoint(61.1675, 56.94, 0, FIX, "BEGMA", 0);

		AddWayPoint(61.09096667, 56.768413883, 2740.0, FIX, "D067J", 0, FIXALT);
		AddWayPoint(61.0557, 56.744, 2410.0, FIX, "CF26R", 0, FIXALT);
		AddWayPoint(60.9521, 56.7447, 2410.0, FIX, "FN26R", 0, FIXALT);
		AddWayPoint(60.8594, 56.7447, 1262.0, FIX, "MN26R", 0, FIXALT);
		AddWayPoint(60.821604, 56.744757, 764.0, GSWAY, "26R", 0, FIXALT);
		AddWayPoint(60.780871, 56.744834, 764.0, RUNWAY, "26R", 0, FIXALT);*/

		

		/*AddWayPoint(60.780871, 56.744834, 0.0, 0, "RD A");
		AddWayPoint(60.780881, 56.748455, 0.0, 0, "RD A");
		AddWayPoint(60.781651, 56.748680, 0.0, 0, "RD A");
		AddWayPoint(60.795913, 56.748686, 0.0, 0, "RD MAIN");
		AddWayPoint(60.796385, 56.748564, 0.0, 0, "RD MAIN");
		AddWayPoint(60.805392, 56.748574, 0.0, 0, "RD MAIN");*/
	}
	void AddWayPoint(double lon, double lat, double alt, int pointType, std::string PointName,double heading, int fixAlt, double speed, double radial)
	{
		static int point = 0;
		if ((point == 0) && (CountWays == 0))
		{
			Ways[CountWays] = new SIMMATH::DSHEH;
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
			Ways[CountWays]->fixAlt = fixAlt;
			Ways[CountWays]->Speed = speed;
			SIMMATH::DOrtoKM(Ways[CountWays]);
			if ((pointType == FIXHEADING) || (pointType == HEADINGUNTILALT))
			{
				Ways[CountWays]->H = heading;
			}
			if (pointType == HEADINGUNTILFROMDIST)
			{
				Ways[CountWays]->Dist = radial;
				Ways[CountWays]->H = heading;
			}
			if (pointType == TRKINTERCEPTRADIAL)
			{
				Ways[CountWays]->Dist = radial;
				Ways[CountWays]->H = heading;
			}
			CountWays++;
			point = 0;
		}
		else if (CountWays != 0)
		{
			Ways[CountWays] = new SIMMATH::DSHEH;
			Ways[CountWays]->Slla.Longitude = Ways[CountWays-1]->Ella.Longitude;
			Ways[CountWays]->Slla.Latitude = Ways[CountWays-1]->Ella.Latitude;
			Ways[CountWays]->Slla.Altitude = Ways[CountWays-1]->Ella.Altitude;
			Ways[CountWays]->Ella.Longitude = lon;
			Ways[CountWays]->Ella.Latitude = lat;
			Ways[CountWays]->Ella.Altitude = alt;
			Ways[CountWays]->typePath = pointType;
			Ways[CountWays]->nameEndPoint = PointName;
			Ways[CountWays]->fixAlt = fixAlt;
			Ways[CountWays]->Speed = speed;
			SIMMATH::DOrtoKM(Ways[CountWays]);
			if ((pointType == FIXHEADING) || (pointType == HEADINGUNTILALT))
			{
				Ways[CountWays]->H = heading;
			}
			if (pointType == HEADINGUNTILFROMDIST)
			{
				Ways[CountWays]->Dist = radial;
				Ways[CountWays]->H = heading;
			}
			if (pointType == TRKINTERCEPTRADIAL)
			{
				Ways[CountWays]->Dist = radial;
				Ways[CountWays]->H = heading;
			}
			CountWays++;
		}
	}
}

