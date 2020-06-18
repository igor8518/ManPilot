//------------------------------------------------------------------------------
//
//  PMDG 737NGX external connection sample 
// 
//------------------------------------------------------------------------------
#pragma once
#include "PMDG_NGX.h"
#include "IWebSimTexture.h"
#include "IWebSimClient.h"


namespace PMDG_TEST{
	using namespace System::Threading;

bool NGX_FuelPumpLAftLight = true;
bool NGX_TaxiLightSwitch = false;
bool NGX_LogoLightSwitch = false;
bool STAR = false;
double speed = 10;
UINT Timer = 0;
bool InTimer = false;
IAirportData * PAirportData;
DWORD attach = 0;
bool aFlag = true;



// This function is called when NGX data changes
void ProcessNGXData(PMDG_NG3_Data *pS) {
	if (pS->FUEL_annunLOWPRESS_Aft[0] != NGX_FuelPumpLAftLight) {
		NGX_FuelPumpLAftLight = pS->FUEL_annunLOWPRESS_Aft[0];
    if (NGX_FuelPumpLAftLight) {
      AddText("\nLOW PRESS LIGHT: [ON]\n");
    }
    else {
      AddText("\nLOW PRESS LIGHT: [OFF]\n");
    }
	}
	if (pS->LTS_TaxiSw != NGX_TaxiLightSwitch) {
		NGX_TaxiLightSwitch = pS->LTS_TaxiSw;
    if (NGX_TaxiLightSwitch) {
      AddText("\nTAXI LIGHTS: [ON]\n");
    }
    else {
      AddText("\nTAXI LIGHTS: [OFF]\n");
    }
	}
	if (pS->LTS_LogoSw != NGX_LogoLightSwitch) {
		NGX_LogoLightSwitch = pS->LTS_LogoSw;
    if (NGX_LogoLightSwitch) {
      AddText("\nLOGO LIGHTS: [ON]\n");
    }
    else {
      AddText("\nLOGO LIGHTS: [OFF]\n");
    }
	}
}




void toggleTaxiLightSwitch() {
	if (AircraftRunning) {
		bool New_TaxiLightSwitch = !NGX_TaxiLightSwitch;
		if (Control.Event == 0) {
			Control.Event = EVT_OH_LIGHTS_TAXI;		// = 69749
      if (New_TaxiLightSwitch) {
        Control.Parameter = 1;
      }
      else {
        Control.Parameter = 0;
        SimConnect_SetClientData(hSimConnect, PMDG_NG3_CONTROL_ID, PMDG_NG3_CONTROL_DEFINITION,
          0, 0, sizeof(PMDG_NG3_Control), &Control);
      }
		}
	}
}




void toggleLogoLightsSwitch() {
	bool New_LogoLightSwitch = !NGX_LogoLightSwitch;
	int parameter = 5000;
}




void toggleFlightDirector() {
	SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_FLIGHT_DIRECTOR_SWITCH, MOUSE_FLAG_LEFTSINGLE,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_FLIGHT_DIRECTOR_SWITCH, MOUSE_FLAG_LEFTRELEASE,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}




void slewHeadingSelector() {
	SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_HEADING_SELECTOR, MOUSE_FLAG_WHEEL_UP,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}




void throttleSet(DWORD position) {
	SetPositionControl((INT16)(position));
}




void throttleLever(int position) {
	throttle.L = (double)position;
	throttle.R = (double)position;
	SimConnect_SetDataOnSimObject(hSimConnect, DEF_THROTTLE, 0, 0, 0, sizeof(tthrottle), &throttle);
}




void setThrust(FLOAT thrust) {
	static clock_t startTime;
	static double throttle = 0;
	static double startThrust = PMDG_TEST::thrust.N1L;
	clock_t endTime = clock()+1;
	double endThrust = PMDG_TEST::thrust.N1L;
	double timeOff = endTime - startTime;
	double thrustOff = endThrust - startThrust;
	double thrustA = (timeOff/1000)*thrustOff;
	double dT = thrust - endThrust;
	double dA = (dT / 5) - thrustA;
	double dThrottle = dA;
	if (dThrottle < -5) {
		dThrottle = -5;
	}
	else if (dThrottle > 5) {
		dThrottle = 5;
	}
	throttle = throttle+dThrottle;
	bool thrustSet = FALSE; 
	if (throttle < 0) {
		throttle = 0;
	}
	else if (throttle > 100) {
		throttle = 100;
	}
	throttleLever(throttle);
	thrustSet = TRUE;
	startTime = clock();
	startThrust = PMDG_TEST::thrust.N1L;
}




void setGSpeed(FLOAT speed, FLOAT Tr=0) {
	static clock_t startTime;
	static double thrust = PMDG_TEST::thrust.N1L;
	static double throttle = 0;
	static double startSpeed = PMDG_TEST::speeds.GS;
	clock_t endTime = clock() + 1;
	double endSpeed = PMDG_TEST::speeds.GS;
	double timeOff = endTime - startTime;
	double speedOff = endSpeed - startSpeed;
	double speedA = (timeOff/1000)*speedOff;
	double dS = speed - endSpeed;
	double dA = (dS/5) - speedA;
	double dThrust = dA;
	if (dThrust < -5) {
		dThrust = -5;
	}
	else if (dThrust > 5) {
		dThrust = 5;
	}
	if ((speedA < -0.3) && (speedA > -0.4)) {
		dThrust = dThrust + 2;
	}
	else if ((speedA > 0.3) && (speedA < 0.4)) {
		dThrust = dThrust - 2;
	}
	if (speedA < -0.4) {
		dThrust = dThrust+5;
	}
	else if (speedA >= 0.4) {
		dThrust = dThrust - 5;
	}
	thrust = PMDG_TEST::thrust.N1L + dThrust;
	bool thrustSet = FALSE;
	if (thrust < 21) {
		thrust = 21;
	}
	else if (thrust > 100) {
		thrust = 100;
	}
	setThrust(thrust);
	thrustSet = TRUE;
	startTime = clock();
	startSpeed = PMDG_TEST::speeds.GS;
}




HRESULT SetLight(int lightInt) {
	if ((CurrCabLight != CabLight) && (IDLights->size() == 10)) {
		if (IDLights->size() == 10) {
			for (int i = 0; i < IDLights->size(); i++) {
				if (IDLights->at(i) > 0) {
					hr = SimConnect_ReleaseSimObjectFromSimObject(hSimConnect, 0, IDLights->at(i));
					hr = SimConnect_AIRemoveObject(hSimConnect, IDLights->at(i), 500);
					IDLights->at(i) = 0;
				}
			}
			IDLights->clear();
			CabinLightState = false;
		}
		if((CurrCabLight != CabLight) || !CabinLightState) {
			std::string Li = "";
			switch (lightInt) {
			  case 1: {
				  Li = "CabL0";
				  break;
			  }
			  case 2: {
				  Li = "CabL25";
				  break;
			  }
			  case 3: {
				  Li = "CabL50";
				  break;
			  }
			  case 4: {
				  Li = "CabL75";
				  break;
			  }
        default: {
          break;
        }
			}
			if (lightInt > 0) {
				hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, Lightxyz[0], zeropbh, Li.c_str(), zeroxyz, zeropbh, REQ_ATTACH1);
				hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, Lightxyz[1], zeropbh, Li.c_str(), zeroxyz, zeropbh, REQ_ATTACH2);
				hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, Lightxyz[2], zeropbh, Li.c_str(), zeroxyz, zeropbh, REQ_ATTACH3);
				hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, Lightxyz[3], zeropbh, Li.c_str(), zeroxyz, zeropbh, REQ_ATTACH4);
				hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, Lightxyz[4], zeropbh, Li.c_str(), zeroxyz, zeropbh, REQ_ATTACH5);
				hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, Lightxyz[5], zeropbh, Li.c_str(), zeroxyz, zeropbh, REQ_ATTACH6);
				hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, Lightxyz[6], zeropbh, Li.c_str(), zeroxyz, zeropbh, REQ_ATTACH7);
				hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, Lightxyz[7], zeropbh, Li.c_str(), zeroxyz, zeropbh, REQ_ATTACH8);
				hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, Lightxyz[8], zeropbh, Li.c_str(), zeroxyz, zeropbh, REQ_ATTACH9);
				hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, Lightxyz[9], zeropbh, Li.c_str(), zeroxyz, zeropbh, REQ_ATTACH10);
			}
			CurrCabLight = CabLight;
		}
	}
	return hr;
}




