namespace PMDG_TEST
{

	void ManPitchWithFD(double NPitch, aircraftData* PaircraftData);
	void ManBankWithFD(double NBank, aircraftData* PaircraftData);
	double ManHeadWithWay(DSHEH * Way);
	double RudWithHead(double Heading);
	int ManVSWithWay(DSHEH * Way);
	double ManVSWithGlide(DSHEH * Way, double GS);
}