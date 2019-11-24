
#include <gdiplus.h>
#include <gdiplusflat.h>
using namespace Gdiplus;
//#define REAL float
#define COUNT unsigned int
//Status WINGDIAPI GdipDrawLineLED(Graphics *graphics, const Pen *pen, REAL x1, REAL y1, REAL x2, REAL y2);
struct impGDIP
{
	//INT64 d1[94];
	INT64 d1[94];
	void *(DrawLine1);	
	INT64 d2[24];
	void *(GDIStart);
} *PimpGDIP;

struct Data
{
	INT64 d1[278];
	double HorBar;
	//int d2[66];
	INT64 d2[3];
	double VerBar;
} * PData, **PPData; 

struct sExportData
{
	double HorBar;
	double VerBar;
	byte HorEnabled;
	byte VerEnabled;
	int Res[20];
} ExportData;

struct sMainData
{
	INT64 d1[22];
	int b0;
} MainData, *PMainData, **PPMainData;


LPWSTR s;
HRESULT hr;
HANDLE  hSimConnect = NULL;
HANDLE hOut = NULL;
HMODULE PPMDG;

#define PMDG_ADD_DATA_NAME			"PMDG_Additional_Data"
#define PMDG_ADD_DATA_ID			0x4E877777
#define PMDG_ADD_DATA_DEFINITION	0x4E877778

COUNT i = 0;


static enum EVENT_ID {
	EVENT_KEYBOARD_A,
	EVENT_6HZ
};

static enum INPUT_ID {
	INPUT0			// used to handle key presses
};

static enum GROUP_ID {
	GROUP_KEYBOARD,		// used to handle key presses
	GROUP0
};

void CreateConsole()
{
	if (hOut == NULL)
	{
		AllocConsole();
		hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	}

}
void DestroyConsole()
{
	if (hOut != NULL)
	{
		FreeConsole();
		hOut = NULL;
	}

}
void PrintOnConsole(LPCWSTR pTxt)
{
	DWORD NumberOfCharsWritten;
	WriteConsole(hOut, pTxt, lstrlen(pTxt), &NumberOfCharsWritten, NULL);
}
