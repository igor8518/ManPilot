#pragma once
#include <Windows.h>
class ErrorCatch
{
public:
// Get last error in MessageBox
	static void ShowError(char* Title, HRESULT hr)
	{
		const DWORD cchBuffer = 1024;
		WCHAR lpszBuffer[cchBuffer];
		//FormatMessage(FORMAT_MESSAGE_FROM_HMODULE, LoadLibrary("ntdll.dll"), hr, 0, (LPTSTR)lpszBuffer, cchBuffer, NULL);
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, 0, (LPTSTR)lpszBuffer, cchBuffer, NULL);
		MessageBoxEx(NULL, (LPCSTR)lpszBuffer, Title, MB_ICONHAND | MB_OK, 0);
	}
};

