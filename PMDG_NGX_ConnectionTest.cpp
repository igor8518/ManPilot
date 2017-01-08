//------------------------------------------------------------------------------
//
//  PMDG 737NGX external connection sample 
// 
//------------------------------------------------------------------------------
#pragma once
#include "PMDG_NGX.h"
namespace PMDG_TEST{
	using namespace System::Threading;






// this variable keeps the state of one of the NGX switches
// NOTE - add these lines to <FSX>\PMDG\PMDG 737 NGX\737NGX_Options.ini: 
//
//[SDK]
//EnableDataBroadcast=1
//
// to enable the data sending from the NGX.

bool NGX_FuelPumpLAftLight = true;
bool NGX_TaxiLightSwitch = false;
bool NGX_LogoLightSwitch = false;
double speed = 10;

// This function is called when NGX data changes
void ProcessNGXData(PMDG_NGX_Data *pS)
{
	// test the data access:
	// get the state of an annunciator light and display it
	
	if (pS->FUEL_annunLOWPRESS_Aft[0] != NGX_FuelPumpLAftLight)
	{
		NGX_FuelPumpLAftLight = pS->FUEL_annunLOWPRESS_Aft[0];
		if (NGX_FuelPumpLAftLight)
			AddText("\nLOW PRESS LIGHT: [ON]\n");

		else
			AddText("\nLOW PRESS LIGHT: [OFF]\n");
	}

	// get the state of switches and save it for later use
	if (pS->LTS_TaxiSw != NGX_TaxiLightSwitch)
	{
		NGX_TaxiLightSwitch = pS->LTS_TaxiSw;
		if (NGX_TaxiLightSwitch)
			AddText("\nTAXI LIGHTS: [ON]\n");
		else
			AddText("\nTAXI LIGHTS: [OFF]\n");
	}

	if (pS->LTS_LogoSw != NGX_LogoLightSwitch)
	{
		NGX_LogoLightSwitch = pS->LTS_LogoSw;
		if (NGX_LogoLightSwitch)
			AddText("\nLOGO LIGHTS: [ON]\n");

		else
			AddText("\nLOGO LIGHTS: [OFF]\n");
	}
}

void toggleTaxiLightSwitch()
{
	// Test the first control method: use the control data area.
	if (AircraftRunning)
	{
		bool New_TaxiLightSwitch = !NGX_TaxiLightSwitch;

		// Send a command only if there is no active command request and previous command has been processed by the NGX
		if (Control.Event == 0)
		{
			Control.Event = EVT_OH_LIGHTS_TAXI;		// = 69749
			if (New_TaxiLightSwitch)
				Control.Parameter = 1;
			else
				Control.Parameter = 0;

			SimConnect_SetClientData(hSimConnect, PMDG_NGX_CONTROL_ID, PMDG_NGX_CONTROL_DEFINITION,
				0, 0, sizeof(PMDG_NGX_Control), &Control);
		}
	}
}

void toggleLogoLightsSwitch()
{
	// Test the second control method: send an event
	// use direct switch position
	bool New_LogoLightSwitch = !NGX_LogoLightSwitch;

	int parameter = 5000;
	//SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_THROTTLE_SET, parameter,
		//SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}

void toggleFlightDirector()
{
	// Test the second control method: send an event
	// use mouse simulation to toggle the switch
	SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_FLIGHT_DIRECTOR_SWITCH, MOUSE_FLAG_LEFTSINGLE,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
	SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_FLIGHT_DIRECTOR_SWITCH, MOUSE_FLAG_LEFTRELEASE,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}

void slewHeadingSelector()
{
	// Test the second control method: send an event
	// use mouse simulation to slew a knob
	SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_HEADING_SELECTOR, MOUSE_FLAG_WHEEL_UP,
		SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
}

void throttleSet(DWORD position)
{
	SetPositionControl((INT16)(position));
}

void throttleLever(int position)
{

	throttle.L = (double)position;
	throttle.R = (double)position;
	SimConnect_SetDataOnSimObject(hSimConnect, DEF_THROTTLE, 0, 0, 0, sizeof(tthrottle), &throttle);
}
void setThrust(FLOAT thrust)
{
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
	if (dThrottle < -5)
	{
		dThrottle = -5;
	}
	else if (dThrottle > 5)
	{
		dThrottle = 5;
	}
	throttle = throttle+dThrottle;
	bool thrustSet = FALSE; 
	//int throttle = (int)((thrust - 21) / (100 - 21) * 90);
	if (throttle < 0)
	{
		throttle = 0;
	}
	else if (throttle > 100)
	{
		throttle = 100;
	}
	throttleLever(throttle);
	thrustSet = TRUE;
	startTime = clock();
	startThrust = PMDG_TEST::thrust.N1L;
}

void setGSpeed(FLOAT speed, FLOAT Tr=0)
{
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
	if (dThrust < -5)
	{
		dThrust = -5;
	}
	else if (dThrust > 5)
	{
		dThrust = 5;
	}
	if ((speedA < -0.3) && (speedA > -0.4))
	{
		dThrust = dThrust + 2;
	}
	else if ((speedA > 0.3) && (speedA < 0.4))
	{
		dThrust = dThrust - 2;
	}
	if (speedA < -0.4)
	{
		dThrust = dThrust+5;
	}
	else if (speedA >= 0.4)
	{
		dThrust = dThrust - 5;
	}
	thrust = PMDG_TEST::thrust.N1L + dThrust;

	bool thrustSet = FALSE;
	if (thrust < 21)
	{
		thrust = 21;
	}
	else if (thrust > 100)
	{
		thrust = 100;
	}
	setThrust(thrust);


	thrustSet = TRUE;
	startTime = clock();
	startSpeed = PMDG_TEST::speeds.GS;
}

void CALLBACK MyDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void *pContext)
{
	switch (pData->dwID)
	{
	case SIMCONNECT_RECV_ID_CLIENT_DATA: // Receive and process the NGX data block
	{
											 SIMCONNECT_RECV_CLIENT_DATA *pObjData = (SIMCONNECT_RECV_CLIENT_DATA*)pData;

											 switch (pObjData->dwRequestID)
											 {
											 case DATA_REQUEST:
											 {
																  PMDG_NGX_Data* ps = (PMDG_NGX_Data*)&pObjData->dwData;
																  NGX = *(PMDG_NGX_Data*)&pObjData->dwData;
																  //ProcessNGXData(&NGX);
																  GetNGXData = TRUE;
																  SIMCONNECT_RECV_EVENT *pObjData = (SIMCONNECT_RECV_EVENT*)pData; 
																  switch (pObjData->uEventID)
																  {
																  case(0):
																  {
																			 break;
																  }
																  default:
																  {
																			 AddText("Data: " + pObjData->uEventID + "\n");
																			 break;
																  }
																  }
																  break;
											 }
											 case CONTROL_REQUEST:
											 {
																	 // keep the present state of Control area to know if the server had received and reset the command
																	 PMDG_NGX_Control *pS = (PMDG_NGX_Control*)&pObjData->dwData;
																	 Control = *pS;
																	 break;
											 }
											 case ADD_DATA_REQUEST:
											 {
																	  sExportData *pS = (sExportData*)&pObjData->dwData;
																	  ExportData.HorBar = pS->HorBar;
																	  ExportData.HorEnabled = pS->HorEnabled;
																	  ExportData.VerBar = pS->VerBar;
																	  ExportData.VerEnabled = pS->VerEnabled;
																	  //PData = ps;
																	  break;
											 }
											 default:
											 {
														AddText("NON Data request and not control request");
														break;
											 }
											 }
											 break;
	}
	case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
	{
											  SIMCONNECT_RECV_SIMOBJECT_DATA *pObjData = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
											  switch (pObjData->dwRequestID)
											  {
											  case REQ_AIRCRAFT_USER:
											  {
																		DWORD ObjectID = pObjData->dwObjectID;
																		aircraftData *ps = (aircraftData*)&pObjData->dwData;
																		PMDG_TEST::AircraftData = *((aircraftData*)&pObjData->dwData);
																		PMDG_TEST::AircraftData.ailer = PMDG_TEST::AircraftData.ailer*4.16666666666666667;
																		//AddText("CLIMB: " + "CLIMB" + "\n");
																		
																		break;
											  }
											  case REQ_THRUST:
											  {
																 PMDG_TEST::thrust = *(tthrust*)&pObjData->dwData;
																 //setThrust(80);
																 break;
											  }
											  case REQ_SPEEDS:
											  {
																 PMDG_TEST::speeds = *(tspeeds*)&pObjData->dwData;
																 tspeeds *ps = (tspeeds*)&pObjData->dwData;
																 if (ps->simOnGround == 1)
																 {
																	 if (mode == TAXI_OUT)
																	 {
																		 setGSpeed(speed);
																	 }
																	 //else if ((mode == TAKE_OFF)&&(ps->TOGA == 0))
																	 //{
																	//	 throttleLever(24);
																	// }
																 }
																 break;
											  }
											  
											  default:
											  {
														 break;
											  }
											  }
											  break;

	}

	case SIMCONNECT_RECV_ID_EVENT:
	{
									 SIMCONNECT_RECV_EVENT *evt = (SIMCONNECT_RECV_EVENT*)pData;

									 switch (evt->uEventID)
									 {
									 case EVENT_ELEVATOR_SET:
									 {
																SetDataPitch = true;
																break;
									 }
									 case EVENT_SIM_START:	// Track aircraft changes
									 {
																HRESULT hr = SimConnect_RequestSystemState(hSimConnect, AIR_PATH_REQUEST, "AircraftLoaded");
																break;
									 }
									 case EVENT_KEYBOARD_A:
									 {
															  toggleTaxiLightSwitch();
															  quit = 1;
															  break;
									 }
									 case EVENT_KEYBOARD_B:
									 {
															  toggleLogoLightsSwitch();
															  break;
									 }
									 case EVENT_KEYBOARD_C:
									 {
															  toggleFlightDirector();
															  break;
									 }
									 case EVENT_KEYBOARD_D:
									 {
															  slewHeadingSelector();
															  break;
									 }
									 case EVENT_THROTTLE_SET:
									 {
																AddText("Value: " + (INT16)evt->dwData + "\n");
																//throttleSet(evt->dwData);
																break;
									 }
									 case EVENT_TEXT:
									 {
																//AddText("Value: " + (INT16)evt->dwData + "\n");
																//throttleSet(evt->dwData);
																break;
									 }
									 default:
									 {
												AddText("EVENT\n");
												break;
									 }
									 }
									 break;
	
	}

	case SIMCONNECT_RECV_ID_SYSTEM_STATE: // Track aircraft changes
	{
											  SIMCONNECT_RECV_SYSTEM_STATE *evt = (SIMCONNECT_RECV_SYSTEM_STATE*)pData;
											  if (evt->dwRequestID == AIR_PATH_REQUEST)
											  {
												  if (strstr(evt->szString, "PMDG 737") != NULL)
												  {
													  AircraftRunning = true;
													  AddText("Select B737\n");
												  }
												  else
												  {
													  AircraftRunning = false;
													  AddText("Select: " + (gcnew System::String(evt->szString))+"\n");
												  }
											  }
											  else
											  {
												  AddText(String::Format("SIMCONNECT_RECV_SYSTEM_STATE: {0}\n", evt->dwRequestID));
											  }
											  break;
	}

	case SIMCONNECT_RECV_ID_QUIT:
	{
									quit = 1;
									break;
	}

	case SIMCONNECT_RECV_ID_EXCEPTION:
	{
										 SIMCONNECT_RECV_EXCEPTION *evt = (SIMCONNECT_RECV_EXCEPTION*)pData;
									//quit = 1;
									break;
	}

	default:
	{	
			   AddText(String::Format("Received: {0}\n", pData->dwID));
		break;
	}
	}
}

void loopThrFunc()
{
	int ii = 0; //Убрать
	while (quit == 0)
	{	
		hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
		SimConnect_CallDispatch(hSimConnect, MyDispatchProc, NULL);
		aircraftData* PaircraftData = &AircraftData;
		switch (mode)
		{
		case PREPARE:
		{
						AddText("MODE: PREPARE" + "\n", true);

						SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_5, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						if (GetNGXData)
						{
							if (AircraftData.simOnGround == TRUE)
							{

								setNGXDataBool(EVT_MCP_FD_SWITCH_L, TRUE, &NGX.MCP_FDSw[0]);
								setNGXDataBool(EVT_MCP_FD_SWITCH_R, TRUE, &NGX.MCP_FDSw[1]);
								setNGXDataBool(EVT_MCP_AT_ARM_SWITCH, TRUE, &NGX.MCP_annunATArm);
								setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 161, &NGX.MCP_IASMach);
								setNGXDataUShort(EVT_MCP_HEADING_SELECTOR, 80, &NGX.MCP_Heading);
								setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, CruiseLevel, &NGX.MCP_Altitude);
								setNGXDataDouble(EVT_CONTROL_STAND_TRIM_WHEEL, 6.44 - 3.9, &AircraftData.elevTrim);
								setNGXDataDouble(EVT_EFIS_CPT_BARO, AircraftData.Pressure, &AircraftData.PressureSet, 0.01, -0.01, TRUE);
								setNGXDataBool(EVT_OH_LIGHTS_LOGO, TRUE, &NGX.LTS_LogoSw);
								setNGXDataBool(EVT_OH_LIGHTS_TAXI, TRUE, &NGX.LTS_TaxiSw);
								setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
								setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, TRUE, &NGX.LTS_RunwayTurnoffSw[0]);
								setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, TRUE, &NGX.LTS_RunwayTurnoffSw[1]);
								setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 0, &NGX.LTS_PositionSw, TRUE);
								setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0]);
								setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1]);
								setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[0]);
								setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[1]);
								SimConnect_TransmitClientEvent(hSimConnect, 0, EVT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
								mode = START;
							}
							else
							{
								CurrentWay = 12;
								mode = CLIMB;
							}
						}

						break;
		}
		case START:
		{
					  AddText("MODE: " + "START" + "\n", true);
					  mode = TAXI_OUT;
					  break;
		}
		case TAXI_OUT:
		{

						 hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
						 aircraftData* PaircraftData = &AircraftData;

						 CurrentPos = *Ways[CurrentWay];
						 CurrentPos.Slla.Latitude = PaircraftData->lat;
						 CurrentPos.Slla.Longitude = PaircraftData->lon;
						 DOrtoKM(&CurrentPos);
						 int icw = CurrentWay;
						 double DCommon = CurrentPos.D;
						 while ((Ways[icw]->typePath != RUNWAY) && (icw < (CountWays - 1)))
						 {
							 DCommon += Ways[icw + 1]->D;
							 icw++;
						 }
						 CHAR s[256];
						 sprintf(s, "MODE: TAXI_OUT Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Total ways: %d", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CountWays);
						 AddText(gcnew String(s), true);
						 /* double EangleRel = CurrentPos.EH - Ways[CurrentWay]->EH;
						  if (EangleRel > 180)
						  {
						  EangleRel = EangleRel - 360;
						  }
						  else if (EangleRel < -180)
						  {
						  EangleRel = EangleRel + 360;
						  }
						  ////////////////////////////////////////////////////////
						  */
						 //Снижение скорости при расстоянии до поворота 500 м.
						 if (CurrentPos.D < 0.5)
						 {
							 speed = 10;
						 }
						 ////////////////////////////////////////////////////////

						 //Расчет начала поворота () в зависимости от скорости и разницы углов текущего и следующего путей
						 double REangle = 0.045;
						 if ((CurrentWay + 1) >= CountWays)
						 {
							 REangle = (Ways[0]->SH - Ways[CurrentWay]->EH);
						 }
						 else
						 {
							 REangle = Ways[CurrentWay + 1]->SH - Ways[CurrentWay]->EH;
						 }
						 if (REangle > 180)
						 {
							 REangle = REangle - 360;
						 }
						 else if (REangle < -180)
						 {
							 REangle = REangle + 360;
						 }
						 REangle = REangle * 5 * PMDG_TEST::speeds.GS / 100000;
						 ////////////////////////////////////////////////////////
						 //Расчет угла отклонения (EangleRel) в точке прибытия от заданного
						 double EangleRel = CurrentPos.EH - Ways[CurrentWay]->EH;
						 if (EangleRel > 180)
						 {
							 EangleRel = EangleRel - 360;
						 }
						 else if (EangleRel < -180)
						 {
							 EangleRel = EangleRel + 360;
						 }
						 //Проверка условия если угол отклонения больше 60 градусов, либо пройдена точка поворота,
						 // то ориентируемся на следующий путь. 
						 if ((EangleRel > 60) || (EangleRel < -60) || (CurrentPos.D < REangle))
						 {
							// CurrentWay = CurrentWay + 1;
							 if ((CurrentWay + 1) < 12)
							 {
						

									 CurrentWay = CurrentWay + 1;
								
							 }
							 else
							 {
								 CurrentWay = 0;
							 }

						 }
						 //Иначе продолжаем удерживать текущий путь.
						 else
						 {
							 double HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
							 if ((((CurrentWay + 2) == CountWays) || (Ways[CurrentWay]->typePath == RUNWAY)) && (CurrentPos.D < 0.1))
							 {
								 setNGXDataBool(EVT_OH_LIGHTS_LOGO, TRUE, &NGX.LTS_LogoSw);
								 setNGXDataBool(EVT_OH_LIGHTS_TAXI, TRUE, &NGX.LTS_TaxiSw);
								 setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
								 setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, TRUE, &NGX.LTS_RunwayTurnoffSw[0]);
								 setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, TRUE, &NGX.LTS_RunwayTurnoffSw[1]);
								 setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, TRUE);
								 setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0]);
								 setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1]);
								 setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, TRUE, &NGX.LTS_LandingLtFixedSw[0]);
								 setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, TRUE, &NGX.LTS_LandingLtFixedSw[1]);
							 }
							 // Если угол доворота (HeadingRel) и угол поправки (EangleRel1) меньше 5 градусов и расстояние до конечной точки 
							 // больше 500 метров, то увеличиваем скорость
							 if ((HeadingRel < 5) && (EangleRel < 5) && (HeadingRel > -5) && (EangleRel > -5) && (CurrentPos.D > 0.5))
							 {
								 if (((CurrentWay +1) != CountWays) && ((Ways[CurrentWay]->typePath == RUNWAY)))
								 {

									 speed = 0;
									 if (PaircraftData->GS < 1)
									 {
										 setNGXDataBool(EVT_MCP_AT_ARM_SWITCH, TRUE, &NGX.MCP_annunATArm);
										 setThrust(40);
										 mode = TAKE_OFF;
									 }
								 }
								 else
								 {
									 speed = 20;
								 }
							 }
							 // иначе уменьшаем
							 else
							 {
								 speed = 10;
							 }

						 }
						 break;
		}
		case TAKE_OFF:
		{
						 static int StartThrottle = 0;
						 //AddText("MODE: " + "TAKE_OFF" + "\n", true);
						 aircraftData* PaircraftData = &AircraftData;
						 hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
						 //Время для расчета ускорений
						 static clock_t sTime = 0; //Начальное время
						 clock_t eTime = clock();
						 //Начальное направление
						 static double sHed;
						 //Начальная скорость
						 static double sSpeed;

						 //Конечное направление
						 double eHed = PaircraftData->hed;
						 //Конечная скорость
						 double eSpeed = PaircraftData->GS;
						 double NPitch = 0;
						 if (ExportData.HorEnabled == 1)
						 {
							 NPitch = -ExportData.HorBar;
						 }
						 else
						 {
							 NPitch = -15;
						 }
						 //Если первый вызов функции, то пропускаем расчет велечин, и устанавливаем начальные значения
						 setNGXDataBool(EVT_OH_LIGHTS_LOGO, TRUE, &NGX.LTS_LogoSw);
						 setNGXDataBool(EVT_OH_LIGHTS_TAXI, TRUE, &NGX.LTS_TaxiSw);
						 setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
						 setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, TRUE, &NGX.LTS_RunwayTurnoffSw[0]);
						 setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, TRUE, &NGX.LTS_RunwayTurnoffSw[1]);
						 setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, TRUE);
						 setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 2, &NGX.LTS_LandingLtRetractableSw[0]);
						 setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 2, &NGX.LTS_LandingLtRetractableSw[1]);
						 setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, TRUE, &NGX.LTS_LandingLtFixedSw[0]);
						 setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, TRUE, &NGX.LTS_LandingLtFixedSw[1]);
						 //Разгон двигателей и включение режима TO/GA
						 if (PaircraftData->N1L > 40)
						 {
							 if (!TOGA)
							 {

								 SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_TOGA, 1, \
									 SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
								 TOGA = TRUE;
							 }
						 }
						 else
						 {
							 throttleLever(StartThrottle);
							 if (SetTimeOff(TAKE_OFF, 3000))
							 {
								 StartThrottle = StartThrottle + 4;
							 }
						 }
						 //////////////////////////////////////////////////////
						 //При V=230 шасси нейтрально
						 if (PaircraftData->AS > 230)
						 {
							 setNGXDataUChar(EVT_GEAR_LEVER, 1, &NGX.MAIN_GearLever);
						 }
						 ////////////////////////////////
						 //При 220<V<230 Убираем закрылки и включаем SPEED либо LVL_CHG
						 else if (PaircraftData->AS > 220)
						 {

							 SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_0, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

							 //setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 240, &NGX.MCP_IASMach);

							 if (NGX.MCP_Altitude > 5000)
							 {
								 //setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
							 }
							 else
							 {
								 //setNGXDataBool(EVT_MCP_SPEED_SWITCH, TRUE, &NGX.MCP_annunSPEED);
							 }
							 if (ExportData.HorEnabled == 1)
							 {
								 NPitch = -ExportData.HorBar;
							 }
							 else
							 {
								 NPitch = -15;
							 }
						 }
						 //////////////////////////////////////
						 //При 190<V<220 закрылки 1
						 else if (PaircraftData->AS > 190)
						 {
							 SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_1, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
							 if (ExportData.HorEnabled == 1)
							 {
								 NPitch = -ExportData.HorBar;
							 }
							 else
							 {
								 NPitch = -15;
							 }
						 }
						 //При V<190 и самолет в воздухе триммируем его
						 if ((PaircraftData->AS < 190) && (PaircraftData->simOnGround == FALSE))
						 {
							 if (ExportData.HorEnabled == 1)
							 {
								 NPitch = -ExportData.HorBar;
							 }
							 else
							 {
								 NPitch = -15;
							 }
							 SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_RUDDER_SET, 0, \
								 SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

						 }
						 //Мы готовы к отрыву
						 if (PaircraftData->AS > 150)
						 {
							 //При положительном наборе убираем шасси и включаем режим N1
							 if ((PaircraftData->VS > 30) && (PaircraftData->simOnGround == FALSE))
							 {

								 setNGXDataUChar(EVT_GEAR_LEVER, 0, &NGX.MAIN_GearLever);
								 //setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 220, &NGX.MCP_IASMach);
								 //setNGXDataBool(EVT_MCP_N1_SWITCH, TRUE, &NGX.MCP_annunN1);
								 //setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
								 // SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ELEVATOR_SET, 0, \
								 								 									 SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
								 setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 240, &NGX.MCP_IASMach);
								 if (NGX.MCP_Altitude > 5000)
								 {
									 //setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
								 }
								 //setNGXDataBool(EVT_MCP_HDG_SEL_SWITCH, TRUE, &NGX.MCP_annunHDG_SEL);
								 //setNGXDataBool(EVT_MCP_CMD_A_SWITCH, TRUE, &NGX.MCP_annunCMD_A);
								 if (NGX.MCP_Altitude > 5000)
								 {
									 //setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
								 }
								 else
								 {
									 //setNGXDataBool(EVT_MCP_SPEED_SWITCH, TRUE, &NGX.MCP_annunSPEED);
								 }
								 TOGA = FALSE;
								 CurrentWay = CurrentWay + 1;
								 PPID.FT = true;
								 mode = CLIMB;
							 }


							 //Если режим LVL_CHG не включен триммируем самолет в ручную
							 if (!NGX.MCP_annunCMD_A)
							 {

								 ManPitchWithFD(NPitch, PaircraftData);

								 if (ExportData.VerEnabled == 1)
								 {
									 ManBankWithFD(-ExportData.VerBar, PaircraftData);
								 }
								 else
								 {
									 ManBankWithFD(0, PaircraftData);
								 }


							 }
						 }
						 //////////////////////////
						 //При 140<V<150 подготавливаем руль высоты
						 else if ((PaircraftData->AS > 140) && (PaircraftData->AS < 150))
						 {
							 SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ELEVATOR_SET, -5000, \
								 SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						 }
						 //Расчет угла отклонения (EangleRel) в точке прибытия от заданного
						 CurrentPos = *Ways[CurrentWay];
						 CurrentPos.Slla.Latitude = PaircraftData->lat;
						 CurrentPos.Slla.Longitude = PaircraftData->lon;
						 DOrtoKM(&CurrentPos);

						 ////////////////////////////////////////////////////////


						 ////////////////////////////////////////////////////////
						 double DCommon = GetDistanceToWayType(GSWAY);
						 double DToChange = CalcToNewWay(false);
						 CHAR s[512];
						 sprintf(s, "MODE: TAKEOFF Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Total ways: %d D for head: %.3f", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CountWays, DToChange);
						 AddText(gcnew String(s), true);

						 //Продолжаем удерживать текущий путь.
						 double HeadingRel = ManHeadWithWay(Ways[CurrentWay]);



					  break;
		}
		case ABORT_TAKE_OFF:
		{
					  break;
		}
		case CIRCLE_TAXI:
		{
					  break;
		}

		case CLIMB:
		{
					  aircraftData* PaircraftData = &AircraftData;
					  hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
					  //AddText("CLIMB: " + "CLIMB" + "\n", true);

					  if (PaircraftData->AS > 230)
					  {
						  setNGXDataUChar(EVT_GEAR_LEVER, 1, &NGX.MAIN_GearLever);
						  if (NGX.MCP_Altitude > 5000)
						  {

							  //setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
						  }
					  }
					  else if (PaircraftData->AS > 218)
					  {
						  if (NGX.MCP_Altitude > 5000)
						  {

							  //setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
						  }
						  //setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 240, &NGX.MCP_IASMach);
						  SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_0, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

					  }
					  else if (PaircraftData->AS > 190)
					  {
						  SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_1, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
					  }
					  if ((PaircraftData->alt > 10000) && (PaircraftData->alt < 11000))
					  {
						  //setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 330, &NGX.MCP_IASMach);
					  }
					  if (PaircraftData->alt < 10000)
					  {
						  setNGXDataBool(EVT_OH_LIGHTS_LOGO, TRUE, &NGX.LTS_LogoSw);
						  setNGXDataBool(EVT_OH_LIGHTS_TAXI, TRUE, &NGX.LTS_TaxiSw);
						  setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
						  setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[0]);
						  setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[1]);
						  setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, TRUE);
						  setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0]);
						  setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1]);
						  setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[0]);
						  setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[1]);
					  }
					  else
					  {
						  setNGXDataBool(EVT_OH_LIGHTS_LOGO, FALSE, &NGX.LTS_LogoSw);
						  setNGXDataBool(EVT_OH_LIGHTS_TAXI, FALSE, &NGX.LTS_TaxiSw);
						  setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
						  setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[0]);
						  setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[1]);
						  setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, TRUE);
						  setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0]);
						  setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1]);
						  setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[0]);
						  setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[1]);
					  }
					  if (PaircraftData->alt > 25000)
					  {
						  //setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 0.79, &NGX.MCP_IASMach);
					  }
					 
					  //Расчет угла отклонения (EangleRel) в точке прибытия от заданного
					  CurrentPos = *Ways[CurrentWay];
					  CurrentPos.Slla.Latitude = PaircraftData->lat;
					  CurrentPos.Slla.Longitude = PaircraftData->lon;
					  DOrtoKM(&CurrentPos);

					  ////////////////////////////////////////////////////////

					  double DCommon = GetDistanceToWayType(GSWAY);
					  double DToChange = CalcToNewWay();
					  /*if (DCommon < 20)
					  {
						  CurrentWay = icw;
					  }*/
					  CHAR s[512];
					  // sprintf(s, "MODE: CLIMB Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Total ways: %d D for head: %.3f", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CountWays, REangle);
					  // AddText(gcnew String(s), true);
					  //Расчет угла отклонения (EangleRel) в точке прибытия от заданного
					  double EangleRel = CurrentPos.EH - Ways[CurrentWay]->EH;
					  if (EangleRel > 180)
					  {
						  EangleRel = EangleRel - 360;
					  }
					  else if (EangleRel < -180)
					  {
						  EangleRel = EangleRel + 360;
					  }

					 
					  //Продолжаем удерживать текущий путь.

					  {
						  if (PaircraftData->alt < STAR.FLTransit)
						  {
							  setNGXDataDouble(EVT_EFIS_CPT_BARO, AircraftData.Pressure, &AircraftData.PressureSet, 0.01, -0.01, TRUE);
						  }
						  if (CurrentPos.typePath != RUNWAY)
						  {

							  int Alt = Ways[CurrentWay]->Ella.Altitude / 100;
							  setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, Alt * 100, &NGX.MCP_Altitude);
							  if (SetTimeOff(5, 5000))
							  {
								  setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
							  }
							  /* int VS = ManVSWithWay(Ways[CurrentWay]);
							   if (VS > 2500)
							   {
							   VS = 2500;
							   }
							   else if (VS < -2500)
							   {
							   VS = -2500;
							   }
							   setNGXDataShort(EVT_MCP_VS_SELECTOR, VS, &NGX.MCP_VertSpeed);*/
							  double HeadingRel;
							  if ((!NGX.MCP_annunCMD_A) && (PaircraftData->simOnGround == 0))
							  {
								  if (ExportData.HorEnabled == 1)
								  {
									  //ManPitchWithFD(-ExportData.HorBar, PaircraftData);
									  if (TestControl == FALSE)
									  {
										  AltPitchWithPos(-ExportData.HorBar);

									  }
									  else
									  {
										  AltPitchWithPos(pidPitch.inVal);
									  }
								  }
								  else
								  {
									  //ManPitchWithFD(-10, PaircraftData);
									  AltPitchWithPos(-10);
								  }

								  if (ExportData.VerEnabled == 1)
								  {
									  //ManBankWithFD(-ExportData.VerBar, PaircraftData);
									  if (TestControl == FALSE)
									  {
										  HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
										  //AltBankWithPos(ExportData.VerBar);
									  }
									  else
									  {
										  AltVSpeedWithPitch(pidVSpeed.inVal);
									  }
								  }
								  else
								  {
									  HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
									  //ManBankWithFD(0, PaircraftData);
									  //AltBankWithPos(0);
								  }



							  }
							  sprintf(s, "MODE: CLIMB Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Name: \"%s\" Total ways: %d D for head: %.3f", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CurrentPos.nameEndPoint, CountWays, REangle);
							  AddText(gcnew String(s), true);

						  }
						  else
						  {
							  mode = LANDING;
						  }
						  if ((PaircraftData->alt + 100) > CruiseLevel)
						  {
							  mode = CRUISE;
						  }
					  }
					  break;
		}
		case CRUISE:
		{
					   aircraftData* PaircraftData = &AircraftData;
					   hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
					   CurrentPos = *Ways[CurrentWay];
					   CurrentPos.Slla.Latitude = PaircraftData->lat;
					   CurrentPos.Slla.Longitude = PaircraftData->lon;
					   DOrtoKM(&CurrentPos);

					   ////////////////////////////////////////////////////////

					   //Расчет начала поворота () в зависимости от скорости и разницы углов текущего и следующего путей
					   double REangle = 0.045;
					   if ((CurrentWay + 1) < CountWays)
					   {
						   REangle = Ways[CurrentWay + 1]->SH - Ways[CurrentWay]->EH;
					   }
					   if (REangle > 180)
					   {
						   REangle = REangle - 360;
					   }
					   else if (REangle < -180)
					   {
						   REangle = REangle + 360;
					   }
					   REangle = -REangle * 5 * PMDG_TEST::speeds.AS / 100000;
					   ////////////////////////////////////////////////////////
					   int icw = CurrentWay;
					   double DCommon = CurrentPos.D;
					   while ((Ways[icw]->typePath != RUNWAY) && (icw < (CountWays - 1)))
					   {
						   DCommon += Ways[icw + 1]->D;
						   icw++;
					   }
					   if (DCommon < 20)
					   {
						   CurrentWay = icw;
					   }
					   if (DCommon < 15)
					   {
						   setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 161, &NGX.MCP_IASMach);
					   }
					   else if (DCommon < 20)
					   {
						   setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 180, &NGX.MCP_IASMach);
					   }
					   else if (DCommon < 25)
					   {
						   setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 200, &NGX.MCP_IASMach);
					   }
					   else if (DCommon < 30)
					   {
						   setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 230, &NGX.MCP_IASMach);
					   }
					   CHAR s[512];
					   // sprintf(s, "MODE: CLIMB Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Total ways: %d D for head: %.3f", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CountWays, REangle);
					   // AddText(gcnew String(s), true);
					   //Расчет угла отклонения (EangleRel) в точке прибытия от заданного
					   double EangleRel = CurrentPos.EH - Ways[CurrentWay]->EH;
					   if (EangleRel > 180)
					   {
						   EangleRel = EangleRel - 360;
					   }
					   else if (EangleRel < -180)
					   {
						   EangleRel = EangleRel + 360;
					   }
					   //Проверка условия если угол отклонения больше 60 градусов, либо пройдена точка поворота,
					   // то ориентируемся на следующий путь. 
					   if ((EangleRel > 60) || (EangleRel < -60) || (CurrentPos.D < REangle))
					   {
						   if ((CurrentWay + 1) < CountWays)
						   {
							   if (PaircraftData->GAlt>200)
							   {

								   CurrentWay = CurrentWay + 1;
							   }
						   }
						   else
						   {
							   CurrentWay = 1;
						   }


					   }
					   //Иначе продолжаем удерживать текущий путь.
					   else //if (PaircraftData->simOnGround!=1)
					   {
						   if (PaircraftData->alt < 4500)
						   {
							   setNGXDataDouble(EVT_EFIS_CPT_BARO, AircraftData.Pressure, &AircraftData.PressureSet, 0.01, -0.01, TRUE);
						   }


						   int Alt = Ways[CurrentWay]->Ella.Altitude / 100;
						   if ((Alt + 200) < CruiseLevel)
						   {
							   mode = DESCENT;
						   }
						   setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, Alt * 100, &NGX.MCP_Altitude);
						   if (SetTimeOff(5, 5000))
						   {
							   setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
						   }

						   double HeadingRel;
						   if ((!NGX.MCP_annunCMD_A) && (PaircraftData->simOnGround == 0))
						   {
							   if (ExportData.HorEnabled == 1)
							   {
								   //ManPitchWithFD(-ExportData.HorBar, PaircraftData);
								   if (TestControl == FALSE)
								   {
									   AltPitchWithPos(-ExportData.HorBar);

								   }
								   else
								   {
									   AltPitchWithPos(pidPitch.inVal);
								   }
							   }
							   else
							   {
								   //ManPitchWithFD(-10, PaircraftData);
								   AltPitchWithPos(-10);
							   }

							   if (ExportData.VerEnabled == 1)
							   {
								   //ManBankWithFD(-ExportData.VerBar, PaircraftData);
								   if (TestControl == FALSE)
								   {
									   HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
									   //AltBankWithPos(ExportData.VerBar);
								   }
								   else
								   {
									   AltVSpeedWithPitch(pidVSpeed.inVal);
								   }
							   }
							   else
							   {
								   HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
								   //ManBankWithFD(0, PaircraftData);
								   //AltBankWithPos(0);
							   }

						   }
						   sprintf(s, "MODE: CRUISE Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Name: \"%s\" Total ways: %d D for head: %.3f", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CurrentPos.nameEndPoint, CountWays, REangle);
						   AddText(gcnew String(s), true);

					   }
						   

					   
					   break;
		}
		case DESCENT:
		{
						aircraftData* PaircraftData = &AircraftData;
						hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
						//AddText("CLIMB: " + "CLIMB" + "\n", true);
						
						if (PaircraftData->AS < 165)
						{

							SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_30, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						}
						else if (PaircraftData->AS < 170)
						{
							SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_20, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						}
						else if (PaircraftData->AS < 175)
						{
							setNGXDataUChar(EVT_GEAR_LEVER, 2, &NGX.MAIN_GearLever);
						}
						else if (PaircraftData->AS < 180)
						{
							SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_15, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						}
						else if (PaircraftData->AS < 200)
						{
							SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_5, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						}
						else if (PaircraftData->AS < 218)
						{
							SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_1, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						}
						else if (PaircraftData->AS < 230)
						{
							setNGXDataUChar(EVT_GEAR_LEVER, 1, &NGX.MAIN_GearLever);
						}

						if ((PaircraftData->alt > 10000) && (PaircraftData->alt < 11000))
						{
							setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 240, &NGX.MCP_IASMach);
						}
						if (PaircraftData->alt < 10000)
						{
							setNGXDataBool(EVT_OH_LIGHTS_LOGO, TRUE, &NGX.LTS_LogoSw);
							setNGXDataBool(EVT_OH_LIGHTS_TAXI, TRUE, &NGX.LTS_TaxiSw);
							setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
							setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[0]);
							setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[1]);
							setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, TRUE);
							setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0]);
							setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1]);
							setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[0]);
							setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[1]);
						}
						else
						{
							setNGXDataBool(EVT_OH_LIGHTS_LOGO, FALSE, &NGX.LTS_LogoSw);
							setNGXDataBool(EVT_OH_LIGHTS_TAXI, FALSE, &NGX.LTS_TaxiSw);
							setNGXDataBool(EVT_OH_LIGHTS_ANT_COL, TRUE, &NGX.LTS_AntiCollisionSw);
							setNGXDataBool(EVT_OH_LIGHTS_L_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[0]);
							setNGXDataBool(EVT_OH_LIGHTS_R_TURNOFF, FALSE, &NGX.LTS_RunwayTurnoffSw[1]);
							setNGXDataUChar(EVT_OH_LIGHTS_POS_STROBE, 2, &NGX.LTS_PositionSw, TRUE);
							setNGXDataUChar(EVT_OH_LIGHTS_L_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[0]);
							setNGXDataUChar(EVT_OH_LIGHTS_R_RETRACT, 0, &NGX.LTS_LandingLtRetractableSw[1]);
							setNGXDataBool(EVT_OH_LIGHTS_L_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[0]);
							setNGXDataBool(EVT_OH_LIGHTS_R_FIXED, FALSE, &NGX.LTS_LandingLtFixedSw[1]);
						}
						if (PaircraftData->alt > 25000)
						{
							//setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 0.79, &NGX.MCP_IASMach);
						}

						//Расчет угла отклонения (EangleRel) в точке прибытия от заданного
						CurrentPos = *Ways[CurrentWay];
						CurrentPos.Slla.Latitude = PaircraftData->lat;
						CurrentPos.Slla.Longitude = PaircraftData->lon;
						DOrtoKM(&CurrentPos);

						////////////////////////////////////////////////////////

						//Расчет начала поворота () в зависимости от скорости и разницы углов текущего и следующего путей
						double REangle = 0.045;
						if ((CurrentWay + 1) < CountWays)
						{
							REangle = Ways[CurrentWay + 1]->SH - Ways[CurrentWay]->EH;
						}
						if (REangle > 180)
						{
							REangle = REangle - 360;
						}
						else if (REangle < -180)
						{
							REangle = REangle + 360;
						}
						REangle = -REangle * 5 * PMDG_TEST::speeds.AS / 100000;
						////////////////////////////////////////////////////////
						int icw = CurrentWay;
						double DCommon = CurrentPos.D;
						while ((Ways[icw]->typePath != RUNWAY) && (icw < (CountWays - 1)))
						{
							DCommon += Ways[icw + 1]->D;
							icw++;
						}
						if (DCommon < 20)
						{
							CurrentWay = icw;
						}
						if (DCommon < 15)
						{
							setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 161, &NGX.MCP_IASMach);
						}
						else if (DCommon < 20)
						{
							setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 180, &NGX.MCP_IASMach);
						}
						else if (DCommon < 25)
						{
							setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 200, &NGX.MCP_IASMach);
						}
						else if (DCommon < 30)
						{
							setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 230, &NGX.MCP_IASMach);
						}
						CHAR s[512];
						// sprintf(s, "MODE: CLIMB Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Total ways: %d D for head: %.3f", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CountWays, REangle);
						// AddText(gcnew String(s), true);
						//Расчет угла отклонения (EangleRel) в точке прибытия от заданного
						double EangleRel = CurrentPos.EH - Ways[CurrentWay]->EH;
						if (EangleRel > 180)
						{
							EangleRel = EangleRel - 360;
						}
						else if (EangleRel < -180)
						{
							EangleRel = EangleRel + 360;
						}
						//Проверка условия если угол отклонения больше 60 градусов, либо пройдена точка поворота,
						// то ориентируемся на следующий путь. 
						if ((EangleRel > 60) || (EangleRel < -60) || (CurrentPos.D < REangle))
						{
							if ((CurrentWay + 1) < CountWays)
							{
								if (PaircraftData->GAlt>200)
								{

									CurrentWay = CurrentWay + 1;
								}
							}
							else
							{
								CurrentWay = 1;
							}


						}
						//Иначе продолжаем удерживать текущий путь.
						else //if (PaircraftData->simOnGround!=1)
						{
							if (PaircraftData->alt < 4500)
							{
								setNGXDataDouble(EVT_EFIS_CPT_BARO, AircraftData.Pressure, &AircraftData.PressureSet, 0.01, -0.01, TRUE);
							}
							if (CurrentPos.typePath != RUNWAY)
							{

								int Alt = Ways[CurrentWay]->Ella.Altitude / 100;
								setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, Alt * 100, &NGX.MCP_Altitude);
								if (SetTimeOff(5, 5000))
								{
									setNGXDataBool(EVT_MCP_LVL_CHG_SWITCH, TRUE, &NGX.MCP_annunLVL_CHG);
								}
								/* int VS = ManVSWithWay(Ways[CurrentWay]);
								if (VS > 2500)
								{
								VS = 2500;
								}
								else if (VS < -2500)
								{
								VS = -2500;
								}
								setNGXDataShort(EVT_MCP_VS_SELECTOR, VS, &NGX.MCP_VertSpeed);*/
								double HeadingRel;
								if ((!NGX.MCP_annunCMD_A) && (PaircraftData->simOnGround == 0))
								{
									if (ExportData.HorEnabled == 1)
									{
										//ManPitchWithFD(-ExportData.HorBar, PaircraftData);
										if (TestControl == FALSE)
										{
											AltPitchWithPos(-ExportData.HorBar);

										}
										else
										{
											AltPitchWithPos(pidPitch.inVal);
										}
									}
									else
									{
										//ManPitchWithFD(-10, PaircraftData);
										AltPitchWithPos(-10);
									}

									if (ExportData.VerEnabled == 1)
									{
										//ManBankWithFD(-ExportData.VerBar, PaircraftData);
										if (TestControl == FALSE)
										{
											HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
											//AltBankWithPos(ExportData.VerBar);
										}
										else
										{
											AltVSpeedWithPitch(pidVSpeed.inVal);
										}
									}
									else
									{
										HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
										//ManBankWithFD(0, PaircraftData);
										//AltBankWithPos(0);
									}



								}
								
								sprintf(s, "MODE: DESCENT Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Name: \"%s\" Total ways: %d D for head: %.3f", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CurrentPos.nameEndPoint, CountWays, REangle);
								AddText(gcnew String(s), true);

							}
							else
							{
								flare = true;
								VSPID.FT = true;
								PPID.FT = true;
								mode = LANDING;
							}

						}
						break;
		}
		case LANDING:
		{
						aircraftData* PaircraftData = &AircraftData;
						hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
						//setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 161, &NGX.MCP_IASMach);

						int Alt = Ways[CurrentWay]->Ella.Altitude / 100;
						setNGXDataUShort(EVT_MCP_ALTITUDE_SELECTOR, 2700, &NGX.MCP_Altitude);
						if (SetTimeOff(5, 5000))
						{
							setNGXDataBool(EVT_MCP_VS_SWITCH, TRUE, &NGX.MCP_annunVS);
						}
						CurrentPos = *Ways[CurrentWay];
						CurrentPos.Slla.Latitude = PaircraftData->lat;
						CurrentPos.Slla.Longitude = PaircraftData->lon;
						DOrtoKM(&CurrentPos);
						int VS = ManVSWithGlide(Ways[CurrentWay], 3);
						if (VS > 1800)
						{
							VS = 1800;
						}
						else if (VS < -1800)
						{
							VS = -1800;
						}
						
						double DCommon = CurrentPos.D;
						
						if (DCommon < 15)
						{
							setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 161, &NGX.MCP_IASMach);
						}
						else if (DCommon < 20)
						{
							setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 180, &NGX.MCP_IASMach);
						}
						else if (DCommon < 25)
						{
							setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 200, &NGX.MCP_IASMach);
						}
						else if (DCommon < 30)
						{
							setNGXDataFloat(EVT_MCP_SPEED_SELECTOR, 230, &NGX.MCP_IASMach);
						}
						if ((PaircraftData->AS < 168))
						{
							SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_30, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						}
						else if ((PaircraftData->AS < 175) && (PaircraftData->AS > 170))
						{
							SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_20, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						}
						
						else if ((PaircraftData->AS < 190) && (PaircraftData->AS > 180))
						{
							SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_15, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						}
						else if ((PaircraftData->AS < 215) && (PaircraftData->AS > 200))
						{
							SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_5, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						}
						else if ((PaircraftData->AS < 222) && (PaircraftData->AS > 218))
						{
							SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_FLAPS_LEVER_1, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
						}
						else if ((PaircraftData->AS < 230) && (PaircraftData->AS > 225))
						{
							setNGXDataUChar(EVT_GEAR_LEVER, 1, &NGX.MAIN_GearLever);
						}
						if ((PaircraftData->AS < 173))
						{
							setNGXDataUChar(EVT_GEAR_LEVER, 2, &NGX.MAIN_GearLever);
						}
						setNGXDataShort(EVT_MCP_VS_SELECTOR, VS, &NGX.MCP_VertSpeed);
						double HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
						if ((!NGX.MCP_annunCMD_A) && (PaircraftData->simOnGround == 0))
						{
							if (ExportData.HorEnabled == 1)
							{
								//ManPitchWithFD(-ExportData.HorBar, PaircraftData);
								if (TestControl == FALSE)
								{
									AltVSpeedWithPitch(VS);

								}
								else
								{
									//AltPitchWithPos(pidPitch.inVal);
									AltVSpeedWithPitch(VS);
								}
							}
							else
							{
								//ManPitchWithFD(-10, PaircraftData);
								AltVSpeedWithPitch(VS);
							}

							if (ExportData.VerEnabled == 1)
							{
								//ManBankWithFD(-ExportData.VerBar, PaircraftData);
								if (TestControl == FALSE)
								{
									HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
									//AltBankWithPos(ExportData.VerBar);
								}
								else
								{
									AltBankWithPos(pidBank.inVal);
								}
							}
							else
							{
								HeadingRel = ManHeadWithWay(Ways[CurrentWay]);
								//ManBankWithFD(0, PaircraftData);
								//AltBankWithPos(0);
							}



						}
						CHAR s[512];
						//double DCommon = CurrentPos.D;

						sprintf(s, "MODE: LANDING Current: %.3f km Total: %.3f km \nCurrent way: %d Type: %d Name: \"%s\" Total ways: %d \nVS: %.d", CurrentPos.D, DCommon, CurrentWay, CurrentPos.typePath, CurrentPos.nameEndPoint, CountWays, VS);
						AddText(gcnew String(s), true);


						if (PaircraftData->GAlt < 200)
						{
							setNGXDataBool(EVT_MCP_AT_ARM_SWITCH, FALSE, &NGX.MCP_annunATArm);
							if ((PaircraftData->AS > 80) && (PaircraftData->simOnGround == 1))
							{
								SimConnect_TransmitClientEvent(hSimConnect, -5000, EVENT_ELEVATOR_SET, 0, \
									SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
								if (SetTimeOff(LANDING, 2000))
								{
									throttleLever(-37);
									
									SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
									SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);


								}
								brakes.LBrake = 0.5;
								brakes.RBrake = 0.5;
							}
							else
							{
								if (PaircraftData->GAlt < 50)
								{
									throttleLever(0);
								}
							}
							if (PaircraftData->GS <= 20)
							{

								SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN, MOUSE_FLAG_LEFTSINGLE, SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

								brakes.LBrake = 0;
								brakes.RBrake = 0;

								SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
								VSPID.FT = true;
								mode = PREPARE;
							}
							else if ((PaircraftData->AS > 20) && (PaircraftData->AS < 160))
							{
								brakes.LBrake = 0.2;
								brakes.RBrake = 0.2;
								SimConnect_SetDataOnSimObject(hSimConnect, DEF_BRAKE, 0, 0, 0, sizeof(brakes), &brakes);
							}

						}
						break;
		}
		case ABORT_LANDING:
		{
							  break;
		}
		case BRAKE:
		{
					  break;
		}
		case TAXI_IN:
		{
						break;
		}
		case PARK:
		{
					 break;
		}
		case SHUTDOWN:
		{
						 break;
		}
		case SECURE:
		{
					   break;
		}
		
		default:
			break;
		}
		Sleep(10);
	}
	disconnectSafe();
}

