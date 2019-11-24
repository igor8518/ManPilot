#include <string>
#include <vector>
namespace PMDG_TEST {

	struct aircraftData
	{
		char title[256];
		double alt;
		double palt;
		double lon;
		double lat;
		double hed;
		double rud;
		double throttle1;
		double throttle2;
		double N1L;
		double N1R;
		double GS;
		double AS;
		double TS;
		double aHed;
		double simOnGround;
		double VS;
		double pitch;
		double bank;
		double elev;
		double ailer;
		double elevTrim;
		double TOGA;
		double FDAct;
		double FDPitch;
		double FDBank;
		double WindSpeed;
		double WindDirection;
		double MagVar;
		double Pressure;
		double PressureSet;
		double AltBaro;
		double GAlt;
		double Spolers;
		double Flaps;

	} AircraftData;
	struct SFlight
	{
		struct SSID
		{
			double altTransit;
		} SID;
		struct SSTAR
		{
			double FLTransit;
		} STAR;
		double cruiseLevel;
	} Flight;
	struct tspeeds
	{
		double GS;
		double AS;
		double simOnGround;
		double TOGA;
		double LBrake;
		double RBrake;
	} speeds;
	struct tspeed
	{
		double GS;
	} push_speed;
	struct sExportData
	{
		double HorBar;
		double VerBar;
		byte HorEnabled;
		byte VerEnabled;
		int Res[20];
	} ExportData;
	struct tthrust
	{
		double N1L;
		double N1R;
	} thrust;
	struct tthrottle
	{
		double L;
		double R;
	} throttle;
	struct tbrakes
	{
		double LBrake;
		double RBrake;
	} brakes;

	std::string * ParkN = NULL;
	std::string ParkType[] = {"", "RAMP_GA", "RAMP_GA_SMALL", "RAMP_GA_MEDIUM", "RAMP_GA_LARGE", "RAMP_CARGO", "RAMP_MIL_CARGO", "RAMP_MIL_COMBAT", "GATE_SMALL", "GATE_MEDIUM", "GATE_HEAVY", "DOCK_GA", "FUEL", "VEHICLES"};
	std::string ParkName[] = { "NONE", "PARKING", "N_PARKING", "NE_PARKING", "E_PARKING", "SE_PARKING", "S_PARKING", "SW_PARKING", "W_PARKING", "NW_PARKING", "GATE", "DOCK",
		"GATE_A", "GATE_B", "GATE_C", "GATE_D", "GATE_E", "GATE_F", "GATE_G", "GATE_H", "GATE_I", "GATE_J", "GATE_K", "GATE_L", "GATE_M", "GATE_N", "GATE_O", "GATE_P", "GATE_Q", "GATE_R", "GATE_S", "GATE_T",
		"GATE_U", "GATE_V", "GATE_W", "GATE_X", "GATE_Y", "GATE_Z" };
	double spoiler;
	double minDCommon = 10000;
	typedef struct
	{
		double iMin, iMax;
		// Maximum and minimum allowable integrator state
		double    pGain,        // integral gain
			iGain,        // proportional gain
			dGain;
		double dState;                  // Last position input
		double iState; // Integrator state
		double val;
		double Term;
		double inVal;
	} SPid;

	struct alterPid
	{
		double Kp;
		double ePn;
		double ePnm1;
		double a;
		double b;
		double g;
		double Ti;
		double Td;
		double eDfn;
		double eDfnm1;
		double eDfnm2;
		long time;
		double uMin;
		double uMax;
		bool FT;
		double Unm1;
	};

	SIMCONNECT_RECV_WEATHER_OBSERVATION *Weather;
	bool Attached = true;
	bool FW = false;
	char* Metar;
	std::string metar;
	struct MetarDecode
	{
		double WindDir;
		double WindSpeed;
	} DestMetar;

	struct GraphPair
	{
		long time;
		double value;
	};
	struct WayPoint
	{
		std::string ICAO;
		WORD Type;
		std::string AirWay;
		double lat;
		double lon;
		double alt;
	};
	SIMCONNECT_DATA_XYZ zeroxyz = { 0, 0, 0 };
	SIMCONNECT_DATA_PBH zeropbh = { 0, 0, 0 };

	std::vector<DWORD>* IDLights = new std::vector<DWORD>();
	bool CabinLightState = false;
	//std::vector<SIMCONNECT_DATA_XYZ> Lightxyz = std::vector<SIMCONNECT_DATA_XYZ>(10);
	SIMCONNECT_DATA_XYZ Lightxyz[10] = {
		{ 1.0, 2.0, -6.0 }, { 1.0, 2.0, -3.0 }, { 1.0, 2.0, 0.0 }, { 1.0, 2.0, 3.0 }, { 1.0, 2.0, 6.0 },
		{ -1.0,2.0,-6.0 }, { -1.0, 2.0, -3.0 }, { -1.0, 2.0, 0.0 }, { -1.0, 2.0, 3.0 }, { -1.0, 2.0, 6.0 }
	};
	int CabLight = 0;
	int CurrCabLight = 0;
	
}