#pragma once
class WriteStream
{
private:
	//��������� ���� - ����
	std::ofstream f;
public:
	WriteStream(std::wstring filename);
	~WriteStream();
	HRESULT Write(LPWSTR buffer, int count);
};

