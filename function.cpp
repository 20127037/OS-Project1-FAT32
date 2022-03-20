#include "main.cpp"

wstring ConvertWStr(string temp)
{
	int strlength = temp.length() + 1;
	int wstrlength = MultiByteToWideChar(CP_ACP, 0, temp.c_str(), strlength, null, 0);
	wchar_t* buffer = new wchar_t[wstrlength];
	MultiByteToWideChar(CP_ACP, 0, temp.c_str(), strlength, buffer, wstrlength);
	wstring res(buffer);
	delete[] buffer;

	return res;
}