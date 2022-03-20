#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <Windows.h>
using namespace std;

class FAT32 {
private:
	int _SC = 0; // Offset 0D - số byte 1 - Số sector trên cluster 
	int _SB = 0; // Offset 0E - số byte 2 - Số sector thuộc vừng Bootsector 
	int _NF = 0; // Offset 10 - số byte 1 - Số bảng FAT, thường là 2 
	int _SV = 0; // Offset 20 - số byte 4 - Kích thước volume
	int _SF = 0; // Offset 24 - số byte 4 - Kích thước mỗi bảng FAT 
	int _SCOR = 0; // Offset 2C - số byte 4 - Cluster bắt đầu của RDET
	int _ExtraInforSector = 0; // Offset 30 - số byte 2 - Sector chứa thông tin phụ (về cluster trống), thường là 1
	int _BackupBootSector = 0; // Offset 32 - số byte 2 - Sector chứa bản sao lưu của Boot Sector
	string _FAT = ""; // Offset 52 - số byte 8 - Loại FAT, là chuỗi "FAT32"

public:
	void convertSectorToString(BYTE*, string*);
	void displayBootSector(BYTE*);
	void readInfor(string*);
	long int convertHexToDec(string);
	long int littleEndian(string*, string, unsigned int);
	void print();
};

wstring ConvertWStr(string temp);