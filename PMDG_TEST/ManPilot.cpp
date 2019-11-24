#include "PMDG_NGX.h"
namespace PMDG_TEST
{
	void ManPitchWithFD(double NPitch, aircraftData* PaircraftData)
	{
		static double NewPitch = PaircraftData->elev;
		static int countFails = 0;
		//AddText("elev: " + PaircraftData->elev + "\n");
		//AddText("NewPitch: " + NewPitch + "\n");
		if (countFails >= 5)
		{
			countFails = 0;
			SetDataPitch = true;
		}
		
		if (round(NewPitch * 1000000) == round(PaircraftData->elev*1000000))
		{
			SetDataPitch = true;
		}
		if (SetDataPitch)
		{

			PaircraftData = &AircraftData;
			static clock_t sTime = 0; //Начальное время
			clock_t eTime = clock();
			//Начальный тангаж
			static double sPitch;
			static double sHorBar;
			//Конечный тангаж
			double ePitch = PaircraftData->pitch;
			double eHorBar = NPitch;

			double timeOff = eTime - sTime;
			double pitchOff = ePitch - sPitch;
			double HorBarOff = eHorBar - sHorBar;
			double pitchA = (1000 / timeOff)*pitchOff;
			double HorBarA = (1000 / timeOff)*HorBarOff;
			//TO DO
			double PitchRel = NPitch - PaircraftData->pitch;
			//AddText("NPitch: " + NPitch + "\n");
			if (PitchRel > 180)
			{
				PitchRel = PitchRel - 360;
			}
			else if (PitchRel < -180)
			{
				PitchRel = PitchRel + 360;
			}
			//AddText("PitchRel: " + PitchRel + "\n");
			double pparameter = (PitchRel / 10) + HorBarA*2;
			////////////////////////////////////////////////////////
			//AddText("pparameter: " + pparameter + "\n");
			//AddText("pitchA: " + pitchA + "\n");
			//Расчет необходимого угла руля направления для удержания ускорения
			double elev = PaircraftData->elev * 16383;
			//AddText("elev: " + PaircraftData->elev + "\n");

			double rel = -((pparameter - pitchA) * 500);
			if (rel < -500)
			{
				rel = -500;
			}
			else if (rel > 500)
			{
				rel = 500;
			}
			elev = elev + rel;

			if (elev > 16383)
			{
				elev = 16383;
			}
			else if (elev < -16383)
			{
				elev = -16383;
			}
			//Инвертирование знака
			double intpParameter = elev / 16383;
			//PaircraftData->elev = intpParameter;
			//AddText("intpParameter: " + (intpParameter)+"\n\n");
			//AddText("intpParameter: " + intpParameter + "\n");
			//SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ELEVATOR_SET, intpParameter, \
													//SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			SimConnect_SetDataOnSimObject(hSimConnect, DEF_ELEVATOR, 0, 0, 0, sizeof(intpParameter), &intpParameter);
			sTime = clock();
			sPitch = PaircraftData->pitch;
			sHorBar = NPitch;
			NewPitch = intpParameter;
			SetDataPitch = false;

		}
		else
		{
			countFails++;
		}
	}

