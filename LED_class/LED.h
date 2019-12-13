
#include <gdiplus.h>
#include <gdiplusflat.h>

#include <stdio.h>

#include <tchar.h>
#include <strsafe.h>
#include <string>

#include "simconnect.h"

//#define DEBUG

// Types

#define COUNT unsigned int

using namespace Gdiplus;

// Classes

class Led {
	public:
		// Enums

		enum EVENT_ID {
			KEYBOARD_A,
			E_6HZ
		};

		enum INPUT_ID {
			I_0				// Used to handle key presses
		};

		enum GROUP_ID {
			KEYBOARD,		// Used to handle key presses
			G_0
		};

		// Structures

		#ifdef DEBUG
			class ImpGdip {
				public:
					//INT64 d1[94];
					INT64 d1[94];
					void* (DrawLine1);
					INT64 d2[24];
					void* (GDIStart);
				};
		#endif

		class Data {
			public:
				INT64 d1[278];
				double HorBar;
				//int d2[66];
				INT64 d2[3];
				double VerBar;
		};

		class ExportData {
			public:
				double HorBar;
				double VerBar;
				byte HorEnabled;
				byte VerEnabled;
				int Res[20];
		};

		class MainData {
			public:
				INT64 d1[22];
				int b0;
			};

		// Constructors

		Led();

		// Methods
		#ifdef DEBUG
			static Gdiplus::Status WINGDIPAPI gdipDrawLineLed(GpGraphics* graphics, GpPen* pen, REAL x1, REAL y1, REAL x2, REAL y2);

			bool		getFirstRunStatus();
			void		invertFirstRunStatus();

			ImpGdip*	getImpGdipPoint();
			void		setImpGdipPoint(ImpGdip* pImpGdip);
		#endif

		Data**		getDataPointPoint();

		Data*		getDataPoint();
		void		setDataPoint(Data* pData);

		void		setDataPointPoint(Data** ppData);

		MainData**	getMainDataPointPoint();

		MainData*	getMainDataPoint();
		void		setMainDataPoint(MainData* pMainData);

		void		setMainDataPointPoint(MainData** ppMainData);

		HMODULE*	getPmdgPoint();

		void		setExportData(ExportData exportData);

		COUNT		getCount();
		void		setCount(COUNT iCount);

		bool		setHr(HRESULT hr);

	private:

		// Vars
		HRESULT		hr_;
		#ifdef DEBUG
			ImpGdip* pImpGdip_;
		#endif
		Data*		pData_, ** ppData_;
		ExportData	exportData_;
		MainData	mainData_, * pMainData_, ** ppMainData_;
		HMODULE		pPmdg_;

		// Initialized vars

		COUNT	iCount_ = 0;
		#ifdef DEBUG
			bool	isFirstRun_ = true;


			// Console methods

			void CreateConsole_();
			void DestroyConsole_();
			void PrintOnConsole_(LPCWSTR pTxt);
		#endif	
};

// Initialized global vars

Led*	led			= 0;
HANDLE  hSimConnect = 0;
HANDLE	hOut		= 0;

// Constants

constexpr auto PMDG_ADD_DATA_NAME		= "PMDG_Additional_Data";
constexpr auto PMDG_ADD_DATA_ID			= 0x4E877777;
constexpr auto PMDG_ADD_DATA_DEFINITION = 0x4E877778;

// Global methods

void CALLBACK myDispatchProcDll(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);



