#include "FAT32.h"

int ReadSector(LPCWSTR  drive, int readPoint, BYTE* sector, int bytes)
{
	int retCode = 0;
	DWORD bytesRead;
	HANDLE device = NULL;

	device = CreateFile(drive,    // Drive to open
		GENERIC_READ,           // Access mode
		FILE_SHARE_READ | FILE_SHARE_WRITE,        // Share Mode
		NULL,                   // Security Descriptor
		OPEN_EXISTING,          // How to create
		0,                      // File attributes
		NULL);                  // Handle to template

	if (device == INVALID_HANDLE_VALUE) // Open Error
	{
		printf("CreateFile: %u\n", GetLastError());
		return 1;
	}

	SetFilePointer(device, readPoint, NULL, FILE_BEGIN); // Set a Point to Read

	if (!ReadFile(device, sector, bytes, &bytesRead, NULL))
	{
		printf("ReadFile: %u\n", GetLastError());
	}
	else
	{
		printf("Success!\n");
	}
}


wstring ConvertWStr(string temp)
{
	int strlength = temp.length() + 1;
	int wstrlength = MultiByteToWideChar(CP_ACP, 0, temp.c_str(), strlength, NULL, 0);
	wchar_t* buffer = new wchar_t[wstrlength];
	MultiByteToWideChar(CP_ACP, 0, temp.c_str(), strlength, buffer, wstrlength);
	wstring res(buffer);
	delete[] buffer;

	return res;
}
vector<int> clusterArray(FAT32 T, int start, LPCWSTR drive) {//cluser
	vector<int> result;
	if (start < 2)
		return result;

	// tính số byte của bảng FAT
	int bytesFat = T.getSF() * T.getBP();
	// tính offset đầu tiên của bảng FAT
	int startOffset = T.getSB() * T.getBP();
	FAT32 temp;
	BYTE* FAT = new BYTE[bytesFat];
	ReadSector(drive, startOffset, FAT, bytesFat);
	temp.displayBootSector(FAT);
	int cluster = start;
	int offset;
	string offsetHEX;
	stringstream ss;
	string* res = new string[bytesFat];
	T.convertSectorToString(FAT, res, bytesFat);



	while (cluster != 268435455)
	{
		result.push_back(cluster);
		offset = cluster * 4;

		ss.clear();
		ss << hex << uppercase << offset;
		ss >> offsetHEX;
		cluster = temp.littleEndian(res, offsetHEX, 4);
	}
	return result;
}
//void ReadEntries(int start, int tab, )

vector<byte> byteArray(FAT32 T, vector<int> clusterArray, LPCWSTR drive)
{
	vector<BYTE> ByteArray;
	//Duyệt mảng các cluster
	for (int i = 0; i < clusterArray.size(); i++)
	{
		int offsetStart = (T.getSB() + T.getNF() * T.getSF() + (clusterArray[i] - 2) * T.getSC()) * T.getBP();
	
		int sizeCluster = T.getBP() * T.getSC();
		BYTE* cluster = new BYTE[sizeCluster];

		ReadSector(drive, offsetStart, cluster, sizeCluster);

		for (int j = 0; j < sizeCluster; j++)
		{
			ByteArray.push_back(cluster[j]);
		}
	}
	return ByteArray;
}
void EntryRdet(vector<BYTE> entry) {
	cout << endl;
	for (int i = 0; i < entry.size(); i++)
	{
		if (i % 16 == 0)
		{
			cout << endl;
			cout << entry[i] << " ";
		}
		else cout << entry[i] << " ";
	}
}
long long int convertHexToDec(string hexa) 
{
	long long int result = 0;
	result = stoi(hexa, nullptr, 16);

	return result;
}

string ConvertHextoText(string sector[512], string offset, unsigned int byte)
{
	int pos = convertHexToDec(offset);
	string resultHex = "";
	for (int i = pos; i < byte; i++)
		resultHex += (char)convertHexToDec(sector[i]);

	return resultHex;
}