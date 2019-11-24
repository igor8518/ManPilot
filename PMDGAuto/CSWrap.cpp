#include "CSWrap.h"
using namespace Runtime::InteropServices;
using namespace System;
//using namespace std;


namespace CSWrap
{
	const char* CSWrap::Format(String^ s)
	{
		return (const char*)(Marshal::StringToHGlobalAnsi(s).ToPointer());
	}
}