void CALLBACK MyDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext) {
	if (PAirportData != NULL) {
		PAirportData->DispatchProc(pData, cbData, pContext);
	}
	switch (pData->dwID) {
	  case SIMCONNECT_RECV_ID_CLIENT_DATA: { // Receive and process the NGX data block 
      SIMCONNECT_RECV_CLIENT_DATA* pObjData = (SIMCONNECT_RECV_CLIENT_DATA*)pData;
      switch (pObjData->dwRequestID) {
        case DATA_REQUEST: {
          PMDG_NG3_Data* ps = (PMDG_NG3_Data*)&pObjData->dwData;
          NGX = *(PMDG_NG3_Data*)&pObjData->dwData;
          GetNGXData = TRUE;
          SIMCONNECT_RECV_EVENT* pObjData = (SIMCONNECT_RECV_EVENT*)pData;
          switch (pObjData->uEventID) {
            case(0): {
              break;
            }
            default: {
              AddText("Data: " + pObjData->uEventID + "\n");
              break;
            }
          }
          break;
        }
        case CONTROL_REQUEST: {
          PMDG_NG3_Control* pS = (PMDG_NG3_Control*)&pObjData->dwData;
          Control = *pS;
          break;
        }
        case ADD_DATA_REQUEST: {
          sExportData* pS = (sExportData*)&pObjData->dwData;
          ExportData.HorBar = pS->HorBar;
          ExportData.HorEnabled = pS->HorEnabled;
          ExportData.VerBar = pS->VerBar;
          ExportData.VerEnabled = pS->VerEnabled;
          break;
        }
        default: {
          AddText("NON Data request and not control request");
          break;
        }
      }
      break;
    }
	  case SIMCONNECT_RECV_ID_ASSIGNED_OBJECT_ID: {
		  SIMCONNECT_RECV_ASSIGNED_OBJECT_ID *pObjData = (SIMCONNECT_RECV_ASSIGNED_OBJECT_ID*)pData;
		  switch (pObjData->dwRequestID) {
		    case REQ_ATTACH: {
			    SIMCONNECT_DATA_XYZ zeroxyz = { 0, 0, 0 };
			    SIMCONNECT_DATA_PBH zeropbh = { 0, 0, 0 };
			    attach = pObjData->dwObjectID;
			    if (aFlag) {
				    hr = SimConnect_AttachSimObjectToSimObject(hSimConnect, 0, zeroxyz, zeropbh, pObjData->dwObjectID, zeroxyz, zeropbh);
				    aFlag = false;
			    }
			    break;
		    }
		    default: {
			    if ((pObjData->dwRequestID >= 2)&&(pObjData->dwRequestID <= 11)) {
					    IDLights->push_back(pObjData->dwObjectID);
					    CabinLightState = true;
			    }
			    break;
		    }
		  }
		  break;
	  }
	  case SIMCONNECT_RECV_ID_SIMOBJECT_DATA: {
      SIMCONNECT_RECV_SIMOBJECT_DATA* pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
      switch (pObjData->dwRequestID) {
        case REQ_AIRCRAFT_USER: {
          DWORD ObjectID = pObjData->dwObjectID;
          aircraftData* ps = (aircraftData*)&pObjData->dwData;
          PMDG_TEST::AircraftData = *((aircraftData*)&pObjData->dwData);
          PMDG_TEST::AircraftData.ailer = PMDG_TEST::AircraftData.ailer * 4.16666666666666667;
          SIMCONNECT_DATA_INITPOSITION AircraftPos;
          if ((PMDG_TEST::AircraftData.lat != 0.0)) {
            AircraftPos.Latitude = PMDG_TEST::AircraftData.lat;
            AircraftPos.Longitude = PMDG_TEST::AircraftData.lon;
            AircraftPos.Altitude = PMDG_TEST::AircraftData.alt;
            AircraftPos.Airspeed = 0.0;
            SIMCONNECT_DATA_INITPOSITION Init;
            Init.Altitude = PMDG_TEST::AircraftData.palt;
            Init.Latitude = PMDG_TEST::AircraftData.lat;
            Init.Longitude = PMDG_TEST::AircraftData.lon;
            Init.Pitch = PMDG_TEST::AircraftData.pitch;
            Init.Bank = PMDG_TEST::AircraftData.bank;
            Init.Heading = PMDG_TEST::AircraftData.hed;
            Init.OnGround = 0; // PMDG_TEST::AircraftData.simOnGround;
            Init.Airspeed = 0.0;
            Attached = true;
            break;
          }
          break;
        }
        case REQ_THRUST: {
          PMDG_TEST::thrust = *(tthrust*)&pObjData->dwData;
          break;
        }
        case REQ_SPEEDS: {
          PMDG_TEST::speeds = *(tspeeds*)&pObjData->dwData;
          tspeeds* ps = (tspeeds*)&pObjData->dwData;
          if (ps->simOnGround == 1) {
            if (mode == TAXI_OUT || mode == TAXI_IN) {
              if ((ps->LBrake == 0.0) && (ps->RBrake == 0.0)) {
                setGSpeed(speed);
              }
            }
          }
          break;
        }
        case REQ_SPEED: {
          PMDG_TEST::push_speed = *(tspeed*)&pObjData->dwData;
          tspeed* ps = (tspeed*)&pObjData->dwData;
          if (ps->GS > -(speed)) {
            if (mode == PUSHBACK) {
              Sleep(1);
              push_speed.GS = ps->GS - 0.05;
              SimConnect_SetDataOnSimObject(hSimConnect, DEF_SPEED, 0, 0, 0, sizeof(tspeed), &push_speed);
              hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_SPEED, DEF_SPEED, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
              //SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
            }
          }
          break;
        }
        default: {
          break;
        }
      }
      break;
    }
	  case SIMCONNECT_RECV_ID_EVENT: {
      SIMCONNECT_RECV_EVENT* evt = (SIMCONNECT_RECV_EVENT*)pData;
      switch (evt->uEventID) {
        case EVENT_ELEVATOR_SET: {
          SetDataPitch = true;
          break;
        }
        case EVENT_SIM_START:	{ // Track aircraft changes 
          HRESULT hr = SimConnect_RequestSystemState(hSimConnect, AIR_PATH_REQUEST, "AircraftLoaded");
          break;
        }
        case EVENT_KEYBOARD_A: {
          toggleTaxiLightSwitch();
          quit = 1;
          break;
        }
        case EVENT_KEYBOARD_B: {
          toggleLogoLightsSwitch();
          break;
        }
        case EVENT_KEYBOARD_C: {
          toggleFlightDirector();
          break;
        }
        case EVENT_KEYBOARD_D: {
          slewHeadingSelector();
          break;
        }
        case EVENT_THROTTLE_SET: {
          AddText("Value: " + (INT16)evt->dwData + "\n");
          break;
        }
        default: {
          AddText("EVENT\n");
          break;
        }
      }
      break;
    }
	  case SIMCONNECT_RECV_ID_SYSTEM_STATE: {// Track aircraft changes 
      SIMCONNECT_RECV_SYSTEM_STATE* evt = (SIMCONNECT_RECV_SYSTEM_STATE*)pData;
      if (evt->dwRequestID == AIR_PATH_REQUEST) {
        if (strstr(evt->szString, "PMDG 737") != NULL) {
          AircraftRunning = true;
          AddText("Select B737\n");
        }
        else
        {
          AircraftRunning = false;
          AddText("Select: " + (gcnew System::String(evt->szString)) + "\n");
        }
      }
      else {
        AddText(String::Format("SIMCONNECT_RECV_SYSTEM_STATE: {0}\n", evt->dwRequestID));
      }
      break;
    }
	  case SIMCONNECT_RECV_ID_QUIT: {
      break;
    }
	  case SIMCONNECT_RECV_ID_WEATHER_OBSERVATION: {
		  using namespace System;
		  using namespace System::Text::RegularExpressions;
		  SIMCONNECT_RECV_WEATHER_OBSERVATION *pObjData = (SIMCONNECT_RECV_WEATHER_OBSERVATION*)pData;
		  switch (pObjData->dwRequestID) {
		    case REQ_WEATHER_LATLON: {
			    PMDG_TEST::Weather = (SIMCONNECT_RECV_WEATHER_OBSERVATION*)pObjData;
			    Metar = Weather->szMetar;
			    metar = std::string(Metar);
			    try {
				    String^ line = gcnew String(Metar);
				    Regex^ re = gcnew Regex(
					    "\\w+\\s+\\w+\\s+(\\d{3})(\\d{2,3})(KT|MPS|KPH)",
					    (RegexOptions)
					    (RegexOptions::CultureInvariant
						    | RegexOptions::Compiled
						    ));
				    Match^ ms = re->Match(line);
				    DestMetar.WindDir = Convert::ToDouble(ms->Groups[1]->Value);
				    DestMetar.WindSpeed = Convert::ToDouble(ms->Groups[2]->Value);
			    }
			    catch (Exception^ ex) {
				    DestMetar.WindDir = 0.0;
				    DestMetar.WindSpeed = -1.0;
			    }
			    break;
		    }
		    default: {
			    break;
		    }
		  }
	  }
	  case SIMCONNECT_RECV_ID_EXCEPTION: {
		  SIMCONNECT_RECV_EXCEPTION *evt = (SIMCONNECT_RECV_EXCEPTION*)pData;
		  if (evt->dwException == 15) {
			  Metar = "ERROR METAR";
			  metar = std::string(Metar);
		  }
		  if (evt->dwException == SIMCONNECT_EXCEPTION_CREATE_OBJECT_FAILED) {
			  AddText(String::Format("SIMCONNECT_EXCEPTION_CREATE_OBJECT_FAILED", pData->dwID));
		  }
		  break;
	  }
	  default: {
		  AddText(String::Format("Received: {0}\n", pData->dwID));
		  break;
	  }
	}
}