	void ManBankWithFD(double NNBank, aircraftData* PaircraftData)
	{
		PaircraftData = &AircraftData;
		
		double NBank = -NNBank;
		if (NBank > 30)
		{
			NBank = 30;
		}
		else if (NBank < -30)
		{
			NBank = -30;
		}
		static double NewBank = PaircraftData->ailer;
		static int countFails = 0;
		AddText("ailer: " + PaircraftData->ailer + "\n");
		AddText("NewBank: " + NewBank + "\n");
		if (countFails >= 5)
		{
			countFails = 0;
			SetDataBank = true;
		}
		if (round(10000*NewBank) == round(10000*PaircraftData->ailer))
		{
			SetDataBank = true;
		}
		if (SetDataBank)
		{

			PaircraftData = &AircraftData;
			static clock_t sTime = 0; //Начальное время
			clock_t eTime = clock();
			//Начальный тангаж
			static double sBank;
			static double sVertBar; //BAR
			//Конечный тангаж
			double eBank = PaircraftData->bank;
			double eVertBar = NNBank; //BAR
			double timeOff = eTime - sTime;
			double bankOff = eBank - sBank;
			double VertBarOff = eVertBar - sVertBar; //BAR
			double VertBarA = (1000 / timeOff)*VertBarOff; //BAR
			
			double bankA = (1000 / timeOff)*bankOff;
			//TO DO
			double BankRel = NBank - (NBank/15) - PaircraftData->bank;
			//AddText("NPitch: " + NPitch + "\n");
			if (BankRel > 180)
			{
				BankRel = BankRel - 360;
			}
			else if (BankRel < -180)
			{
				BankRel = BankRel + 360;
			}
			//AddText("PitchRel: " + PitchRel + "\n");
			double pparameter = (BankRel / 1);// -VertBarA;
			////////////////////////////////////////////////////////
			//AddText("pparameter: " + pparameter + "\n");
			//AddText("pitchA: " + pitchA + "\n");
			//Расчет необходимого угла руля направления для удержания ускорения
			double ailer = PaircraftData->ailer * 16383;
			AddText("ailer: " + ailer + "\n");

			double rel = -((pparameter - bankA) * 300);
			if (rel < -500)
			{
				rel = -500;
			}
			else if (rel > 500)
			{
				rel = 500;
			}
			ailer = ailer + rel;

			if (ailer > (16383 / 4.16666666666666667))
			{
				ailer = 16383 / 4.16666666666666667;
			}
			else if (ailer < (-16383 / 4.16666666666666667))
			{
				ailer = -16383 / 4.16666666666666667;
			}
			//Инвертирование знака
			double intpParameter = ailer / 16383;// / 0.40000001;
			//PaircraftData->elev = intpParameter;
			
			//AddText("intpParameter: " + intpParameter + "\n");
			//SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_ELEVATOR_SET, intpParameter, \
																				//SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);
			SimConnect_SetDataOnSimObject(hSimConnect, DEF_AILERON, 0, 0, 0, sizeof(intpParameter), &intpParameter);
			sTime = clock();
			sBank = PaircraftData->bank;
			sVertBar = NNBank;
			NewBank = intpParameter;
			SetDataBank = false;

		}
		else
		{
			countFails++;
		}
		//AddText("NBank: " + NBank + "\n\n");
	}

	double RudWithHead(double Heading)
	{
		
		hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
		aircraftData* PaircraftData = &AircraftData;
		//Время для расчета ускорений
		static clock_t sTime = 0; //Начальное время
		clock_t eTime = clock();
		//Начальное направление
		static double sHed = PaircraftData->hed;

		
		//Конечное направление
		double eHed = PaircraftData->hed;
		//Поперечное ускорение
		double aHed = (eHed - sHed) / (double)((double)(eTime - sTime) / 1000);
		//Расчет доворота (HeadingRel) до угла поправки ошибки (EangleRel)
		double HeadingRel = Heading - PaircraftData->hed;
		if (HeadingRel > 180)
		{
			HeadingRel = HeadingRel - 360;
		}
		else if (HeadingRel < -180)
		{
			HeadingRel = HeadingRel + 360;
		}

		double parameter = HeadingRel / 4;
		////////////////////////////////////////////////////////
		double rud = PaircraftData->rud * 16383;
		if (PaircraftData->GS > 5)
		{

			rud = rud - (100 * (PaircraftData->aHed - parameter) * (PaircraftData->GS / 5));
		}
		else
		{
			rud = rud - (500.0 * (PaircraftData->aHed - parameter));
		}
		if (rud > 16383)
		{
			rud = 16383;
		}
		else if (rud < -16383)
		{
			rud = -16383;
		}
		int intParameter = -rud;
		////////////////////////////////////////////////////////
		
		SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_RUDDER_SET, intParameter, \
			SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

		sTime = clock();
		sHed = PaircraftData->hed;
		return HeadingRel;
	}

