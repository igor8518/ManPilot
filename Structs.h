namespace PMDG_TEST {
	struct aircraftData
	{
		char title[256];
		double alt;
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
		double GAlt;
		double Spolers;

	} AircraftData;

	struct SSTAR 
	{
		double FLTransit;
	} STAR;
	struct tspeeds
	{
		double GS;
		double AS;
		double simOnGround;
		double TOGA;
	} speeds;
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

	double spoiler;

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

	struct GraphPair
	{
		long time;
		double value;
	};
}