void DrawAirport(IAirportData* PAirportData, double Lat, double Lon, double Hed) {
	if (aDC != NULL) {
		RECT rc;
		HWND PDraw = (HWND)MainForm::mainForm->AirportImage->Handle.ToPointer();
		GetClientRect(PDraw, &rc);
		MainForm::mainForm->AirportImage->Refresh();
		SelectObject(aDC, hPenSolid2Grey);
		double MinLat = 360.0, MinLon = 360.0, MaxLat = -360.0, MaxLon = -360.0;
		IAirport* Air = PAirportData->GetAirport();
		for (int i = 0; i < Air->PTaxiwayPoints->size(); i++) {
			if (SIMMATH::DecodeLat(Air->PTaxiwayPoints->at(i).Lat) < MinLat) {
				MinLat = SIMMATH::DecodeLat(Air->PTaxiwayPoints->at(i).Lat);
			}
			if (SIMMATH::DecodeLat(Air->PTaxiwayPoints->at(i).Lat) > MaxLat) {
				MaxLat = SIMMATH::DecodeLat(Air->PTaxiwayPoints->at(i).Lat);
			}
			if (SIMMATH::DecodeLon(Air->PTaxiwayPoints->at(i).Lon) < MinLon) {
				MinLon = SIMMATH::DecodeLon(Air->PTaxiwayPoints->at(i).Lon);
			}
			if (SIMMATH::DecodeLon(Air->PTaxiwayPoints->at(i).Lon) > MaxLon) {
				MaxLon = SIMMATH::DecodeLon(Air->PTaxiwayPoints->at(i).Lon);
			}
		}
		for (int i = 0; i < Air->PTaxiwayParks->size(); i++) {
			if (SIMMATH::DecodeLat(Air->PTaxiwayParks->at(i).Lat) < MinLat) {
				MinLat = SIMMATH::DecodeLat(Air->PTaxiwayParks->at(i).Lat);
			}
			if (SIMMATH::DecodeLat(Air->PTaxiwayParks->at(i).Lat) > MaxLat) {
				MaxLat = SIMMATH::DecodeLat(Air->PTaxiwayParks->at(i).Lat);
			}
			if (SIMMATH::DecodeLon(Air->PTaxiwayParks->at(i).Lon) < MinLon) {
				MinLon = SIMMATH::DecodeLon(Air->PTaxiwayParks->at(i).Lon);
			}
			if (SIMMATH::DecodeLon(Air->PTaxiwayParks->at(i).Lon) > MaxLon) {
				MaxLon = SIMMATH::DecodeLon(Air->PTaxiwayParks->at(i).Lon);
			}
		}
		double dLat = MaxLat - MinLat;
		double dLon = MaxLon - MinLon;
		double rcRate = (rc.right - 10.0) / (rc.bottom - 10.0);
		double AirRate = dLon / dLat;
		double drawRate;
		if (AirRate <= rcRate) {
			drawRate = (rc.bottom - 10.0) / dLat;
		}
		else {
			drawRate = (rc.right - 10.0) / dLon;
		}
		for (int i = 0; i < Air->PTaxiwayPaths->size(); i++) {
			if ((Air->PTaxiwayPaths->at(i).Type & 0xf) == 0x1) {
				SelectObject(aDC, hPenSolid2Blue);
			}
			if ((Air->PTaxiwayPaths->at(i).Type & 0xf) == 0x2) {
				SelectObject(aDC, hPenSolid2Grey);
			}
			if ((Air->PTaxiwayPaths->at(i).Type & 0xf) == 0x3) {
				SelectObject(aDC, hPenSolidRed);
			}
			if ((Air->PTaxiwayPaths->at(i).Type & 0xf) == 0x4) {
				SelectObject(aDC, hPenSolid2Green);
			}
			SelectObject(aDC, hPenSolid2Grey);
			if (((Air->PTaxiwayPaths->at(i).Type & 0xf) == 0x1) || ((Air->PTaxiwayPaths->at(i).Type & 0xf) == 0x2) || ((Air->PTaxiwayPaths->at(i).Type & 0xf) == 0x4)) {
				MoveToEx(aDC,
					((SIMMATH::DecodeLon(Air->PTaxiwayPoints->at(Air->PTaxiwayPaths->at(i).IndexStartPoint).Lon) - MinLon) * drawRate) + (((dLon * drawRate) - rc.right) / 2.0),
					(rc.bottom - ((SIMMATH::DecodeLat(Air->PTaxiwayPoints->at(Air->PTaxiwayPaths->at(i).IndexStartPoint).Lat) - MinLat) * drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)), NULL);
				LineTo(aDC,
					((SIMMATH::DecodeLon(Air->PTaxiwayPoints->at(Air->PTaxiwayPaths->at(i).IndexEndPoint & 0xfff).Lon) - MinLon)* drawRate) + (((dLon * drawRate) - rc.right) / 2.0),
					(rc.bottom - ((SIMMATH::DecodeLat(Air->PTaxiwayPoints->at(Air->PTaxiwayPaths->at(i).IndexEndPoint & 0xfff).Lat) - MinLat)* drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)));
			}
			if ((Air->PTaxiwayPaths->at(i).Type & 0xf) == 0x3) {
				MoveToEx(aDC,
					((SIMMATH::DecodeLon(Air->PTaxiwayPoints->at(Air->PTaxiwayPaths->at(i).IndexStartPoint).Lon) - MinLon) * drawRate) + (((dLon * drawRate) - rc.right) / 2.0),
					(rc.bottom - ((SIMMATH::DecodeLat(Air->PTaxiwayPoints->at(Air->PTaxiwayPaths->at(i).IndexStartPoint).Lat) - MinLat) * drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)), NULL);
				LineTo(aDC,
					((SIMMATH::DecodeLon(Air->PTaxiwayParks->at(Air->PTaxiwayPaths->at(i).IndexEndPoint & 0xfff).Lon) - MinLon)* drawRate) + (((dLon * drawRate) - rc.right) / 2.0),
					(rc.bottom - ((SIMMATH::DecodeLat(Air->PTaxiwayParks->at(Air->PTaxiwayPaths->at(i).IndexEndPoint & 0xfff).Lat) - MinLat)* drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)));
			}
			SelectObject(aDC, hPenSolid2Green);
			for (int i = 0; i < PAirportData->ReturnPath->size() - 1; i++) {
				MoveToEx(aDC,
					(((PAirportData->ReturnPath->at(i).Lon) - MinLon) * drawRate) + (((dLon * drawRate) - rc.right) / 2.0),
					(rc.bottom - (((PAirportData->ReturnPath->at(i).Lat) - MinLat) * drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)), NULL);
				LineTo(aDC,
					(((PAirportData->ReturnPath->at(i + 1).Lon) - MinLon)* drawRate) + (((dLon * drawRate) - rc.right) / 2.0),
					(rc.bottom - (((PAirportData->ReturnPath->at(i + 1).Lat) - MinLat)* drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)));
			}
		}
		double INF = 1000000000.0;
		double dMin = INF, dMax = 0.0;
		for (int i = 0; i < PAirportData->d->size() - 1; i++) {
			if ((PAirportData->d->at(i) < INF) && (PAirportData->d->at(i) > 0.0)) {
				if (PAirportData->d->at(i) < dMin) {
					dMin = PAirportData->d->at(i);
				}
				if (PAirportData->d->at(i) > dMax) {
					dMax = PAirportData->d->at(i);
				}
			}
		}
		double dD = (1279.0 / (dMax - dMin));
		for (int i = 0; i < PAirportData->d->size() - 2; i++) {
			if ((PAirportData->d->at(i) > 0.0)&&(PAirportData->d->at(i) < INF)) {
				int dRel = (PAirportData->d->at(i) - dMin) * dD;
				if (i == 700) {
					std::cout << 1;
				}
				int R, G, B;
				if ((dRel <= 255) && (dRel >= 0)) {
					R = 0;
					G = 0;
					B = dRel;
				}
				else if ((dRel <= 511) && (dRel >= 256)) {
					R = 0;
					G = (dRel - 256);
					B = 255;
				}
				else if ((dRel <= 767) && (dRel >= 512)) {
					R = 0;
					G = 255;
					B = 255 - (dRel - 512);
				}
				else if ((dRel <= 1023) && (dRel >= 768)) {
					R = dRel - 768;
					G = 255;
					B = 0;
				}
				else if ((dRel <= 1279) && (dRel >= 1024)) {
					R = 255;
					G = 255 - (dRel - 1024);
					B = 0;
				}
				else {
					std::cout << 1;
				}
				HPEN hPen = CreatePen(PS_SOLID, 2, RGB(R, G, B));
				SelectObject(aDC, hPen);
				if (i < Air->PTaxiwayPoints->size()) {
					Ellipse(aDC,
						((SIMMATH::DecodeLon(Air->PTaxiwayPoints->at(i).Lon) - MinLon) * drawRate) + (((dLon * drawRate) - rc.right) / 2.0) - 2,
						(rc.bottom - ((SIMMATH::DecodeLat(Air->PTaxiwayPoints->at(i).Lat) - MinLat) * drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)) - 2,
						((SIMMATH::DecodeLon(Air->PTaxiwayPoints->at(i).Lon) - MinLon) * drawRate) + (((dLon * drawRate) - rc.right) / 2.0) +2,
						(rc.bottom - ((SIMMATH::DecodeLat(Air->PTaxiwayPoints->at(i).Lat) - MinLat) * drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)) + 2);
				}
				else {
					Ellipse(aDC,
						((SIMMATH::DecodeLon(Air->PTaxiwayParks->at(i- Air->PTaxiwayPoints->size()).Lon) - MinLon) * drawRate) + (((dLon * drawRate) - rc.right) / 2.0) - 2,
						(rc.bottom - ((SIMMATH::DecodeLat(Air->PTaxiwayParks->at(i - Air->PTaxiwayPoints->size()).Lat) - MinLat) * drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)) - 2,
						((SIMMATH::DecodeLon(Air->PTaxiwayParks->at(i - Air->PTaxiwayPoints->size()).Lon) - MinLon) * drawRate) + (((dLon * drawRate) - rc.right) / 2.0) + 2,
						(rc.bottom - ((SIMMATH::DecodeLat(Air->PTaxiwayParks->at(i - Air->PTaxiwayPoints->size()).Lat) - MinLat) * drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)) + 2);
				}
				DeleteObject(hPen);
			}
		}
		SelectObject(aDC, hPenSolidRed);
		Ellipse(aDC,
			((Lon - MinLon) * drawRate) + (((dLon * drawRate) - rc.right) / 2.0) - 4,
			(rc.bottom - ((Lat - MinLat) * drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)) - 4,
			((Lon - MinLon) * drawRate) + (((dLon * drawRate) - rc.right) / 2.0) + 4,
			(rc.bottom - ((Lat - MinLat) * drawRate) + (((dLat * drawRate) - rc.bottom) / 2.0)) + 4);
	}
}