	double MRudWithHead(double Heading)
	{
		hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
		aircraftData* PaircraftData = &AircraftData;
		//Расчет доворота (HeadingRel) до угла поправки ошибки (EangleRel)
		double HeadingRel = Heading - PaircraftData->hed;
		if (HeadingRel > 180)
		{
			HeadingRel = HeadingRel - 360;
		}
		else if (HeadingRel < -180)
		{
			HeadingRel = HeadingRel + 360;
		}

		double parameter = -HeadingRel / 4;
		////////////////////////////////////////////////////////
		double rud = PaircraftData->rud * 16383;
		//rud = rud + parameter;
		rud = rud - (150 * (-PaircraftData->aHed - parameter));
		if (rud > 16383)
		{
			rud = 16383;
		}
		else if (rud < -16383)
		{
			rud = -16383;
		}
		int intParameter = -rud;
		////////////////////////////////////////////////////////

		SimConnect_TransmitClientEvent(hSimConnect, 0, EVENT_RUDDER_SET, intParameter, \
			SIMCONNECT_GROUP_PRIORITY_HIGHEST, SIMCONNECT_EVENT_FLAG_GROUPID_IS_PRIORITY);

		return HeadingRel;
	}

	double BankWithHead(double Heading)
	{

		hr = SimConnect_RequestDataOnSimObject(hSimConnect, REQ_AIRCRAFT_USER, DEF_AIRCRAFT_USER, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_ONCE);
		aircraftData* PaircraftData = &AircraftData;
		//Время для расчета ускорений
		static clock_t sTime = 0; //Начальное время
		clock_t eTime = clock();
		//Начальное направление
		static double sHed = PaircraftData->hed;


		//Конечное направление
		double eHed = PaircraftData->hed;
		//Поперечное ускорение
		double aHed = (eHed - sHed) / (double)((double)(eTime - sTime) / 1000);
		//Расчет доворота (HeadingRel) до угла поправки ошибки (EangleRel)
		double HeadingRel = Heading - PaircraftData->hed;
		if (HeadingRel > 180)
		{
			HeadingRel = HeadingRel - 360;
		}
		else if (HeadingRel < -180)
		{
			HeadingRel = HeadingRel + 360;
		}

		double parameter = HeadingRel / 4;
		////////////////////////////////////////////////////////
		double rud = PaircraftData->bank;
		{
			rud = rud - (5 * (PaircraftData->aHed - parameter));
		}
		if ((HeadingRel > 60) || (HeadingRel < -60))
		{
			if (CurrentPos.typePath == TURNRIGHTDIRECTFIX)
			{
				HeadingRel = GetFixDA(HeadingRel, 1);
			}
			else if (CurrentPos.typePath == TURNLEFTDIRECTFIX)
			{
				HeadingRel = GetFixDA(HeadingRel, -1);
			}
		}
		rud = HeadingRel * 2;
		if (rud > 30)
		{
			rud = 30;
		}
		else if (rud < -30)
		{
			rud = -30;
		}
		
		int intParameter = rud;

		////////////////////////////////////////////////////////

		AltBankWithPos(-intParameter);

		sTime = clock();
		sHed = PaircraftData->hed;
		return HeadingRel;
	}

	double PBHeadWithWay(SIMMATH::DSHEH * Way)
	{
		aircraftData* PaircraftData = &AircraftData;
		double EndHead;

			EndHead = Ways[CurrentWay]->EH;

		//Расчет угла отклонения (EangleRel) в точке прибытия от заданного

		double EangleRel = GetAnglePM180(CurrentPos.EH - EndHead);
		double EangleRel1 = EangleRel;

		////////////////////////////////////////////////////////
		//Расчет расстояния (a) до текущего пути
		double a = GetFixDA(sin(EangleRel*M_PI / 180)* CurrentPos.D, EangleRel);

		////////////////////////////////////////////////////////

		double HeadingRel;

		EangleRel = a * 1000;
		
		EangleRel = AngleLimitS(EangleRel, 60);
		double EangleRel2 = EangleRel;

		////////////////////////////////////////////////////////

		//Расчет необходимого угла для поправки ошибки пути
		EangleRel = GetAnglePM180(EndHead + EangleRel + 180.0);
		CHAR s[256];
		sprintf(s, "EndHead: %.3f CurrentPos.EH: %.3f EangleRel1: %.3f a: %.3f EangleRel2: %.3f\n EangleRel: %.3f\n CurrAngle: %.3f", EndHead, CurrentPos.EH, EangleRel1, a, EangleRel2, EangleRel, GetAnglePM180(PaircraftData->hed));
		AddText(gcnew String(s), true);
		////////////////////////////////////////////////////////
		HeadingRel = MRudWithHead(EangleRel);
		return HeadingRel;

	}