void Disconnect()
{

	hr = SimConnect_Close(hSimConnect);
	MyForm::mainForm->button1->Text = "Disconnected...";
	MyForm::mainForm->Refresh();
	MyForm::mainForm->textBox1->Clear();
	for (int currHeight = MyForm::mainForm->button1->Size.Height; currHeight < 237; currHeight++)
	{
		MyForm::mainForm->button1->Size = System::Drawing::Size(MyForm::mainForm->button1->Size.Width, currHeight);
		if (currHeight < 231)
		{
			MyForm::mainForm->textBox1->Size = System::Drawing::Size(MyForm::mainForm->textBox1->Size.Width, 231 - currHeight);
			MyForm::mainForm->textBox1->Location = System::Drawing::Point(MyForm::mainForm->textBox1->Location.X, 249 - (231 - currHeight));
		}
		MyForm::mainForm->Refresh();
		Sleep(1);
	}
	MyForm::mainForm->button1->ForeColor = System::Windows::Forms::Control::DefaultForeColor;
	MyForm::mainForm->button1->BackColor = System::Windows::Forms::Control::DefaultBackColor;
	MyForm::mainForm->button1->FlatAppearance->BorderColor = System::Drawing::Color::Blue;
	MyForm::mainForm->button1->Text = "Connect to the simulator.";
	MyForm::mainForm->Refresh();
	Connected = FALSE;

}
delegate void d();
void Connect()
{
	//using std::function;
	static int ii = 0;
	ii++;
	CRoute^ Way = gcnew CRoute();
	DSHEH* WAYS = Way->GetSID("USSS", "dosu1a");
	FillWays();
	
	MyForm::mainForm->button1->ForeColor = System::Drawing::Color::DarkBlue;
	MyForm::mainForm->button1->BackColor = System::Drawing::Color::LightBlue;
	MyForm::mainForm->button1->FlatAppearance->BorderColor = System::Drawing::Color::DarkBlue;
	Thread ^loopThr;
	if (!Connected)
	{
		
		MyForm::mainForm->button1->Text = "Try connected...";
		MyForm::mainForm->Refresh();
		Sleep(500);
		if (SUCCEEDED(SimConnect_Open(&hSimConnect, "PMDG NGX Test", NULL, 0, 0, 0)))
		{
			for (int currHeight = MyForm::mainForm->button1->Size.Height; currHeight > 27; currHeight--)
			{
				MyForm::mainForm->button1->Size = System::Drawing::Size(MyForm::mainForm->button1->Size.Width, currHeight);
				if (currHeight < 231)
				{
					MyForm::mainForm->textBox1->Size = System::Drawing::Size(MyForm::mainForm->textBox1->Size.Width, 231 - currHeight);
					MyForm::mainForm->textBox1->Location = System::Drawing::Point(MyForm::mainForm->textBox1->Location.X, 249 - (231 - currHeight));
				}
				MyForm::mainForm->Refresh();
				//Sleep(1);
			}
			MyForm::mainForm->button1->ForeColor = System::Drawing::Color::DarkGreen;
			MyForm::mainForm->button1->BackColor = System::Drawing::Color::LightGreen;
			MyForm::mainForm->button1->FlatAppearance->BorderColor = System::Drawing::Color::DarkGreen;
			MyForm::mainForm->button1->Text = "Connected to Flight Simulator!";
			MyForm::mainForm->Refresh();
			

			// 1) Set up data connection

			// Associate an ID with the PMDG data area name
			hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NGX_DATA_NAME, PMDG_NGX_DATA_ID);
			hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_ADD_DATA_NAME, PMDG_ADD_DATA_ID);

			// Define the data area structure - this is a required step
			hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NGX_DATA_DEFINITION, 0, sizeof(PMDG_NGX_Data), 0, 0);
			hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_ADD_DATA_DEFINITION, 0, sizeof(sExportData), 0, 0);

			// Sign up for notification of data change.  
			// SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED flag asks for the data to be sent only when some of the data is changed.
			hr = SimConnect_RequestClientData(hSimConnect, PMDG_NGX_DATA_ID, DATA_REQUEST, PMDG_NGX_DATA_DEFINITION,
				SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);

			hr = SimConnect_RequestClientData(hSimConnect, PMDG_ADD_DATA_ID, ADD_DATA_REQUEST, PMDG_ADD_DATA_DEFINITION,
				SIMCONNECT_CLIENT_DATA_PERIOD_SECOND, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);


			// 2) Set up control connection

			// First method: control data area
			PMDG_TEST::Control.Event = 0;
			PMDG_TEST::Control.Parameter = 0;

			// Associate an ID with the PMDG control area name
			hr = SimConnect_MapClientDataNameToID(hSimConnect, PMDG_NGX_CONTROL_NAME, PMDG_NGX_CONTROL_ID);


			// Define the control area structure - this is a required step
			hr = SimConnect_AddToClientDataDefinition(hSimConnect, PMDG_NGX_CONTROL_DEFINITION, 0, sizeof(PMDG_NGX_Control), 0, 0);

			// Sign up for notification of control change.  
			hr = SimConnect_RequestClientData(hSimConnect, PMDG_NGX_CONTROL_ID, CONTROL_REQUEST, PMDG_NGX_CONTROL_DEFINITION,
				SIMCONNECT_CLIENT_DATA_PERIOD_ON_SET, SIMCONNECT_CLIENT_DATA_REQUEST_FLAG_CHANGED, 0, 0, 0);

			// Second method: Create event IDs for controls that we are going to operate
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_LOGO_LIGHT_SWITCH, "#69754");		//EVT_OH_LIGHTS_LOGO
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_0, "#76773");		//EVT_OH_LIGHTS_LOGO
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_1, "#76774");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_5, "#76776");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_15, "#76778");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_20, "#76779");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_30, "#76780");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_FLIGHT_DIRECTOR_SWITCH, "#70010");	//EVT_MCP_FD_SWITCH_L
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_HEADING_SELECTOR, "#70022");		//EVT_MCP_HEADING_SELECTOR
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_THROTTLE_SET, "THROTTLE_SET");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_RUDDER_SET, "RUDDER_SET");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ELEVATOR_SET, "AXIS_ELEVATOR_SET");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_AILERON_SET, "AILERON_SET");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_TOGA, "AUTO_THROTTLE_TO_GA");

			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_DOWN, "#76423");
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_SPEED_BRAKE_LEVER_UP, "#76427");
			
			
			//hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_CONTROL_STAND_FLAPS_LEVER_5, "EVT_CONTROL_STAND_FLAPS_LEVER_5");
			

			// 3) Request current aircraft .air file path
			hr = SimConnect_RequestSystemState(hSimConnect, AIR_PATH_REQUEST, "AircraftLoaded");
			// also request notifications on sim start and aircraft change
			hr = SimConnect_SubscribeToSystemEvent(hSimConnect, EVENT_SIM_START, "SimStart");


			// 4) Assign keyboard shortcuts
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_KEYBOARD_A);
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_KEYBOARD_B);
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_KEYBOARD_C);
			hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_KEYBOARD_D);

			hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYBOARD, EVENT_KEYBOARD_A);
			hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYBOARD, EVENT_KEYBOARD_B);
			hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYBOARD, EVENT_KEYBOARD_C);
			hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_KEYBOARD, EVENT_KEYBOARD_D);
			//hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP0, EVENT_THROTTLE_SET);

			hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP_KEYBOARD, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
			hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP0, SIMCONNECT_GROUP_PRIORITY_HIGHEST);

			hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+ctrl+a", EVENT_KEYBOARD_A);
			hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+ctrl+b", EVENT_KEYBOARD_B);
			hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+ctrl+c", EVENT_KEYBOARD_C);
			hr = SimConnect_MapInputEventToClientEvent(hSimConnect, INPUT0, "shift+ctrl+d", EVENT_KEYBOARD_D);

			hr = SimConnect_SetInputGroupState(hSimConnect, INPUT0, SIMCONNECT_STATE_ON);

			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "Title", NULL, SIMCONNECT_DATATYPE_STRING256);
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "INDICATED ALTITUDE", "feet");
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
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "PLANE ALT ABOVE GROUND", "feet");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AIRCRAFT_USER, "SPOILERS HANDLE POSITION", "position");



			
			

			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_THROTTLE, "GENERAL ENG THROTTLE LEVER POSITION:1", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_THROTTLE, "GENERAL ENG THROTTLE LEVER POSITION:2", "Percent");

			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_THRUST, "TURB ENG N1:1", "Percent");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_THRUST, "TURB ENG N1:2", "Percent");

			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEEDS, "GROUND VELOCITY", "Knots");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEEDS, "AIRSPEED INDICATED", "Knots");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEEDS, "SIM ON GROUND", "bool");
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPEEDS, "AUTOPILOT TAKEOFF POWER ACTIVE", "bool");

			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_ELEVATOR, "YOKE Y POSITION", "position"); //ELEVATOR POSITION
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_AILERON, "YOKE X POSITION", "position"); //ELEVATOR POSITION

			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_SPOILER, "SPOILERS HANDLE POSITION", "position"); 
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_BRAKE, "BRAKE LEFT POSITION", "position"); 
			hr = SimConnect_AddToDataDefinition(hSimConnect, DEF_BRAKE, "BRAKE RIGHT POSITION", "position");

			//hr = SimConnect_MapClientEventToSimEvent(hSimConnect, EVENT_ELEVATOR_SET, "ELEVATOR_SET");
			hr = SimConnect_AddClientEventToNotificationGroup(hSimConnect, GROUP_ELEVATOR, EVENT_ELEVATOR_SET);
			hr = SimConnect_SetNotificationGroupPriority(hSimConnect, GROUP_ELEVATOR, SIMCONNECT_GROUP_PRIORITY_HIGHEST);
			//hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_ELEVATOR, DEF_ELEVATOR, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_NEVER);

			

			//hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);
			hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_THRUST, DEF_THRUST, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);
			hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_SPEEDS, DEF_SPEEDS, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND);

			quit = 0;
			//Sleep(10000);
			//mode = PREPARE;
			// 5) Main loop
			
			ThreadStart ^myThreadDelegate = gcnew ThreadStart(loopThrFunc);
			loopThr = gcnew Thread(myThreadDelegate);
			loopThr->IsBackground = TRUE;
			Connected = TRUE;
			loopThr->Start();

		}
		else
		{
			MyForm::mainForm->button1->ForeColor = System::Drawing::Color::DarkRed;
			MyForm::mainForm->button1->BackColor = System::Drawing::Color::LightPink;
			MyForm::mainForm->button1->FlatAppearance->BorderColor = System::Drawing::Color::DarkRed;
			MyForm::mainForm->button1->Text = "Unable to connect! Please verify the simulator`s run, and try again";

			//printf("\nUnable to connect!\n");
		}
	}
	else
	{
		
		quit = 1;
		//loopThr->Join();
		//disconnect();
	}
};
}


	