VOID CALLBACK TimerProc(_In_ HWND hwnd, _In_ UINT uMsg, _In_ UINT_PTR idEvent, _In_ DWORD dwTime) {
	int ii = 0; //Убрать
	if (!InTimer) {
		InTimer = true;
		hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
		hr = SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
		aircraftData* PaircraftData = &AircraftData;
		if (PAirportData == NULL) {
			if (PaircraftData->lat != 0.0 && PaircraftData->lon != 0.0) {
				PAirportData = ::GetAirportData();
				PAirportData->SetHSim(hSimConnect);
        while (1) {
          if (PAirportData->GetProgress() == 100) {
            break;
          }
        }
			}
		}
		else {
			SetLight(CabLight);
			if (WP != NULL) {
				hr = SimConnect_WeatherRequestInterpolatedObservation(hSimConnect, REQ_WEATHER_LATLON, WP->at(WP->size() - 1).lat, WP->at(WP->size() - 1).lon, WP->at(WP->size() - 1).alt);
			}
		  switch (mode) {
		    case PREPARE: {
			    AddText("MODE: PREPARE" + "\n", true);
			    SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_5, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			    SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			    if (GetNGXData) {
				    if (AircraftData.simOnGround == TRUE) {
					    double AngleToDesc = GetDescentAngle(Ways);
					    Flight.cruiseLevel = CruiseLevel;
					    setNGXDataBool(EVT_MCP_FD_SWITCH_L, TRUE, &NGX.MCP_FDSw[0]);
					    setNGXDataBool(EVT_MCP_FD_SWITCH_R, TRUE, &NGX.MCP_FDSw[1]);
					    setNGXDataBool(EVT_MCP_AT_ARM_SWITCH, TRUE, &NGX.MCP_annunATArm);
					    setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 161, &NGX.MCP_IASMach);
					    setNGXDataUShort(EVT_MCP_HEADING_SELECTOR, 80, &NGX.MCP_Heading);
					    setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, Flight.cruiseLevel, &NGX.MCP_Altitude);
					    setNGXDataDouble(EVT_CONTROL_STAND_TRIM_WHEEL, 6.44 - 3.9, &AircraftData.elevTrim);
					    setNGXDataDouble(EVT_EFIS_CPT_BARO, AircraftData.Pressure, &AircraftData.PressureSet, 0.01, -0.01, TRUE);
					    setNGXDataBool(EVT_OH_LIGHTS_LOGO, TRUE, &NGX.LTS_LogoSw);
					    setNGXDataBool(EVT_OH_LIGHTS_TAXI, TRUE, &NGX.LTS_TaxiSw);
					    setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
					    //setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, TRUE, &NGX.LTS_RunwayTurnoffSw[0]);
					    //setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, TRUE, &NGX.LTS_RunwayTurnoffSw[1]);
					    setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 0, &NGX.LTS_PositionSw, FALSE);
              //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_POS_STROBE, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					    setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0], TRUE);
					    setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1], TRUE);
					    setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[0]);
					    setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[1]);
					    SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					    mode = TAXI_OUT;
				    }
				    else {
					    //CurrentWay = 12;
					    //mode = CLIMB;
				    }
			    }
			    break;
		    }
		    case START: {
			    for (int u = 0; u < 10; u++) {
				    IDLights->push_back(0);
			    }
			    CabLight = 4;
			    SetLight(4);
			    SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
			    if (!FW) {
				    FillWays();
			    }
			    CabLight = 0;
			    SetLight(0);
			    SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
			    DateTime Dt = DateTime(2018, 12, 9, 3, 15, 0);
			    if (DateTime::Now < Dt) {
				    AddText("MODE: " + "START" + "\n" + DateTime::Now, true);
			      FW = true;
				    break;
			    }
			    AddText("MODE: " + "START" + "\n", true);
			    AirportList ApList = PAirportData->GetNearAirport(PaircraftData->lat, PaircraftData->lon);
			    if (ApList.ICAO != "PAPO") {
				    DWORD index = PAirportData->GetNearTaxiwayPoint(PaircraftData->lat, PaircraftData->lon);
				    std::vector<TPath> Path = PAirportData->GetPath(index, PaircraftData->hed, PaircraftData->WindDirection);
				    for (int i = 0; i < Path.size() - 1; i++) {
					    AddWayPoint(Path.at(i).Lon, Path.at(i).Lat, 0.0, Path[i].Type, Path.at(i).name);
				    }
				    SIMMATH::DSHEH HeadTakeOff = { Path.at(Path.size() - 2).Lat,Path.at(Path.size() - 2).Lon,0.0,Path.at(Path.size() - 1).Lat,Path.at(Path.size() - 1).Lon,0.0 };
				    SIMMATH::DOrtoKM(&HeadTakeOff);
				    AddWayPoint(Path.at(Path.size() - 1).Lon, Path.at(Path.size() - 1).Lat, Flight.cruiseLevel, RUNWAY, Path.at(Path.size() - 1).name);
				    SIDPoint = CountWays-2;
				    Ways[SIDPoint]->nameEndPoint = AddSID(PAirportData, PaircraftData, &Path.at(Path.size() - 1), HeadTakeOff.EH);
				    RoutePoint = CountWays;
			    }
			    else {
				    AddWayPoint(WP->at(1).lon, WP->at(1).lat, CruiseLevel, FIX, WP->at(1).ICAO, 0, CRUISEALT);
			    }
			    CabLight = 4;
			    SetLight(4);
			    Flight.cruiseLevel = CruiseLevel;
			    for (int kk = 2; kk < WP->size() - 1; kk++) {
				    AddWayPoint(WP->at(kk).lon, WP->at(kk).lat, Flight.cruiseLevel, WAYPOINT, WP->at(kk).ICAO, 0, CRUISEALT);
			    }
			    AddWayPoint(WP->at(WP->size() - 1).lon, WP->at(WP->size() - 1).lat, WP->at(WP->size() - 1).alt, FIX, WP->at(WP->size() - 1).ICAO, 0, FIXALT);
			    STARPoint = CountWays - 1;
			    STAR = false;
			    CabLight = 1;
			    SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
			    mode = PUSHBACK;
          //mode = CLIMB;
			    break;
		    }
		    case PUSHBACK: {
			    hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
          hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_SPEED, DEF_SPEED, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
			    DrawAirport(PAirportData, PaircraftData->lat, PaircraftData->lon, PaircraftData->hed);
			    aircraftData* PaircraftData = &AircraftData;
			    setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, Flight.cruiseLevel, &NGX.MCP_Altitude);
			    speed = 5;
			    CurrentPos = *Ways[CurrentWay];
			    if (CurrentPos.typePath == 11) {
				    CurrentPos.Slla.Latitude = PaircraftData->lat;
				    CurrentPos.Slla.Longitude = PaircraftData->lon;
				    SIMMATH::DOrtoKM(&CurrentPos);
				    int icw = CurrentWay;
				    double DCommon = CurrentPos.D;
				    while ((Ways[icw]->typePath != RUNWAY) && (icw < (CountWays - 1))) {
					    DCommon += Ways[icw + 1]->D;
					    icw++;
				    }
				    CHAR s[256];
				    double REangle = 0.045;
				    if ((CurrentWay + 1) >= CountWays) {
					    REangle = (Ways[0]->SH - Ways[CurrentWay]->EH);
				    }
				    else {
					    REangle = Ways[CurrentWay + 1]->SH - Ways[CurrentWay]->EH;
				    }
				    if (REangle > 180) {
					    REangle = REangle - 360;
				    }
				    else if (REangle < -180) {
					    REangle = REangle + 360;
				    }
				    REangle = (REangle * 10 * 2 / 100000);
				    double EangleRel = CurrentPos.EH - Ways[CurrentWay]->EH;
				    if (EangleRel > 180) {
					    EangleRel = EangleRel - 360;
				    }
				    else if (EangleRel < -180) {
					    EangleRel = EangleRel + 360;
				    }
				    if ((EangleRel > 60) || (EangleRel < -60) || (CurrentPos.D < abs(REangle))) {
					    CurrentWay = CurrentWay + 1;

				    }
				    else {
					    double HeadingRel = PBHeadWithWay(Ways[CurrentWay]);
					    if ((HeadingRel < 5) && (EangleRel < 5) && (HeadingRel > -5) && (EangleRel > -5) && (CurrentPos.D > 0.2)) {
						    speed = 2;
					    }
					    else {
						    speed = 5;
					    }
				    }
			    }
			    else {
				    CabLight = 1;
				    mode = PREPARE;
			    }
          
			    break;
		    }
		    case TAXI_OUT: {
			    hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
			    aircraftData* PaircraftData = &AircraftData;
			    DrawAirport(PAirportData, PaircraftData->lat, PaircraftData->lon, PaircraftData->hed);
			    CurrentPos = *Ways[CurrentWay];
			    CurrentPos.Slla.Latitude = PaircraftData->lat;
			    CurrentPos.Slla.Longitude = PaircraftData->lon;
			    SIMMATH::DOrtoKM(&CurrentPos);
			    int icw = CurrentWay;
			    double DCommon = CurrentPos.D;
			    while ((Ways[icw]->typePath != RUNWAY) && (icw < (CountWays - 1))) {
				    DCommon += Ways[icw + 1]->D;
				    icw++;
			    }
			    CHAR s[256];
			    sprintf(s, "MODE: TAXI_OUT To \"%s\" - \"%s\" Current: %.3f km Total: %.3f km \n Speed %.3f; Current way: %d Name: \"%s\" Type: %d Total ways: %d", Ways[icw-1]->nameEndPoint.c_str(), Ways[icw]->nameEndPoint.c_str(), CurrentPos.D, DCommon, speed, CurrentWay, CurrentPos.nameEndPoint.c_str(), CurrentPos.typePath, CountWays);
			    AddText(gcnew String(s), true);
			    double kDToHed = 0.00004;
			    double DToHed;
			    double REangle;
			    REangle = GetAnglePM180(Ways[CurrentWay + 1]->SH - Ways[CurrentWay]->EH);
			    DToHed = abs(REangle) * PMDG_TEST::speeds.GS * kDToHed;
			    if (DToHed < 0.025) {
				    DToHed = 0.025;
			    }
			    double EangleRel = GetAnglePM180(CurrentPos.EH - Ways[CurrentWay]->EH);
			    double a = GetFixDA(sin(EangleRel*M_PI / 180)* CurrentPos.D, EangleRel);
			    double HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
			    if ((abs(EangleRel) > 60) || (CurrentPos.D < DToHed)) {
				    CurrentWay = CurrentWay + 1;
			    }
			    else {
				    if ((abs(HeadingRel) < 5.0) && (abs(a) < 0.005) && (CurrentPos.D > 0.200)) {
					    if (((CurrentWay + 1) != CountWays) && ((Ways[CurrentWay]->typePath == RUNWAY))) {
						    speed = 0;
						    if (PaircraftData->GS < 1) {
							    setNGXDataDouble(EVT_EFIS_CPT_BARO, AircraftData.Pressure, &AircraftData.PressureSet, 0.01, -0.01, TRUE);
							    setNGXDataBool(EVT_MCP_AT_ARM_SWITCH, TRUE, &NGX.MCP_annunATArm);
							    setThrust(40);
							    speed = 180;
							    mode = TAKE_OFF;
						    }
					    }
					    else {
						    speed = 20;
					    }
				    }
				    else {
					    if ((abs(EangleRel) > 60) || (CurrentPos.D < (DToHed + 0.050))) {
						    speed = 20;
						    if (abs(REangle) > 10) {
							    speed = 15;
						    }
						    if (abs(REangle) > 30) {
							    speed = 12;
						    }
						    if (abs(REangle) > 50) {
							    speed = 10;
						    }
						    if (abs(REangle) > 100) {
							    speed = 5;
						    }
						    if (abs(HeadingRel) > 5) {
							    speed = 5;
						    }
					    }
					    else {
						    speed = 20;
					    }
					    if (abs(a) > 0.005) {
						    if ((abs(HeadingRel) > 5)) {
							    speed = 5;
						    }
						    else if((abs(HeadingRel) > 10)) {
							    speed = 7;
						    }
						    else if ((abs(HeadingRel) > 15)) {
							    speed = 10;
						    }
						    else {
							    speed = 12;
						    }
					    }					
				    }
				    if (PaircraftData->GS > (speed + 2)) {
					    double brake = (PaircraftData->GS - speed) / 70.0;
					    setThrust(0);
					    brakes.LBrake = brake;
					    brakes.RBrake = brake;
				    }
				    else if (PaircraftData->GS < (speed)) {
					    brakes.LBrake = 0.0;
					    brakes.RBrake = 0.0;
				    }
				    SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
			    }
			    break;
		    }
		    case TAKE_OFF: {
          static int StartThrottle = 0;
          aircraftData* PaircraftData = &AircraftData;
          hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
          static clock_t sTime = 0; //Начальное время
          clock_t eTime = clock();
          static double sHed;
          static double sSpeed;
          double eHed = PaircraftData->hed;
          double eSpeed = PaircraftData->GS;
          double NPitch = 0;
          if (ExportData.HorEnabled == 1) {
            NPitch = -ExportData.HorBar;
          }
          else {
            NPitch = -15;
          }
          setNGXDataBool(EVT_OH_LIGHTS_LOGO, TRUE, &NGX.LTS_LogoSw);
          setNGXDataBool(EVT_OH_LIGHTS_TAXI, TRUE, &NGX.LTS_TaxiSw);
          setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
          //setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, TRUE, &NGX.LTS_RunwayTurnoffSw[0]);
          //setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, TRUE, &NGX.LTS_RunwayTurnoffSw[1]);
          //setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, TRUE);
          setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, FALSE);
          setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 2, &NGX.LTS_LandingLtRetractableSw[0], TRUE);
          setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 2, &NGX.LTS_LandingLtRetractableSw[1], TRUE);
          //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_POS_STROBE, 2, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_L_RETRACT, 2, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_R_RETRACT, 2, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, TRUE, &NGX.LTS_LandingLtFixedSw[0]);
          setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, TRUE, &NGX.LTS_LandingLtFixedSw[1]);
          //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_LOGO, 1, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_TAXI, 1, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_ANT_COL, 1, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_L_TURNOFF, 1, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_R_TURNOFF, 1, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_L_FIXED, 1, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_OH_LIGHTS_R_FIXED, 1, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          if (PaircraftData->N1L > 40) {
            if (!TOGA) {
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_TOGA, 1, \
                SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              TOGA = TRUE;
            }
          }
          else {
            throttleLever(StartThrottle);
            if (SetTimeOff(TAKE_OFF, 3000)) {
              StartThrottle = StartThrottle + 4;
            }
          }
          if (PaircraftData->AS > 230) {
            //setNGXDataUChar(EVT_GEAR_LEVER, 1, &NGX.MAIN_GearLever, TRUE);
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_OFF, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          else if (PaircraftData->AS > 220) {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_0, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            if (ExportData.HorEnabled == 1) {
              NPitch = -ExportData.HorBar;
            }
            else {
              NPitch = -15;
            }
          }
          else if (PaircraftData->AS > 190) {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_1, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            if (ExportData.HorEnabled == 1) {
              NPitch = -ExportData.HorBar;
            }
            else {
              NPitch = -15;
            }
          }
          if ((PaircraftData->AS < 190) && (PaircraftData->simOnGround == FALSE)) {
            if (ExportData.HorEnabled == 1) {
              NPitch = -ExportData.HorBar;
            }
            else {
              NPitch = -15;
            }
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_RUDDER_SET, 0, \
              SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          if (PaircraftData->AS > 150) {
            if ((PaircraftData->VS > 10) && (PaircraftData->simOnGround == FALSE)) {
              double intpParameter = 0.0;
              SimConnect_SetDataOnSimObject(hSimConnect, DEF_AILERON, 0, 0, 0, sizeof(intpParameter), &intpParameter);
              //setNGXDataUChar(EVT_GEAR_LEVER, 0, &NGX.MAIN_GearLever, TRUE);
              //SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_LEVER, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              //Sleep(300);
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_UN, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_UN, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              //Sleep(300);              
              //SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_LEVER, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              //SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_LEVER, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_UN, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_UN, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              //SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_LEVER, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
             // hr = SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              TOGA = FALSE;
              CurrentWay = CurrentWay + 1;
              PPID.FT = true;
              mode = CLIMB;
            }
            if (PaircraftData->alt > 1800) {
              setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
            }
            if (!NGX.MCP_annunCMD_A) {
              if (ExportData.HorEnabled == 1) {
                ManPitchWithFD(-ExportData.HorBar, PaircraftData);
              }
              else {
                ManPitchWithFD(NPitch, PaircraftData);
              }
              if (ExportData.VerEnabled == 1) {
                ManBankWithFD(-ExportData.VerBar, PaircraftData);
              }
              else {
                ManBankWithFD(0, PaircraftData);
              }
            }
            else if (ExportData.HorEnabled == 1) {
              ManPitchWithFD(-ExportData.HorBar, PaircraftData);
            }
          }
          else if ((PaircraftData->AS > 140) && (PaircraftData->AS < 150)) {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ELEVATOR_SET, -5000, \
              SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          CurrentPos = *Ways[CurrentWay];
          CurrentPos.Slla.Latitude = PaircraftData->lat;
          CurrentPos.Slla.Longitude = PaircraftData->lon;
          SIMMATH::DOrtoKM(&CurrentPos);
          double DCommon = GetDistanceToWayType(GSWAY);
          double DToChange = CalcToNewWay(false);
          CHAR s[512];
          sprintf(s, "MODE: TAKEOFF Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Total ways: %d D for head: %.3f", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CountWays, DToChange);
          AddText(gcnew String(s), true);
          if (AircraftData.simOnGround == 1) {
            double HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
          }
          else {
            double HeadingRel = BankWithHead(GetAnglePM180(Ways[CurrentWay]->EH));
          }
          break;
        }
		    case ABORT_TAKE_OFF: {
					      break;
		    }
		    case CIRCLE_TAXI: {
					      break;
		    }
		    case CLIMB: {
          
          aircraftData* PaircraftData = &AircraftData;
          hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);

          if (PaircraftData->alt > 1800) {
            setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
          }
          if (PaircraftData->AS > 230) {
            //setNGXDataUChar(EVT_GEAR_LEVER, 1, &NGX.MAIN_GearLever, TRUE);
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_OFF, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          else if (PaircraftData->AS > 218) {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_0, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_UN, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_UN, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          else if (PaircraftData->AS > 190) {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_1, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_UN, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_UN, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            //SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_GEAR_LEVER, 0, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          else
          {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_UN, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_UN, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          if (NGX.MCP_annunCMD_A) {
            break;
          }
          CurrentPos = *Ways[CurrentWay];
          if (CurrentPos.Speed == 0.0) {
            if (PaircraftData->alt > 10500) {
              if (NGX.MCP_IASMach > 10.0) {
                setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 335, &NGX.MCP_IASMach);
              }
              else {
                setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 0.79, &NGX.MCP_IASMach);
              }
            }
            else if (PaircraftData->alt < 9500) {
!              setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 250, &NGX.MCP_IASMach);
            }
          }
          else {
            setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, Ways[CurrentWay]->Speed, &NGX.MCP_IASMach);
          }
          if (PaircraftData->alt < 10000) {
            setNGXDataBool(EVT_OH_LIGHTS_LOGO, TRUE, &NGX.LTS_LogoSw);
            setNGXDataBool(EVT_OH_LIGHTS_TAXI, TRUE, &NGX.LTS_TaxiSw);
            setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
            //setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[0]);
            //setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[1]);
            setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, FALSE);
            setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0], TRUE);
            setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1], TRUE);
            setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[0]);
            setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[1]);
          }
          else {
            setNGXDataBool(EVT_OH_LIGHTS_LOGO, FALSE, &NGX.LTS_LogoSw);
            setNGXDataBool(EVT_OH_LIGHTS_TAXI, FALSE, &NGX.LTS_TaxiSw);
            setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
            //setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[0]);
            //setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[1]);
            setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, FALSE);
            setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0], TRUE);
            setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1], TRUE);
            setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[0]);
            setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[1]);
          }
          if (PaircraftData->alt < 10000) {
            CabLight = 1;
          }
          else {
            CabLight = 4;
          }
          if (PaircraftData->alt > 28000) {
            setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 0.79, &NGX.MCP_IASMach);
          }
          if (PaircraftData->alt > 3600) {
            if (((PaircraftData->PressureSet - 0.01) > 29.92) || ((PaircraftData->PressureSet + 0.01) < 29.92)) {
              Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
              Control.Event = EVT_EFIS_CPT_BARO_STD;
              SimConnect_SetClientData(hSimConnect, PMDG_NG3_CONTROL_ID, PMDG_NG3_CONTROL_DEFINITION,
                0, 0, sizeof(PMDG_NG3_Control), &Control);
            }
          }
          CurrentPos.Slla.Latitude = PaircraftData->lat;
          CurrentPos.Slla.Longitude = PaircraftData->lon;
          SIMMATH::DOrtoKM(&CurrentPos);
          double DCommon = GetDistanceToWayType(GSWAY);
          if (((DCommon < 230.0) || ((CurrentWay + 1) == CountWays)) && (!STAR)) {
            AirportList ApList = PAirportData->GetNearAirport(WP->at(WP->size() - 1).lat, WP->at(WP->size() - 1).lon);
            std::vector<int>* StartIndex;
            if (DestMetar.WindSpeed >= 0.0) {
              StartIndex = PAirportData->GetRunwayStart(PaircraftData->lat, PaircraftData->lon, DestMetar.WindDir);
            }
            else {
              StartIndex = PAirportData->GetRunwayStart(PaircraftData->lat, PaircraftData->lon, PaircraftData->WindDirection);
            }
            for (int iii = 0; iii < StartIndex->size(); iii++) {
              DATA_RUNWAY runway = PAirportData->GetStartCoord(StartIndex->at(iii));
              SIMCONNECT_DATA_LATLONALT latlon = SIMMATH::GetDALatLon(runway.sLatitude, runway.sLongitude, runway.eHeading, 18.0);
              CountWays = CountWays - 1;
              STARPoint = CountWays - 1;
              Ways[STARPoint]->nameEndPoint = AddSTAR(PAirportData, PaircraftData, &runway);
              if (Ways[STARPoint]->nameEndPoint != "") {
                break;
              }
            }
            STAR = true;
          }
          CHAR s[512];
          double AngleToDesc = GetDescentAngle(Ways);
          setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, Flight.cruiseLevel, &NGX.MCP_Altitude);
          if (AngleToDesc >= GetAngleToDesc(PaircraftData->alt)) {
            mode = DESCENT;
          }
          if (CurrentPos.typePath != GSWAY) {
            int Alt = Flight.cruiseLevel / 100;
            double HeadingRel;
            if ((!NGX.MCP_annunCMD_A) && (PaircraftData->simOnGround == 0)) {
              if (ExportData.HorEnabled == 1) {
                AltPitchWithPos(-ExportData.HorBar);
              }
              else {
                AltPitchWithPos(-10);
              }
              if ((CurrentPos.typePath == HEADINGUNTILALT) || (CurrentPos.typePath == HEADINGUNTILFROMDIST)) {
                double AngleWind = PaircraftData->WindDirection - CurrentPos.SH;
                double AngleDrift = asin(sin(AngleWind / 180 * M_PI) * PaircraftData->WindSpeed / PaircraftData->TS) * 180 / M_PI;
                HeadingRel = BankWithHead(GetAnglePM180(Ways[CurrentWay]->H + AircraftData.MagVar));
              }
              else if (CurrentPos.typePath == TRKINTERCEPTRADIAL) {
                double AngleWind = PaircraftData->WindDirection - CurrentPos.SH;
                double AngleDrift = asin(sin(AngleWind / 180 * M_PI) * PaircraftData->WindSpeed / PaircraftData->TS) * 180 / M_PI;
                HeadingRel = BankWithHead(GetAnglePM180(Ways[CurrentWay]->H + AircraftData.MagVar));
              }
              else {
                HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
              }
            }
            double DToChange = CalcToNewWay();
            sprintf(s, "MODE: CLIMB \"%s\" Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Name: \"%s\" Total ways: %d D for head: %.3f \n ATD: %.3f Current angle: %.3f", Ways[STARPoint]->nameEndPoint.c_str(), CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CurrentPos.nameEndPoint.c_str(), CountWays, DToChange, AngleToDesc, GetAngleToDesc(PaircraftData->alt));
            AddText(gcnew String(s), true);
          }
          else {
            mode = LANDING;
          }
          if ((PaircraftData->alt + 100) > Flight.cruiseLevel) {
            mode = CRUISE;
          }
          if (AngleToDesc >= ((PaircraftData->alt / 48148) + 2.19)) {
            setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
            mode = DESCENT;
          }
          break;
        }
		    case CRUISE: {
          CabLight = 2;
          aircraftData* PaircraftData = &AircraftData;
          hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
          CurrentPos = *Ways[CurrentWay];
          CurrentPos.Slla.Latitude = PaircraftData->lat;
          CurrentPos.Slla.Longitude = PaircraftData->lon;
          SIMMATH::DOrtoKM(&CurrentPos);
          int icw = CurrentWay;
          double DCommon = CurrentPos.D;
          while ((Ways[icw]->typePath != RUNWAY) && (icw < (CountWays - 1))) {
            DCommon += Ways[icw + 1]->D;
            icw++;
          }
          if (DCommon < 20) {
            CurrentWay = icw;
          }
          if (DCommon < 15) {
            setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 161, &NGX.MCP_IASMach);
          }
          else if (DCommon < 20) {
            setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 180, &NGX.MCP_IASMach);
          }
          else if (DCommon < 25) {
            setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 200, &NGX.MCP_IASMach);
          }
          else if (DCommon < 30) {
            setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 200, &NGX.MCP_IASMach);
          }
          if (((DCommon < 230.0) || ((CurrentWay + 1) == CountWays)) && (!STAR)) {
            AirportList ApList = PAirportData->GetNearAirport(WP->at(WP->size() - 1).lat, WP->at(WP->size() - 1).lon);
            std::vector<int>* StartIndex;
            if (DestMetar.WindSpeed >= 0.0) {
              StartIndex = PAirportData->GetRunwayStart(PaircraftData->lat, PaircraftData->lon, DestMetar.WindDir);
            }
            else {
              StartIndex = PAirportData->GetRunwayStart(PaircraftData->lat, PaircraftData->lon, PaircraftData->WindDirection);
            }
            for (int iii = 0; iii < StartIndex->size(); iii++) {
              DATA_RUNWAY runway = PAirportData->GetStartCoord(StartIndex->at(iii));
              SIMCONNECT_DATA_LATLONALT latlon = SIMMATH::GetDALatLon(runway.sLatitude, runway.sLongitude, runway.eHeading, 18.0);
              CountWays = CountWays - 1;
              STARPoint = CountWays - 1;
              Ways[STARPoint]->nameEndPoint = AddSTAR(PAirportData, PaircraftData, &runway);
              if (Ways[STARPoint]->nameEndPoint != "") {
                break;
              }
            }
            STAR = true;
          }
          CHAR s[512];
          if (PaircraftData->alt < 10000) {
            CabLight = 1;
          }
          else {
            CabLight = 4;
          }
          if (PaircraftData->alt < 4500)
          {
            setNGXDataDouble(EVT_EFIS_CPT_BARO, AircraftData.Pressure, &AircraftData.PressureSet, 0.01, -0.01, TRUE);
          }
          double AngleToDesc = GetDescentAngle(Ways);
          setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, Flight.cruiseLevel, &NGX.MCP_Altitude);
          if (AngleToDesc >= GetAngleToDesc(PaircraftData->alt)) {
            setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
            mode = DESCENT;
          }
          if ((PaircraftData->alt + 100) < Flight.cruiseLevel) {
            mode = CLIMB;
          }
          double HeadingRel;
          if ((!NGX.MCP_annunCMD_A) && (PaircraftData->simOnGround == 0)) {
            if (ExportData.HorEnabled == 1) {
              if (TestControl == FALSE) {
                AltPitchWithPos(-ExportData.HorBar);
              }
              else {
                AltPitchWithPos(pidPitch.inVal);
              }
            }
            else {
              AltPitchWithPos(-10);
            }
            if (ExportData.VerEnabled == 1) {
              if (TestControl == FALSE) {
                HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
              }
              else {
                AltVSpeedWithPitch(pidVSpeed.inVal);
              }
            }
            else {
              HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
            }
          }
          double DToChange = CalcToNewWay();
          sprintf(s, "MODE: CRUISE \"%s\" Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Name: \"%s\" Total ways: %d D for head: %.3f \n ATD: %.3f Current angle: %.3f", Ways[STARPoint]->nameEndPoint.c_str(), CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CurrentPos.nameEndPoint.c_str(), CountWays, DToChange, AngleToDesc, GetAngleToDesc(PaircraftData->alt));
          AddText(gcnew String(s), true);
          break;
        }
		    case DESCENT: {
          CabLight = 4;
          aircraftData* PaircraftData = &AircraftData;
          hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
          if (PaircraftData->alt < 4600) {
            if (((PaircraftData->PressureSet - 0.01) <= 29.92) && ((PaircraftData->PressureSet + 0.01) >= 29.92)) {
              Control.Parameter = MOUSE_FLAG_LEFTSINGLE;
              Control.Event = EVT_EFIS_CPT_BARO_STD;
              SimConnect_SetClientData(hSimConnect, PMDG_NG3_CONTROL_ID, PMDG_NG3_CONTROL_DEFINITION,
                0, 0, sizeof(PMDG_NG3_Control), &Control);
            }
            setNGXDataDouble(EVT_EFIS_CPT_BARO, AircraftData.Pressure, &AircraftData.PressureSet, 0.01, -0.01, TRUE);
          }
          if (!abortLanding2) {
            if (PaircraftData->AS < 165) {
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_30, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
            else if (PaircraftData->AS < 170) {
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_20, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
            else if (PaircraftData->AS < 175) {
              //setNGXDataUChar(EVT_GEAR_LEVER, 2, &NGX.MAIN_GearLever, TRUE);
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_LEVER, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              Sleep(300);
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_LEVER, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
            else if (PaircraftData->AS < 180) {
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_15, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
            else if (PaircraftData->AS < 200) {
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_5, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
            else if (PaircraftData->AS < 218) {
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_1, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
            else if (PaircraftData->AS < 230) {
              //setNGXDataUChar(EVT_GEAR_LEVER, 1, &NGX.MAIN_GearLever, TRUE);
              //SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_LEVER, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              //Sleep(300);
              //SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_LEVER, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
          }
          if ((PaircraftData->alt > 10000) && (PaircraftData->alt < 12000)) {
            if (NGX.MCP_IASMach > 10.0) {
              setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 240, &NGX.MCP_IASMach);
            }
          }
          else if (PaircraftData->alt >= 12000) {
            if (NGX.MCP_IASMach > 10.0) {
              setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 335, &NGX.MCP_IASMach);
            }
            else {
              setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
              setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 0.79, &NGX.MCP_IASMach);
            }
          }
          if (PaircraftData->alt < 10000) {
            setNGXDataBool(EVT_OH_LIGHTS_LOGO, TRUE, &NGX.LTS_LogoSw);
            setNGXDataBool(EVT_OH_LIGHTS_TAXI, TRUE, &NGX.LTS_TaxiSw);
            setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
            //setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[0]);
            //setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[1]);
            setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, FALSE);
            setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0], TRUE);
            setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1], TRUE);
            setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[0]);
            setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[1]);
          }
          else {
            setNGXDataBool(EVT_OH_LIGHTS_LOGO, FALSE, &NGX.LTS_LogoSw);
            setNGXDataBool(EVT_OH_LIGHTS_TAXI, FALSE, &NGX.LTS_TaxiSw);
            setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
            //setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[0]);
            //setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[1]);
            setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, FALSE);
            setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0], TRUE);
            setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1], TRUE);
            setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[0]);
            setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[1]);
          }
          if (PaircraftData->alt < 10000) {
            CabLight = 1;
          }
          else {
            CabLight = 4;
          }
          if (CurrentWay >= CountWays) {
            //CurrentWay = AppPoint;
            //abortLanding = false;
          }
          CurrentPos = *Ways[CurrentWay];
          CurrentPos.Slla.Latitude = PaircraftData->lat;
          CurrentPos.Slla.Longitude = PaircraftData->lon;
          SIMMATH::DOrtoKM(&CurrentPos);
          int icw = CurrentWay;
          double DCommon = CurrentPos.D;
          while ((Ways[icw]->typePath != RUNWAY) && (icw < (CountWays - 1))) {
            DCommon += Ways[icw + 1]->D;
            icw++;
          }
          if (!abortLanding) {
            if (DCommon < 15.0) {
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              flare = true;
              VSPID.FT = true;
              PPID.FT = true;
              mode = LANDING;
            }
            if (DCommon < 20) {
              setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 161, &NGX.MCP_IASMach);
            }
            else if (DCommon < 25) {
              setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 180, &NGX.MCP_IASMach);
            }
            else if (DCommon < 30) {
              setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 200, &NGX.MCP_IASMach);
            }
            else if (DCommon < 35) {
              setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 200, &NGX.MCP_IASMach);
            }
            else if (DCommon > 40 && PaircraftData->alt < 10000.0 && !abortLanding2) {
              setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 240, &NGX.MCP_IASMach);
            }
          }
          else {
            //setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 250, &NGX.MCP_IASMach);
            int appcount = CountWays - AppPoint;
            int newapppoint = CountWays;
            for (int ii = 0; ii < appcount; ii++) {
              //Ways[CountWays] = Ways[AppPoint+ii];
              AddWayPoint(Ways[AppPoint + ii]->Slla.Longitude,
                Ways[AppPoint + ii]->Slla.Latitude,
                Ways[AppPoint + ii]->Slla.Altitude,
                Ways[AppPoint + ii]->typePath,
                Ways[AppPoint + ii]->nameEndPoint,
                Ways[AppPoint + ii]->EH,
                Ways[AppPoint + ii]->fixAlt,
                Ways[AppPoint + ii]->Speed,
                Ways[AppPoint + ii]->H);
              //SIMMATH::DOrtoKM(Ways[CountWays]);
              //CountWays++;
            }
            AppPoint = newapppoint;
            abortLanding = false;
          }
          CHAR s[512];
          if (((DCommon < 230.0) || ((CurrentWay + 1) == CountWays)) && (!STAR)) {
            AirportList ApList = PAirportData->GetNearAirport(WP->at(WP->size() - 1).lat, WP->at(WP->size() - 1).lon);
            std::vector<int>* StartIndex;
            if (DestMetar.WindSpeed >= 0.0) {
              StartIndex = PAirportData->GetRunwayStart(PaircraftData->lat, PaircraftData->lon, DestMetar.WindDir);
            }
            else {
              StartIndex = PAirportData->GetRunwayStart(PaircraftData->lat, PaircraftData->lon, PaircraftData->WindDirection);
            }
            for (int iii = 0; iii < StartIndex->size(); iii++) {
              DATA_RUNWAY runway = PAirportData->GetStartCoord(StartIndex->at(iii));
              SIMCONNECT_DATA_LATLONALT latlon = SIMMATH::GetDALatLon(runway.sLatitude, runway.sLongitude, runway.eHeading, 18.0);
              CountWays = CountWays - 1;
              STARPoint = CountWays - 1;
              Ways[STARPoint]->nameEndPoint = AddSTAR(PAirportData, PaircraftData, &runway);
              if (Ways[STARPoint]->nameEndPoint != "") {
                break;
              }
            }
            STAR = true;
          }
          double currentAngle = GetDescentAngle(Ways);
          double VSD = ManVSWithAngle(currentAngle);
          int VS = VSD;
          if (SetTimeOff(DESCENT, 30000)) {
            if ((VSD + (20.0 * DCommon)) < (PaircraftData->VS * 60.0)) {
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
            else {
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            }
          }
          if ((CurrentPos.typePath != RUNWAY) && ((CurrentPos.typePath != GSWAY))) {
            int Alt = CurrentPos.TargetAlt / 100;
            setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, Alt * 100, &NGX.MCP_Altitude);
            if (abortLanding2 && PaircraftData->alt > 1800.0) {
              setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
              abortLanding2 = false;
            }
            double HeadingRel;
            if ((!NGX.MCP_annunCMD_A) && (PaircraftData->simOnGround == 0)) {
              if (ExportData.HorEnabled == 1) {
                if (TestControl == FALSE) {
                  AltPitchWithPos(-ExportData.HorBar);
                }
                else {
                  AltPitchWithPos(pidPitch.inVal);
                }
              }
              else {
                AltPitchWithPos(-10);
              }
              if (ExportData.VerEnabled == 1) {
                if (TestControl == FALSE) {
                  HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
                }
                else {
                  AltVSpeedWithPitch(pidVSpeed.inVal);
                }
              }
              else {
                HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
              }
            }
            double DToChange = CalcToNewWay();
            sprintf(s, "MODE: DESCENT \"%s\" Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Name: \"%s\" Total ways: %d D for head: %.3f \nVS: %.d CurrentAngle to EOD: %.3f", Ways[STARPoint]->nameEndPoint.c_str(), CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CurrentPos.nameEndPoint.c_str(), CountWays, DToChange, VS, currentAngle);
            AddText(gcnew String(s), true);
          }
          else {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
            flare = true;
            VSPID.FT = true;
            PPID.FT = true;
            mode = LANDING;
          }
          break;
        }
        case LANDING: {
          aircraftData* PaircraftData = &AircraftData;
          hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
          int Alt = Ways[CurrentWay]->Ella.Altitude / 100;
          setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, 2700, &NGX.MCP_Altitude);
          if (SetTimeOff(5, 5000)) {
            setNGXDataBool(EVT_MCP_VS_SWITCH, TRUE, &NGX.MCP_annunVS);
          }
          while ((Ways[CurrentWay]->typePath != RUNWAY) && CurrentWay < CountWays) {
            CurrentWay = CurrentWay + 1;
          }
          
          CurrentPos = *Ways[CurrentWay];
          CurrentPos.Slla.Latitude = PaircraftData->lat;
          CurrentPos.Slla.Longitude = PaircraftData->lon;
          SIMMATH::DOrtoKM(&CurrentPos);
          double TAlt = 0.0;
          int VS = ManVSWithGlide(Ways[CurrentWay], 3, TAlt);
          if (PaircraftData->alt < 1000.0 && (VS < -1000.0 || PaircraftData->AS > 170.0)) {
            //mode = ABORT_LANDING;
          }
          if (VS > 1800) {
            VS = 1800;
          }
          else if (VS < -1100) {
            VS = -1000;
          }
          else if (VS < -2000) {
            //mode = ABORT_LANDING;
          }
          double DCommon = CurrentPos.D;
          if (DCommon < 15) {
            setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 161, &NGX.MCP_IASMach);
          }
          else if (DCommon < 20) {
            setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 180, &NGX.MCP_IASMach);
          }
          else if (DCommon < 25) {
            setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 200, &NGX.MCP_IASMach);
          }
          else if (DCommon < 30) {
            setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 230, &NGX.MCP_IASMach);
          }
          if ((PaircraftData->AS < 168)) {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_30, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          else if ((PaircraftData->AS < 175) && (PaircraftData->AS > 170)) {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_20, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          else if ((PaircraftData->AS < 190) && (PaircraftData->AS > 180)) {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_15, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          else if ((PaircraftData->AS < 215) && (PaircraftData->AS > 200)) {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_5, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          else if ((PaircraftData->AS < 222) && (PaircraftData->AS > 218)) {
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_1, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          else if ((PaircraftData->AS < 230) && (PaircraftData->AS > 225)) {
            //setNGXDataUChar(EVT_GEAR_LEVER, 1, &NGX.MAIN_GearLever, TRUE);
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_LEVER, MOUSE_FLAG_RIGHTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          if ((PaircraftData->AS < 173)) {
            //setNGXDataUChar(EVT_GEAR_LEVER, 2, &NGX.MAIN_GearLever, TRUE);
            SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_GEAR_LEVER, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
          }
          if (PaircraftData->alt < 10000) {
            CabLight = 1;
          }
          else {
            CabLight = 4;
          }
          setNGXDataShort(EVT_MCP_VS_SELECTOR, VS, &NGX.MCP_VertSpeed, TRUE);
          double HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
          if ((!NGX.MCP_annunCMD_A) && (PaircraftData->simOnGround == 0)) {
            if (ExportData.HorEnabled == 1) {
              if (TestControl == FALSE) {
                AltVSpeedWithPitch(VS);
              }
              else {
                AltVSpeedWithPitch(VS);
              }
            }
            else {
              AltVSpeedWithPitch(VS);
            }
            if (ExportData.VerEnabled == 1) {
              if (TestControl == FALSE) {
                HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
              }
              else {
                AltBankWithPos(pidBank.inVal);
              }
            }
            else {
              HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
            }
          }
          CHAR s[512];
          sprintf(s, "MODE: LANDING Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Name: \"%s\" Total ways: %d \nVS: %.d", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CurrentPos.nameEndPoint.c_str(), CountWays, VS);
          AddText(gcnew String(s), true);
          if (PaircraftData->GAlt < 200) {
            setNGXDataBool(EVT_MCP_AT_ARM_SWITCH, FALSE, &NGX.MCP_annunATArm);
            if ((PaircraftData->AS > 80) && (PaircraftData->simOnGround == 1)) {
              SimConnect_TransmitClientEvent(hSimConnect, -5000, EVENT_ELEVATOR_SET, 0, \
                SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              if (SetTimeOff(LANDING, 2000)) {
                throttleLever(-37);
                brakes.LBrake = 0.0;
                brakes.RBrake = 0.0;
                SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
                SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              }
            }
            else {
              brakes.LBrake = 0.4;
              brakes.RBrake = 0.4;
              SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
              if (PaircraftData->GAlt < 50.0) {
                throttleLever(0);
              }
            }
            if (PaircraftData->GS <= 35.0) {
              SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
              brakes.LBrake = 0.0;
              brakes.RBrake = 0.0;
              SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
              VSPID.FT = true;
              AirportList ApList = PAirportData->GetNearAirport(PaircraftData->lat, PaircraftData->lon);
              DWORD indexPath = PAirportData->GetNearTaxiwayPath(PaircraftData->lat, PaircraftData->lon, PaircraftData->hed);
              TaxiwayParks* Park = new TaxiwayParks();
              std::vector<TPath> Path = PAirportData->GetPathGate(indexPath, PaircraftData->hed, 15.0, Park);
              DWORD PI = Park->TaxiParkInfo;
              DWORD PIi = (PI >> 12) & 0xfff;
              DWORD PIt = (PI >> 8) & 0xf;
              DWORD PIn = PI & 0x1f;
              ParkN = new std::string(ParkType[PIt] + " " + std::to_string(PIi) + " " + ParkName[PIn]);
              CountWays = CurrentWay + 1;
              AddWayPoint(PaircraftData->lon, PaircraftData->lat, 0.0, RUNWAY, "Go out");
              for (int i = 0; i < Path.size() - 1; i++) {
                AddWayPoint(Path.at(i).Lon, Path.at(i).Lat, 0.0, Path[i].Type, Path.at(i).name);
              }
              AddWayPoint(Path.at(Path.size() - 1).Lon, Path.at(Path.size() - 1).Lat, Flight.cruiseLevel, GATE, Path.at(Path.size() - 1).name);
              CurrentWay = CurrentWay + 2;
              mode = TAXI_IN;
            }
          }
          break;
        }
		    case ABORT_LANDING: {
			    SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_TOGA, 1, \
				    SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			    TOGA = TRUE;
			    SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_15, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			    //setNGXDataUChar(EVT_GEAR_LEVER, 0, &NGX.MAIN_GearLever);
			    CurrentWay++;
			    mode = DESCENT;
          abortLanding = true;
          abortLanding2 = true;
					break;
		    }
        case BRAKE: {
          break;
        }
		    case TAXI_IN:{
			    hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
			    aircraftData* PaircraftData = &AircraftData;
			    DrawAirport(PAirportData, PaircraftData->lat, PaircraftData->lon, PaircraftData->hed);
			    CurrentPos = *Ways[CurrentWay];
			    CurrentPos.Slla.Latitude = PaircraftData->lat;
			    CurrentPos.Slla.Longitude = PaircraftData->lon;
			    SIMMATH::DOrtoKM(&CurrentPos);
			    int icw = CurrentWay;
			    double DCommon = CurrentPos.D;
			    while ((Ways[icw]->typePath != RUNWAY) && (icw < (CountWays - 1))) {
				    DCommon += Ways[icw + 1]->D;
				    icw++;
			    }
			    CHAR s[256];
			    sprintf(s, "MODE: TAXI_IN to: \"%s\" Current: %.3f km Total: %.3f km \n Speed %.3f; Current way: %d Name: \"%s\" Type: %d Total ways: %d", ParkN->c_str(), CurrentPos.D, DCommon, speed, CurrentWay, CurrentPos.nameEndPoint.c_str(), CurrentPos.typePath, CountWays);
			    AddText(gcnew String(s), true);
			    double kDToHed = 0.00004;
			    double DToHed;
			    double REangle;
			    if ((CurrentWay + 1) >= CountWays) {
				    REangle = GetAnglePM180(Ways[CurrentWay]->SH - Ways[CurrentWay]->EH);
			    }
			    else {
				    REangle = GetAnglePM180(Ways[CurrentWay + 1]->SH - Ways[CurrentWay]->EH);
			    }
			    DToHed = abs(REangle) * PMDG_TEST::speeds.GS * kDToHed;
			    if (DToHed < 0.025) {
				    DToHed = 0.025;
			    }
			    if ((CurrentWay + 1) >= CountWays) {
				    DToHed = 0.001;
			    }
			    double EangleRel = GetAnglePM180(CurrentPos.EH - Ways[CurrentWay]->EH);
			    double a = GetFixDA(sin(EangleRel*M_PI / 180)* CurrentPos.D, EangleRel);
			    double HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
			    if (minDCommon > DCommon) {
				    minDCommon = DCommon;
			    }
			    if (((CurrentWay + 1) >= CountWays) && (DCommon < 0.003) && ((minDCommon + 0.001) < DCommon)) {
				    speed = 0;
				    double brake = 0.5; // (PaircraftData->GS - speed) / 70.0;
				    setThrust(0);
				    brakes.LBrake = brake;
				    brakes.RBrake = brake;
				    if (PaircraftData->GS < 0.05) {
					    setThrust(0);
					    brakes.LBrake = 1.0;
					    brakes.RBrake = 1.0;
					    CabLight = 4;
					    SetLight(4);
					    AddText(gcnew String("DONE"), true);
					    CurrentWay = CurrentWay + 1;
				    }
				    SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
			    }
			    if ((abs(EangleRel) > 60) || (CurrentPos.D < DToHed)) {
				    if ((CurrentWay + 1) >= CountWays) {
					    speed = 0;
					    double brake = 0.5; // (PaircraftData->GS - speed) / 70.0;
					    setThrust(0);
					    brakes.LBrake = brake;
					    brakes.RBrake = brake;
					    if (PaircraftData->GS < 0.05)
					    {
						    setThrust(0);
						    brakes.LBrake = 1.0;
						    brakes.RBrake = 1.0;
						    CabLight = 4;
						    SetLight(4);
						    AddText(gcnew String("DONE"), true);
						    CurrentWay = CurrentWay + 1;
					    }
					    SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
				    }
				    else {
					    CurrentWay = CurrentWay + 1;
				    }
			    }
			    else
			    {
            if ((abs(HeadingRel) < 5.0) && (abs(a) < 0.005) && (CurrentPos.D > 0.200)) {
              speed = 20;
            }
				    else {
					    if ((abs(EangleRel) > 60) || (CurrentPos.D < (DToHed + 0.050))) {
						    speed = 20;
						    if (abs(REangle) > 10) {
							    speed = 15;
						    }
						    if (abs(REangle) > 30) {
							    speed = 12;
						    }
						    if (abs(REangle) > 50) {
							    speed = 10;
						    }
						    if (abs(REangle) > 100) {
							    speed = 5;
						    }
						    if (abs(HeadingRel) > 5) {
							    speed = 5;
						    }
					    }
					    else {
						    speed = 20;
					    }
					    if (abs(a) > 0.005) {
						    if ((abs(HeadingRel) > 5)) {
							    speed = 5;
						    }
						    else if ((abs(HeadingRel) > 10)) {
							    speed = 7;
						    }
						    else if ((abs(HeadingRel) > 15)) {
							    speed = 10;
						    }
						    else {
							    speed = 12;
						    }
					    }
					    if (DCommon < 0.2) {
						    speed = 10;
						    if (DCommon < 0.100) {
							    speed = 5;
						    }
						    if (DCommon < 0.002) {
							    setThrust(0);
							    speed = 0;
							    brakes.LBrake = 0.1;
							    brakes.RBrake = 0.1;
							    SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
							    if (PaircraftData->GS < 0.5) {
								    setThrust(0);
								    speed = 0;
								    brakes.LBrake = 0;
								    brakes.RBrake = 0;
								    SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
								    mode = SHUTDOWN;
							    }
						    }
						    if ((CurrentWay + 1) >= CountWays) {
							    speed = 3;
						    }
						    if (DCommon < 0.100) {
							    speed = 3;
						    }
					    }
				    }
				    if (DCommon < 0.2) {
					    if (PaircraftData->GS >(speed + 1)) {
						    double brake = (PaircraftData->GS - speed) / 70.0;
						    setThrust(0);
						    brakes.LBrake = brake;
						    brakes.RBrake = brake;
					    }
					    else if (PaircraftData->GS < (speed)) {
						    brakes.LBrake = 0.0;
						    brakes.RBrake = 0.0;
					    }
				    }
				    else {
					    if (PaircraftData->GS > (speed + 2)) {
						    double brake = (PaircraftData->GS - speed) / 70.0;
						    setThrust(0);
						    brakes.LBrake = brake;
						    brakes.RBrake = brake;
					    }
					    else if (PaircraftData->GS < (speed)) {
						    brakes.LBrake = 0.0;
						    brakes.RBrake = 0.0;
					    }
				    }
				    SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
			    }
			    break;
		    }
        case PARK: {
          break;
        }
        case SHUTDOWN: {
          break;
        }
        case SECURE: {
          break;
        }
        default: {
          break;
        }
		  }
		}
		InTimer = false;
	}
	if (quit==1) {
		KillTimer(NULL, Timer);
		disconnectSafe();
	}
}




void Disconnect()
{
	hr = SimConnect_Close(hSimConnect);
	MainForm::mainForm->ConnectButton->Text = "Disconnected...";
	MainForm::mainForm->Refresh();
	MainForm::mainForm->textBox1->Clear();
	for (int currHeight = MainForm::mainForm->ConnectButton->Size.Height; currHeight < 237; currHeight++) {
		MainForm::mainForm->ConnectButton->Size = System::Drawing::Size(MainForm::mainForm->ConnectButton->Size.Width, currHeight);
		if (currHeight < 231) {
			MainForm::mainForm->textBox1->Size = System::Drawing::Size(MainForm::mainForm->textBox1->Size.Width, 231 - currHeight);
			MainForm::mainForm->textBox1->Location = System::Drawing::Point(MainForm::mainForm->textBox1->Location.X, 249 - (231 - currHeight));
		}
		MainForm::mainForm->Refresh();
		Sleep(1);
	}
	MainForm::mainForm->ConnectButton->ForeColor = System::Windows::Forms::Control::DefaultForeColor;
	MainForm::mainForm->ConnectButton->BackColor = System::Windows::Forms::Control::DefaultBackColor;
	MainForm::mainForm->ConnectButton->FlatAppearance->BorderColor = System::Drawing::Color::Blue;
	MainForm::mainForm->ConnectButton->Text = "Connect to the simulator.";
	MainForm::mainForm->Refresh();
	Connected = FALSE;
}




delegate void d();




void Connect() {
	static int ii = 0;
	ii++;
	CRoute^ Way = gcnew CRoute();
	MainForm::mainForm->ConnectButton->ForeColor = System::Drawing::Color::DarkBlue;
	MainForm::mainForm->ConnectButton->BackColor = System::Drawing::Color::LightBlue;
	MainForm::mainForm->ConnectButton->FlatAppearance->BorderColor = System::Drawing::Color::DarkBlue;
	Thread ^loopThr;
	if ((!Connected)) {
    
		MainForm::mainForm->ConnectButton->Text = "Try connected...";
		MainForm::mainForm->Refresh();
		Sleep(500);
		if (SUCCEEDED(SimConnect_Open(&hSimConnect, "PMDG NGX Test", NULL, 0, 0, 0))) {
      //hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, zeroxyz, zeropbh, "PMDG 737-800NGXu PMDG House Blended Winglets (N738BW | 2019)", zeroxyz, zeropbh, REQ_ATTACH);
      hr = SimConnect_AttachObjectToSimObject(hSimConnect, 0, zeroxyz, zeropbh, "PMDG 737-800NGXu BW S7 Airlines (VP-BND | 2018)", zeroxyz, zeropbh, REQ_ATTACH);
      
      hr = SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
			for (int currHeight = MainForm::mainForm->ConnectButton->Size.Height; currHeight > 27; (currHeight--)) {
				MainForm::mainForm->ConnectButton->Size = System::Drawing::Size(MainForm::mainForm->ConnectButton->Size.Width, currHeight);
				if (currHeight < 231) {
					MainForm::mainForm->textBox1->Size = System::Drawing::Size(MainForm::mainForm->textBox1->Size.Width, 231 - currHeight);
					MainForm::mainForm->textBox1->Location = System::Drawing::Point(MainForm::mainForm->textBox1->Location.X, 249 - (231 - currHeight));
				}
				Sleep(1);
			}
			MainForm::mainForm->ConnectButton->ForeColor = System::Drawing::Color::DarkGreen;
			MainForm::mainForm->ConnectButton->BackColor = System::Drawing::Color::LightGreen;
			MainForm::mainForm->ConnectButton->FlatAppearance->BorderColor = System::Drawing::Color::DarkGreen;
			MainForm::mainForm->ConnectButton->Text = "Connected to Flight Simulator!";
			MainForm::mainForm->Refresh();
			typedef LPVOID(__cdecl *IWEBSIMTEXTURE)();
			HINSTANCE hinstLib = GetModuleHandle("WebSimBrowser.dll");
			IWebSimTexture* pIWebSimTexture;
			if (hinstLib != NULL) {
				IWEBSIMTEXTURE IWebSimTextureProc = (IWEBSIMTEXTURE)GetProcAddress(hinstLib, "IWebSimTexture");
				if (NULL != IWebSimTextureProc) {
					pIWebSimTexture = (IWebSimTexture*)(IWebSimTextureProc)();
					if (pIWebSimTexture != NULL) {
						int m = 0;
					}  
				} 
			}
			hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_DATA_NAME, PMDG_NG3_DATA_ID);
			hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_ADD_DATA_NAME, PMDG_ADD_DATA_ID);
			hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NG3_DATA_DEFINITION, 0, sizeof(PMDG_NG3_Data), 0, 0);
			hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_ADD_DATA_DEFINITION, 0, sizeof(sExportData), 0, 0);
			hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_DATA_ID, DATA_REQUEST, PMDG_NG3_DATA_DEFINITION,
				SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
			hr = SimConnect_RequestClientData(hSimConnect, PMDG_ADD_DATA_ID, ADD_DATA_REQUEST, PMDG_ADD_DATA_DEFINITION,
				SIMCONNECT_CLIENT_DATA_PERIOD_SECOND, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
			PMDG_TEST::Control.Event = 0;
			PMDG_TEST::Control.Parameter = 0;
			hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NG3_CONTROL_NAME, PMDG_NG3_CONTROL_ID);
			hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NG3_CONTROL_DEFINITION, 0, sizeof(PMDG_NG3_Control), 0, 0); 
			hr = SimConnect_RequestClientData(hSimConnect, PMDG_NG3_CONTROL_ID, CONTROL_REQUEST, PMDG_NG3_CONTROL_DEFINITION,
				SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_LOGO_LIGHT_SWITCH, "#69754");		//EVT_OH_LIGHTS_LOGO
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_0, "#76773");		//EVT_OH_LIGHTS_LOGO
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_1, "#76774");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_5, "#76776");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_15, "#76778");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_20, "#76779");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_30, "#76780");
      hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_GEAR_LEVER, "#70087");
      hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_GEAR_OFF, "#74183");
      hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_GEAR_UN, "#74184");
      hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_GEAR, "KEY_GEAR_TOGGLE");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_FLIGHT_DIRECTOR_SWITCH, "#70010");	//EVT_MCP_FD_SWITCH_L
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_HEADING_SELECTOR, "#70022");		//EVT_MCP_HEADING_SELECTOR
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_THROTTLE_SET, "THROTTLE_SET");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_RUDDER_SET, "AXIS_RUDDER_SET");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ELEVATOR_SET, "AXIS_ELEVATOR_SET");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_AILERON_SET, "AXIS_AILERON_SET");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_TOGA, "AUTO_THROTTLE_TO_GA");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN, "#76423");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP, "#76427");
			hr = SimConnect_RequestSystemState(hSimConnect, AIR_PATH_REQUEST, "AircraftLoaded");
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_KEYBOARD_A);
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_KEYBOARD_B);
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_KEYBOARD_C);
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_KEYBOARD_D);
			hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYBOARD, EVENT_KEYBOARD_A);
			hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYBOARD, EVENT_KEYBOARD_B);
			hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYBOARD, EVENT_KEYBOARD_C);
			hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYBOARD, EVENT_KEYBOARD_D);
			hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP_KEYBOARD, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
			hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
			hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+ctrl+a", EVENT_KEYBOARD_A);
			hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+ctrl+b", EVENT_KEYBOARD_B);
			hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+ctrl+c", EVENT_KEYBOARD_C);
			hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+ctrl+d", EVENT_KEYBOARD_D);
			hr = SimConnect_SetInputGroupState(hSimConnect, INPUT0, SIMCONNECT_STATE_ON);
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "INDICATED ALTITUDE", "feet");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "PLANE ALTITUDE", "feet");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "Plane Longitude", "degrees");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "Plane Latitude", "degrees");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "PLANE HEADING DEGREES TRUE", "degrees");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "RUDDER PEDAL POSITION", "position");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "GENERAL ENG THROTTLE LEVER POSITION:1", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "GENERAL ENG THROTTLE LEVER POSITION:2", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "TURB ENG N1:1", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "TURB ENG N1:2", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "GROUND VELOCITY", "Knots");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "AIRSPEED INDICATED", "Knots");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "AIRSPEED TRUE", "Knots");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "DELTA HEADING RATE", "degrees per second");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "SIM ON GROUND", "bool");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "VERTICAL SPEED", "Feet per second");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "PLANE PITCH DEGREES", "degrees");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "PLANE BANK DEGREES", "degrees");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "YOKE Y POSITION", "position"); //ELEVATOR POSITION
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "YOKE X POSITION", "position"); //AILERON POSITION
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "ELEVATOR TRIM POSITION", "degrees");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "AUTOPILOT TAKEOFF POWER ACTIVE", "bool");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "AUTOPILOT FLIGHT DIRECTOR ACTIVE", "bool");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "AUTOPILOT FLIGHT DIRECTOR PITCH", "Radians");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "AUTOPILOT FLIGHT DIRECTOR BANK", "Radians");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "AMBIENT WIND VELOCITY", "Knots");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "AMBIENT WIND DIRECTION", "degrees");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "MAGVAR", "degrees");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "SEA LEVEL PRESSURE", "Inches of mercury");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "KOHLSMAN SETTING HG", "Inches of mercury");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "BAROMETER PRESSURE", "Inches of mercury");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "PLANE ALT ABOVE GROUND", "feet");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "SPOILERS HANDLE POSITION", "position");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "FLAPS HANDLE PERCENT", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_THROTTLE, "GENERAL ENG THROTTLE LEVER POSITION:1", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_THROTTLE, "GENERAL ENG THROTTLE LEVER POSITION:2", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_THRUST, "TURB ENG N1:1", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_THRUST, "TURB ENG N1:2", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEEDS, "GROUND VELOCITY", "Knots");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEEDS, "AIRSPEED INDICATED", "Knots");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEEDS, "SIM ON GROUND", "bool");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEEDS, "AUTOPILOT TAKEOFF POWER ACTIVE", "bool");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEEDS, "BRAKE LEFT POSITION", "position");
      hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEEDS, "BRAKE RIGHT POSITION", "position");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_ELEVATOR, "YOKE Y POSITION", "position"); //ELEVATOR POSITION
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AILERON, "YOKE X POSITION", "position"); //ELEVATOR POSITION
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPOILER, "SPOILERS HANDLE POSITION", "position"); 
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_BRAKE, "BRAKE LEFT POSITION", "position"); 
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_BRAKE, "BRAKE RIGHT POSITION", "position");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEED, "VELOCITY BODY Z", "Feet per second");
			hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_ELEVATOR, EVENT_ELEVATOR_SET);
			hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP_ELEVATOR, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
			hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_THRUST, DEF_THRUST, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);
			hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_SPEEDS, DEF_SPEEDS, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);
			quit = 0;
			Connected = TRUE;
			Timer = SetTimer(NULL, 0, 1, TimerProc);
		}
		else {
			MainForm::mainForm->ConnectButton->ForeColor = System::Drawing::Color::DarkRed;
			MainForm::mainForm->ConnectButton->BackColor = System::Drawing::Color::LightPink;
			MainForm::mainForm->ConnectButton->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			MainForm::mainForm->ConnectButton->Text = "Unable to connect! Please verify the simulator`s run, and try again";
		}
	}
	else {		
		quit = 1;
	}
};
}


	






