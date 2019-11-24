#pragma once
class WriteStream
{
private:
	//Приватное поле - файл
	std::ofstream f;
public:
	WriteStream(std::wstring filename);
	~WriteStream();
	HRESULT Write(LPWSTR buffer, int count);
};

