#pragma once
//#include "stdafx.h"
class ReadStream
{
private:
	//��������� ���� - ����
	std::ifstream f;
public:
	ReadStream(std::wstring filename);
	~ReadStream();
	HRESULT Read(PVOID64 buffer, INT64 seek, INT64 count);
};

