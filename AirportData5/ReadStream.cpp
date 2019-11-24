#include "stdafx.h"
#include "ReadStream.h"


ReadStream::ReadStream(std::wstring filename)
{
	f.open(filename,std::ios::in|std::ios::binary);
	if (!f.is_open()) 
		std::wcout << "Файл не открыт";
}


ReadStream::~ReadStream()
{
	if (f.is_open())
		f.close();
}


HRESULT ReadStream::Read(PVOID64 buffer, INT64 seek, INT64 count)
{
	if (!f.is_open()) 
		return -1;
	f.seekg(seek, std::ios::beg);
	if (f.eof()) return 0;
	//char *buffer = new char[count];
	try
	{
		f.read((char*)buffer, count);
		int tell = f.tellg();
		if ((tell - seek) != count)
		{
			return -1;
		}
		if (tell < 0x0)
		{
			return -1;
		}
	}
	catch(...)
	{
		std::wcout << "ERROR";
	}
	//return buffer;
	return NOERROR;
}
