#include "stdafx.h"
#include "WriteStream.h"


WriteStream::WriteStream(std::wstring filename)
{
	f.open(filename);
	if (!f.is_open())
		std::cout << "Файл не открыт";
}


WriteStream::~WriteStream()
{
}

HRESULT WriteStream::Write(LPWSTR buffer, int count)
{
	if (!f.is_open())
		return -1;
	try
	{
		f.write((char*)buffer, count);
	}
	catch (...)
	{
		std::wcout << "ERROR";
	}
	//return buffer;
	return NOERROR;
}
