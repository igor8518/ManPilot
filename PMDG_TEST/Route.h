//#include "PMDG_NGX.h"
//#define _HAS_ITERATOR_DEBUGGING 0
#include <Windows.h>
//#include <iostream>
//#include "fstream"
using namespace System::IO;
//using namespace std;
#pragma once
namespace PMDG_TEST
{

	ref class  CRoute
	{
	public:
		CRoute();
		~CRoute();
		SIMMATH::DSHEH *GetSID(char* airport, char* sid);
	protected:
		String^  pathSim;
		String^  fsidstar;
	protected:
		void OpenSSFile(char* airport);
	public:
		array<String^>^ GetSectionByName(String^ section, String^ name);
	};
}

