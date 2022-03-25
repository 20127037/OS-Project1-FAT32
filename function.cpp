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
	int cluster = start;
	int offset;
	string offsetHEX;
	stringstream ss;
	string* res = new string[1024*2];
	T.convertSectorToString(FAT, res, 1024*2);



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
	for (int i = pos; i < pos + byte; i++)
		resultHex += (char)convertHexToDec(sector[i]);

	return resultHex;
}
string getStatus(string s)
{
	int X = stoi(s);
	bitset<8> builder(X);
	string S = builder.to_string();

	vector<int> flag;
	for (int i = 0; i < 8; i++)
	{
		string bitStat = S.substr(i, 1);
		if (bitStat == "1")
			flag.push_back(7 - i);
	}

	stringstream stat;
	for (int i = 0; i < flag.size(); i++)
	{
		switch (flag[i])
		{
		case 0:
			stat << "Read-Only";
			break;
		case 1:
			stat << "Hidden";
			break;
		case 2:
			stat << "System";
			break;
		case 3:
			stat << "VolLabel";
			break;
		case 4:
			stat << "Directory";
			break;
		case 5:
			stat << "Archive";
			break;
		}
		if (i == flag.size() - 1)
			stat << ".";
		else stat << ", ";
	}
	return stat.str();
}
string convertDectoHex(int dec) {
	int r;
	string hexString = "";
	char hexTab[] = { '0', '1', '2','3','4','5','6','7','8','9','A','B','C','D','E','F' };
	while (dec > 0)
	{
		r = dec % 16;
		hexString = hexTab[r] + hexString;
		dec = dec / 16;
	}
	return hexString;
}
long long int getSize(vector<BYTE> MainEntry) {
	stringstream ss;
	for (int i = 31; i >= 28; i--)
	{
		ss << setw(2) << setfill('0') << convertDectoHex((int)MainEntry[i]);
	}
	string hexString = ss.str();
	ss.str("");
	ss.clear();
	ss << hex << hexString;
	long long int size;
	ss >> size;
	return size;

}
int convertHexCharToInt(char a) {
	if (a >= '0' && a <= '9') return a - 48;
	if (a >= 'A' && a <= 'Z') return a - 55;
	return (a - 87);
}
string convertHextoAscii(string str) {
	stringstream ss;
	for (int i = 0; i < str.length(); i++)
	{
		char a = str[i + 1];
		char b = str[i];

		int x = convertHexCharToInt(a);
		int y = convertHexCharToInt(b);
		ss << (char)((16 * x) + y);
	}
	return ss.str();
}
string getExtraEntry(vector<BYTE> entry) {
	stringstream ss;
	for (int i = 1; i < 32; i++) {
		if (i == 11 or i == 13 or i == 27 or i == 29)
			continue;
		int curE = (int)entry[i];
		if (curE == 0x00)
			continue;
		if (curE == 0xff)
			continue;
		ss << entry[i];
	}
	return ss.str();
}

