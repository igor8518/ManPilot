#pragma once
#define PMDG_ADD_DATA_NAME			"PMDG_Additional_Data"
#define PMDG_ADD_DATA_ID			0x4E877777
#define PMDG_ADD_DATA_DEFINITION	0x4E877778

static class PMDG_ADD_Data
{
public:
	static double HorBar;
	static double VerBar;
	static byte HorEnabled;
	static byte VerEnabled;
	static int Res[20];
} ExportData;