	double ManHeadWithWay(SIMMATH::DSHEH * Way)
	{
		aircraftData* PaircraftData = &AircraftData;
		double EndHead;
		if ((CurrentPos.typePath == FIXHEADING))
		{
			EndHead = Ways[CurrentWay]->EH + PaircraftData->MagVar;
		}
		else if (CurrentPos.typePath == FIX)
		{
			EndHead = CurrentPos.EH;
		}
		else if (CurrentPos.typePath == TURNLEFTDIRECTFIX)
		{
			

			EndHead = CurrentPos.EH;


		}
		else if (CurrentPos.typePath == TURNRIGHTDIRECTFIX)
		{
			

			EndHead = CurrentPos.EH;

		}
		else
		{
			EndHead = Ways[CurrentWay]->EH;
		}

		//Расчет угла отклонения (EangleRel) в точке прибытия от заданного

		double EangleRel = GetAnglePM180(CurrentPos.EH - EndHead);


		////////////////////////////////////////////////////////
		//Расчет расстояния (a) до текущего пути
		double a = GetFixDA(sin(EangleRel*M_PI / 180)* CurrentPos.D,EangleRel);
		
		////////////////////////////////////////////////////////

		double HeadingRel;
		if ((PaircraftData->simOnGround == 1))
		{
			EangleRel = a * 1000;
			if (PaircraftData->GS < 2)
			{
				EangleRel = a * 50000;
			}
			if (PaircraftData->GS < 5)
			{
				EangleRel = a * 10000;
			}
			if (PaircraftData->GS < 7)
			{
				EangleRel = a * 3000;
			}
			
			double EangleRel1 = EangleRel;
			EangleRel = AngleLimitS(EangleRel,60);
			
			////////////////////////////////////////////////////////

			//Расчет необходимого угла для поправки ошибки пути
			EangleRel = GetAnglePM180(EndHead + EangleRel);
			////////////////////////////////////////////////////////
			HeadingRel = RudWithHead(EangleRel);
		}
		else
		{
			if ((CurrentPos.typePath == GSWAY) || (CurrentPos.typePath == RUNWAY))
			{

				EangleRel = a * 500 / CurrentPos.D;
			}
			else
			{
				EangleRel = a * 10;
			}
			double EangleRel1 = EangleRel;
			EangleRel = AngleLimitS(EangleRel, 60);
			////////////////////////////////////////////////////////
			//Расчет угла сноса
			double AngleWind = PaircraftData->WindDirection - CurrentPos.SH;
			double AngleDrift = asin(sin(AngleWind / 180 * M_PI)*PaircraftData->WindSpeed / PaircraftData->TS) * 180 / M_PI;

			//Расчет необходимого угла для поправки ошибки пути
			EangleRel = GetAnglePM180(EndHead + EangleRel + AngleDrift);

			int EangleRel2 = GetAngle360(EangleRel - PaircraftData->MagVar);

			////////////////////////////////////////////////////////
			setNGXHeadShort(EVT_MCP_HEADING_SELECTOR, EangleRel2, &NGX.MCP_Heading);


			HeadingRel = BankWithHead(EangleRel);

		}
		return HeadingRel;

	}
	int ManVSWithWay(SIMMATH::DSHEH * Way)
	{
		aircraftData* PaircraftData = &AircraftData;
		
		double TToNextWay = CurrentPos.D / PaircraftData->GS*1.852;
		double TFromWay = Way->D / PaircraftData->GS*1.852;
		//double VSWay = 
		double CurrentNeedAlt = (((Way->Slla.Altitude - Way->Ella.Altitude) / Way->D)*CurrentPos.D) + Way->Ella.Altitude;
		if ((CurrentNeedAlt > Way->Slla.Altitude) && (CurrentNeedAlt > Way->Ella.Altitude))
		{
			if (Way->Slla.Altitude > Way->Ella.Altitude)
			{
				CurrentNeedAlt = Way->Slla.Altitude;
			}
			else
			{
				CurrentNeedAlt = Way->Ella.Altitude;
			}
		}
		if ((CurrentNeedAlt < Way->Slla.Altitude) && (CurrentNeedAlt < Way->Ella.Altitude))
		{
			if (Way->Slla.Altitude > Way->Ella.Altitude)
			{
				CurrentNeedAlt = Way->Ella.Altitude;
			}
			else
			{
				CurrentNeedAlt = Way->Slla.Altitude;
			}
		}
		if (CurrentNeedAlt < 200)
		{
			CurrentNeedAlt = 200;
		}
		double VSWay = (CurrentNeedAlt - Way->Slla.Altitude)*0.0003048 / TToNextWay*54.681;
		int VS = (VSWay + (CurrentNeedAlt - PaircraftData->alt))/100;
		if (Way->typePath == RUNWAY)
		{
			if (VS > 0)
			{
				VS = 0;
			}
		}
		return VS * 100;
	}
	double ManVSWithGlide(SIMMATH::DSHEH * Way,double GS, double TAlt)
	{
		aircraftData* PaircraftData = &AircraftData;

		double TToNextWay = (CurrentPos.D - Way->D+0.05)/ PaircraftData->GS*1.852;
		double TFromWay = Way->D / PaircraftData->GS*1.852;
		//double VSWay = 
		double CurrentNeedAlt = (((tan(GS / 180 * M_PI))*(CurrentPos.D - Way->D+0.05)) * 3280.84) + Way->Slla.Altitude;

		double VSWay = -(CurrentNeedAlt)*0.0003048 / TToNextWay*54.681;
		int VS = (VSWay + (CurrentNeedAlt - PaircraftData->alt)*10);
		if (Way->typePath == RUNWAY)
		{
			if (VS > 0)
			{
				VS = 0;
			}
		}
		/*if (TAlt > (PaircraftData->alt + 100.0))
		{
			if ((PaircraftData->alt-TAlt) < -VS / 6)
			{
				VS = -(PaircraftData->alt)-TAlt * 6;
			}
		}
		else*/
		{
			/*if (PaircraftData->GAlt < -VS / 6)
			{
				VS = -PaircraftData->GAlt * 6;
			}*/
			if ((PaircraftData->alt - WP->at(WP->size()-1).alt) < -VS / 6)
			{
				VS = -PaircraftData->GAlt * 6;
			}
		}
		/*if (PaircraftData->GAlt < -VS / 6)
		{
			VS = -PaircraftData->GAlt * 6;
		}*/
		/*if (PaircraftData->GAlt < -VS / 6)
		{
			flare = false;
		}
		if (!flare)
		{
			VS = 0;
		}*/
		/*if (VS <= 1000)
		{
			int VS2 = VS / 50;
			return VS2 * 50;
		}
		else
		{
			int VS2 = VS / 100;
			return VS2 * 100;
		}*/
		return VS;

	}
	double ManVSWithAngle(double GS)
	{
		aircraftData* PaircraftData = &AircraftData;

		double TToTarget = (CurrentPos.DistToAlt) / PaircraftData->GS/1.852;

		double CurrentNeedAlt = tan(GS/180.0*M_PI)*CurrentPos.DistToAlt * 3280.84 + CurrentPos.TargetAlt;

		double VSWay = -(CurrentNeedAlt - CurrentPos.TargetAlt)/**0.0003048*/ / TToTarget/60/**54.681*/;
		int VS = (VSWay);// +(CurrentNeedAlt - PaircraftData->alt) * 10);

		if (VS > 0)
		{
			VS = 0;
		}
		
		return VS;

	}
	double ManVSWithDescent(SIMMATH::DSHEH * Way)
	{
		aircraftData* PaircraftData = &AircraftData;
		double GS = atan(CurrentPos.D / (PaircraftData->alt - CurrentPos.Ella.Altitude) / 3280.84) * 180 / M_PI;
		double TToNextWay = (CurrentPos.D) / PaircraftData->GS*1.852;
		//double TFromWay = Way->D / PaircraftData->GS*1.852;
		//double VSWay = 
		double CurrentNeedAlt = (((tan(GS / 180 * M_PI))*(CurrentPos.D)) * 3280.84) + (PaircraftData->alt);

		double VSWay = -(CurrentNeedAlt)*0.0003048 / TToNextWay*54.681;
		int VS = (VSWay + (CurrentNeedAlt - PaircraftData->alt) * 10);

		if (VS > 0)
		{
			VS = 0;
		}


		return VS;
	}
	double GetDescentAngle(SIMMATH::DSHEH * Way[])
	{
		aircraftData* PaircraftData = &AircraftData;
		double MaxAlt = CruiseLevel;
		int wi = CurrentWay;
		while ((Way[wi]->fixAlt != CRUISEALT) && (wi < (CountWays - 1)))
		{
			wi++;
		}
		//Ищем минимальный эшелон
		for (int i = wi-1; i >= CurrentWay; i--)
		{
			if ((MaxAlt > Way[i]->Ella.Altitude) && ((Way[i]->fixAlt == ALTORBELOW) || (Way[i]->fixAlt == FIXALT)))
			{
				MaxAlt = Way[i]->Ella.Altitude;
			}
		}
		Flight.cruiseLevel = (int(MaxAlt / 100.0)) * 100;
		//Получаеи точку с фиксированной высотой
		wi = CurrentWay;
		while (((Way[wi]->fixAlt != FIXALT)) && (wi < (CountWays-1)))
		{
			wi++;
		}

		if (wi == CountWays)
		{
			//Точка не найдена
			return -1.0;
		}
		double angle = 0.0;
		double doubleAlt = 0.0;
		int fi = wi;
		//Находим наименьший угол снижения
		for (fi = wi; fi >= CurrentWay; fi--)
		{
			
			double D = 0.0;
			int icw;
			for (int icw = fi; icw > CurrentWay; icw--)
			{
				D += Ways[icw]->D;
			}
			D += CurrentPos.D;
			double Tangle = atan((PaircraftData->alt - Way[fi]->Ella.Altitude) / 3280.84 / D) * 180.0 / M_PI;

			if (doubleAlt < Ways[fi]->Ella.Altitude)
			{
				double Alt = Ways[fi]->Ella.Altitude;
			}
			switch (Ways[fi]->fixAlt)
			{
			case FIXALT:
			{
						   angle = Tangle;
						   doubleAlt = Ways[fi]->Ella.Altitude;
						   //Flight.cruiseLevel = (int(doubleAlt/100.0))*100;
						   CurrentPos.DistToAlt = D;
						   break;
			}
			case ALTORBELOW:
			{
							   if (angle < Tangle)
							   {
								  
								   angle = Tangle;
								   if (doubleAlt > Ways[fi]->Ella.Altitude)
								   {
									   doubleAlt = Ways[fi]->Ella.Altitude;
									   //Flight.cruiseLevel = (int(doubleAlt / 100.0)) * 100;
									   CurrentPos.DistToAlt = D;
								   }
							   }
							   
							   break;
			}
			case ALTORABOVE:
			{
							   if (angle > Tangle)
							   {
								   
								   angle = Tangle;
								   if (doubleAlt < Ways[fi]->Ella.Altitude)
								   {
									   doubleAlt = Ways[fi]->Ella.Altitude;
									   CurrentPos.DistToAlt = D;
								   }
							   }
							   break;
			}
			default:
				break;
			}
		}
		CurrentPos.TargetAlt = doubleAlt;
		Flight.cruiseLevel = (int(MaxAlt / 100.0)) * 100;
		return angle;	
	}
	double GetDescentAngleNew(SIMMATH::DSHEH * Way[])
	{
		aircraftData* PaircraftData = &AircraftData;
		//Получаем ближайший эшелон
		return 0.0;
			
		
	}
}

