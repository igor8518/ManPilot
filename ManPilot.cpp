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
		if (PaircraftData->GS > 15)
		{

			rud = rud - (50 * (PaircraftData->aHed - parameter)*(PaircraftData->GS) / 10);
		}
		else
		{
			rud = rud - (50 * (PaircraftData->aHed - parameter));
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

	double ManHeadWithWay(DSHEH * Way)
	{
		aircraftData* PaircraftData = &AircraftData;

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
		////////////////////////////////////////////////////////
		//Расчет расстояния (a) до текущего пути
		double a = sin(EangleRel*M_PI / 180)* CurrentPos.D;
		if ((EangleRel < 0) && (a > 0))
		{
			a = -a;
		}
		if ((EangleRel > 0) && (a < 0))
		{
			a = -a;
		}
		////////////////////////////////////////////////////////

		
		double HeadingRel;
		if ((PaircraftData->simOnGround == 1))
		{
			//Расчет угла поправки в зависимости от ошибки пути, но не больше 60 градусов для EangleRel и
			//общего для EangleRel1
			//Вариант 1
			//EangleRel = (60 * a) / 0.07;
			//Вариант 2
			EangleRel = a * 1000;
			double EangleRel1 = EangleRel;
			if (EangleRel > 60)
			{
				EangleRel = 60;
			}
			else if (EangleRel < -60)
			{
				EangleRel = -60;
			}
			////////////////////////////////////////////////////////

			//Расчет необходимого угла для поправки ошибки пути
			EangleRel = Ways[CurrentWay]->EH+ EangleRel;
			if (EangleRel > 180)
			{
				EangleRel = EangleRel - 360;
			}
			else if (EangleRel < -180)
			{
				EangleRel = EangleRel + 360;
			}
			////////////////////////////////////////////////////////
			HeadingRel = RudWithHead(EangleRel);
		}
		else
		{
			//Расчет угла поправки в зависимости от ошибки пути, но не больше 60 градусов для EangleRel и
			//общего для EangleRel1
			//Вариант 1
			//EangleRel = (60 * a) / 0.07;
			//Вариант 2
			if ((CurrentPos.typePath == GSWAY) || (CurrentPos.typePath == RUNWAY))
			{

				EangleRel = a * 500 / CurrentPos.D;
			}
			else
			{
				EangleRel = a * 10;
			}
			double EangleRel1 = EangleRel;
			if (EangleRel > 60)
			{
				EangleRel = 60;
			}
			else if (EangleRel < -60)
			{
				EangleRel = -60;
			}
			////////////////////////////////////////////////////////
			//Расчет угла сноса
			double AngleWind = PaircraftData->WindDirection - CurrentPos.SH;
			double AngleDrift = asin(sin(AngleWind/180*M_PI)*PaircraftData->WindSpeed / PaircraftData->TS) * 180 / M_PI;
			if (AngleWind < 0)
			{
				//AngleDrift = -AngleDrift;
			}
			//Расчет необходимого угла для поправки ошибки пути
			EangleRel = Ways[CurrentWay]->EH + EangleRel + AngleDrift;
			if (EangleRel > 180)
			{
				EangleRel = EangleRel - 360;
			}
			else if (EangleRel < -180)
			{
				EangleRel = EangleRel + 360;
			}

			int EangleRel2 = EangleRel - PaircraftData->MagVar;

			while (EangleRel2 < 0)
			{
				EangleRel2 = EangleRel2 + 360;
			}
			while (EangleRel2 >= 360)
			{
				EangleRel2 = EangleRel2 - 360;
			}
			////////////////////////////////////////////////////////
			setNGXHeadShort(EVT_MCP_HEADING_SELECTOR, EangleRel2, &NGX.MCP_Heading);
			HeadingRel = BankWithHead(EangleRel);
		}
		return HeadingRel;

	}
	int ManVSWithWay(DSHEH * Way)
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
	double ManVSWithGlide(DSHEH * Way,double GS)
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
		if (PaircraftData->GAlt < -VS / 6)
		{
			VS = -PaircraftData->GAlt*6;
		}
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
}