bool isLastentry(vector<byte> entry) {
	for (int i = 0; i < 32; i++)
	{
		if ((int)entry[0] != 0x00)
			return false;
	}
	return true;
}
int convertHextoDec1(string num) {
	int len = num.length();
	int base = 1;
	int temp = 0;
	for (int i = len - 1; i >= 0; i--) {
		if (num[i] >= '0' && num[i] <= '9') {
			temp += (num[i] - 48) * base;
			base = base * 16;
		}
		else if (num[i] >= 'A' && num[i] <= 'F') {
			temp += (num[i] - 55) * base;
			base = base * 16;
		}
	}
	return temp;
}
int StartCluster(vector<BYTE> entry, vector<string> ClusterList, long long int& beginCluster)
{
	stringstream s1;
	s1 << convertDectoHex((int)entry[21]) << convertDectoHex((int)entry[20]);
	s1 << convertDectoHex((int)entry[27]) << convertDectoHex((int)entry[26]);
	int count = 1;
	beginCluster = convertHextoDec1(s1.str());
	long long int curIndexCluster = beginCluster;
	if (curIndexCluster < 2) return 0;
	while (ClusterList[curIndexCluster] != "0FFFFFFF") {
		curIndexCluster = convertHextoDec1(ClusterList[curIndexCluster]);
		count++;
	}
	return count;
}
vector<string> getClusterList(BYTE* FAT1, int FATsize, int bytePerCluster) {
	int i = 0;
	vector<string> clusterList;
	while (i < FATsize) {
		int j = bytePerCluster;
		stringstream s;
		do {
			s << setw(2) << setfill('0') << convertDectoHex((int)FAT1[i + j - 1]);
			j--;
		} while (j > 0);
		i += 4;
		clusterList.push_back(s.str());
	}
	//printVectorString(clusterList);
	return clusterList;
}
string HandleTxtFile(int sectorPosition, int numberOfSector, LPCWSTR drive, FAT32 T) {
	BYTE contentTable[512];
	ReadSector(drive, sectorPosition * T.getBP(), contentTable, 512);
	int i = 0;
	stringstream s;
	while ((int)contentTable[i] != 0x00) {
		s << contentTable[i];
		i++;
	}
	return s.str();
}
void TabNTimes(int tab) {
	for (int i = 0; i < tab; i++)//tab ra
		cout << "\t"; printf("%c", 200);
}
void HandleSdet(BYTE bangFat1[512], BYTE* SDETtable, int tableSize, FAT32 T, LPCWSTR disk, int tab) {

	int i = 0;
	vector<string> clusterList = getClusterList(bangFat1, 512, 4);

	bool isEndRdetTable = false;
	int indexMainEntry = 0;
	while (i < tableSize) {
		vector<vector<BYTE>> extraEntry;
		vector<BYTE> entryCur;
		bool isExtreEntry = true;
		long long int size;
		string state;
		do {
			int j = 0;
			do {
				entryCur.push_back(SDETtable[i]);
				j++;
				i++;
			} while (j < 32);
			if ((int)entryCur[11] == 0x0f) {
				extraEntry.push_back(entryCur);
				entryCur.clear();
			}
			else {
				isExtreEntry = false;
			}
		} while (isExtreEntry == true);

		if (indexMainEntry < 2) {
			indexMainEntry++;
			continue;
		}
		indexMainEntry++;

		if ((int)entryCur[0] == 0xe5)
			continue;

		if (isLastentry(entryCur))
			break;

		string FileName = "";

		if (extraEntry.size() == 0) {
			stringstream s;
			stringstream FileEnd;
			for (int i = 0; i < 8; i++) {
				s << convertHextoAscii(convertDectoHex((int)entryCur[i]));
			}

			for (int i = 8; i < 11; i++) {
				FileEnd << convertHextoAscii(convertDectoHex((int)entryCur[i]));
			}
			if (FileEnd.str() != "   ") {
				s << ".";
				s << FileEnd.str();
			}
			FileName = s.str();
		}

		while (extraEntry.size() != 0) {
			vector<BYTE> Entry = extraEntry[extraEntry.size() - 1];
			string nameComponent = getExtraEntry(Entry);
			FileName += nameComponent;
			extraEntry.pop_back();
		}

		// tìm cluster bắt đầu
		long long int startClusternum;
		int numCluster = StartCluster(entryCur, clusterList, startClusternum);
		int sectorPosition = T.getSB() + T.getSF() * T.getNF() + 0 + (startClusternum - 2) * T.getSC();



		state = getStatus(to_string((int)entryCur[11]));
		size = getSize(entryCur);

		TabNTimes(tab); cout << "Ten: " << FileName << endl;
		TabNTimes(tab); cout << "Trang Thai: " << state << endl;
		TabNTimes(tab); cout << "Kich Co: " << dec << size << " bytes" << endl;
		

		// nếu là file txt:
		if (FileName[FileName.length() - 4] == '.' && FileName.substr(FileName.length() - 3, 3) == "txt") {
			string content;
			content = HandleTxtFile(sectorPosition, numCluster * T.getSC(), disk, T);
			TabNTimes(tab); cout << "Content: " << content << endl;
			
		}

		// nếu là folder:
		if (entryCur[11] == 0x10) {
			BYTE* SDETtable = new BYTE[numCluster * T.getBP()];
			ReadSector(disk, sectorPosition * T.getBP(), SDETtable, numCluster * T.getBP());
			HandleSdet(bangFat1, SDETtable, numCluster * T.getBP(), T, disk, tab+1);
			delete[] SDETtable;
		}

		cout << endl;

		entryCur.clear();

	}
}

