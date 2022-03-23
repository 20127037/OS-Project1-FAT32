#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <Windows.h>
using namespace std;

class NTFS {
private:
	// BPB
	int _BytesPerSector = 0; // Offset 0B - số byte 2 - Kích thước 1 sector	(Đơn vị Byte) 
	int _SectorsPerCluster = 0; // Offset 0D - số byte 1 - Số sector trong 1 cluster  
	int _MediaDescriptor = 0; // Offset 15 - số byte 1 - Mã xác định loại đĩa 
	int _SectorsPerTrack = 0; // Offset 18 - số byte 2 - Số sector/track
	int _NumberOfHeads = 0; // Offset 1A - số byte 2 - Số mặt đĩa (head hay side)
	int _HiddenSectors = 0; // Offset 1C - số byte 4 - Số sector bắt đầu của ổ đĩa logic 
	long long _TotalsSectors = 0; // Offset 28 - số byte 8 - Số sector của ổ đĩa logic
	long long _LogicalClusterNumberforMFT = 0; // Offset 30 - số byte 8 - Cluster bắt đầu của MFT 
	long long _LogicalClusterNumberforMFTMirror = 0; // Offset 38 - số byte 8 - Cluster bắt đầu của MFT dự phòng (MFTMirror)
	int _ClustersPerFileRecordSegment = 0; // Offset 40 - số byte 1 - Kích thước của một bản ghi trong MFT (MFT entry), đơn vị tính là byte
	int _ClustersPerIndexBuffer = 0; // Offset 44 - số byte 1 - số cluster của Index Buffer 
	//long long _VolumeSerialNumber = 0; // Offset 48 - số byte 8 - số seri của ổ đĩa (volume seria number)

	// HEADER MFT

public:
	/*long int getSC();
	long int getSB();
	long int getNF();
	long int getSV();
	long int getSF();
	long int getSCOR();
	long int getExtraInforSector();
	long int getBackupBootSector();
	string getFAT();

	void setStartingByteRDET();
	long int getStartingByteRDET();*/

	void convertSectorToString(BYTE*, string*);
	void displayBootSector(BYTE*);
	void readInfor(string*);
	long long int convertHexToDec(string);
	long long int littleEndian(string*, string, unsigned int);
	void ConvertHextoText(string*, string, unsigned int);
	void readInforHeaderMFT(string*);
	string ConvertDectoHex(int);
	void readInfoAttribute(string*, string);
	void print();
};

//wstring ConvertWStr(string temp);