#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <Windows.h>
#include <bitset>
using namespace std;

int ReadSector(LPCWSTR  drive, int readPoint, BYTE* sector, int bytes);
string ConvertHextoText(string sector[512], string offset, unsigned int byte);
long long int convertHexToDec(string hexa);

class FAT32 {
private:
	int _BP = 0; // Offset B - số byte 2 - Số byte mỗi sector
	int _SC = 0; // Offset 0D - số byte 1 - Số sector trên cluster 
	int _SB = 0; // Offset 0E - số byte 2 - Số sector thuộc vừng Bootsector 
	int _NF = 0; // Offset 10 - số byte 1 - Số bảng FAT, thường là 2 
	long int _SV = 0; // Offset 20 - số byte 4 - Kích thước volume
	long int _SF = 0; // Offset 24 - số byte 4 - Kích thước mỗi bảng FAT 
	int _SCOR = 0; // Offset 2C - số byte 4 - Cluster bắt đầu của RDET
	int _ExtraInforSector = 0; // Offset 30 - số byte 2 - Sector chứa thông tin phụ (về cluster trống), thường là 1
	int _BackupBootSector = 0; // Offset 32 - số byte 2 - Sector chứa bản sao lưu của Boot Sector
	
	long int StartingByteRDET;

public:
	long int getBP();

	long int getSC();
	long int getSB();
	long int getNF();
	long int getSV();
	long int getSF();
	long int getSCOR();
	long int getExtraInforSector();
	long int getBackupBootSector();

	void setStartingByteRDET();
	long int getStartingByteRDET();

	void convertSectorToString(BYTE*, string*, int );
	void displayBootSector(BYTE*);
	void readInfor(string*);
	long int convertHexToDec(string);
	long int littleEndian(string*, string, unsigned int);
	void print();
};

wstring ConvertWStr(string temp);
vector<int> clusterArray(FAT32 T, int start, LPCWSTR drive);

vector<byte> byteArray(FAT32 T, vector<int> clusterArray, LPCWSTR drive);
void EntryRdet(vector<BYTE> entry);
string convertDectoHex(int dec);
long long int getSize(vector<BYTE> MainEntry);
int convertHexCharToInt(char a);
string convertHextoAscii(string str);
string getExtraEntry(vector<BYTE> entry);
void EntryRdet(vector<BYTE> entry);