void EntryRdet(vector<BYTE> entry, BYTE bangFat1[512], FAT32 T, LPCWSTR drive) {
	vector<string> clusterList = getClusterList(bangFat1, 512, 4);
	int curStart = 0;
	int curEnd = 32;
	int totalByte = 0;
	while (totalByte < 1024) // entry.size() nhưng do file ko nhiều nên để còn 1024
	{
		vector<vector<BYTE>> extraEntry;
		vector<BYTE> curMainEntry;
		bool isEE = true;
		long long int size;
		string status;
		while (isEE == true)
		{
			vector<BYTE> tempEntry(&entry[curStart], &entry[curEnd]);
			if ((int)tempEntry[11] == 0x0f) {
				extraEntry.push_back(tempEntry);
			}
			else {
				curMainEntry = tempEntry;
				isEE = false;
			}
			curStart += 32;
			curEnd += 32;
			totalByte += 32;
		}

		if ((int)curMainEntry[0] == 0xe5)
			continue;

		if (isLastentry(curMainEntry))
			break;

		size = getSize(curMainEntry);
		status = getStatus(to_string((int)curMainEntry[11]));

		string FileName;
		
		if (extraEntry.size() == 0) {
			stringstream FileEnd;
			stringstream s;
			for (int i = 0; i < 11; i++) {
				if (i < 8)
					s << curMainEntry[i];
				else
					FileEnd << curMainEntry[i];
			}
			if (FileEnd.str() != "   ")
				s << "." << FileEnd.str();
			FileName = s.str();
		}

		while (extraEntry.size() != 0) {
			vector<BYTE> Entry = extraEntry[extraEntry.size() - 1];
			string name = getExtraEntry(Entry);
			FileName += name;
			extraEntry.pop_back();
		}

		// tìm cluster bat dau
		long long int startCluster;
		int numCluster = StartCluster(curMainEntry, clusterList, startCluster);
		int sectorPosition = T.getSB() + T.getSF() * T.getNF() + 0 + (startCluster - 2) * T.getSC();

		cout << "Ten: " << FileName << endl;
		cout << "Trang Thai: " << status << endl;
		cout << "Kich Co:" << dec << size << " bytes" << endl;

		// nếu là file txt:
		if (FileName[FileName.length() - 4] == '.' && FileName.substr(FileName.length() - 3, 3) == "txt") {
			string content;
			content = HandleTxtFile(sectorPosition, numCluster * T.getSC(), drive, T);
			cout << "Content: " << content << endl;
		}

		// nếu là folder:
		if (curMainEntry[11] == 0x10) {
			BYTE* SDETtable = new BYTE[numCluster * T.getBP()];
			ReadSector(drive, sectorPosition * T.getBP(), SDETtable, numCluster * T.getBP());
			HandleSdet(bangFat1, SDETtable, numCluster * T.getBP(), T, drive, 1);
			delete[] SDETtable;
		}
		cout << endl << endl;

	}

}

int readSector(LPCWSTR  drive, LARGE_INTEGER readPoint, BYTE sector[512])
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

	SetFilePointerEx(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

	if (!ReadFile(device, sector, 512, &bytesRead, NULL))
	{
		printf("ReadFile: %u\n", GetLastError());
	}
	else
	{
		printf("\n");
	}
}

