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
	string _SIG = ""; // Dấu hiệu nhận biết MFT entry
	int _US = 0; // Địa chỉ (offset) của Update sequence.
	int _NF = 0; // Số phần tử của mảng Fixup
	int _LSN = 0; // $LogFile Sequence Number (LSN): mã định danh MFT entry của file log (log record).;
	int _SN = 0; // Sequence Number: cho biết số lần MFT entry này đã được sử dụng lại
	int _RC = 0; // Reference Count: cho biết số thư mục mà tập tin này được hiển thị trong đó
	int _OA = 0; // Địa chỉ (offset) bắt đầu của attribute đầu tiên, trong MFT entry này là byte thứ 56.
	string _Flag = ""; // Flags: giá trị 0x01: MFT entry đã được sử dụng - giá trị 0x02: MFT entry của một thư mục - giá trị 0x04, 0x08: không xác định
	int _UB = 0; // Số byte trong MFT entry đã được sử dụng. Ví dụ, trong trường hợp này đã sử dụng 0x0168 = 360 byte.
	int _SD = 0; // Kích thước vùng đĩa đã được cấp cho MFT entry, Ví dụ: 0x0400 = 1024 byte.
	int _BMS = 0; // Tham chiếu đến MFT entry cơ sở của nó (Base  MFT Record).;
	int _NAID = 0; // Next attribute ID: mã định danh của attribute kế tiếp sẽ được thêm vào MFT entry.0;

	// HEADER ATTRIBUTE
	int _startA = 0; // Offset bắt đầu của Attribute
	int _TID = 0; // Mã loại của attribute (type ID)
	int _SA = 0; // Kích thước của attribute
	int _FlagNonRes = 0; // Cờ báo non-resident
	int _LN = 0; // Chiều dài của tên attribute
	int _ONA = 0; // Vị trí (offset) chứa tên của attribute
	int _VF = 0; // Các cờ báo
	int _AID = 0;  // Định danh của attribute (định danh này là duy nhất trong phạm vi một MFT entry)
	int _SC = 0; // Kích thước phần nội dung của attribute 
	int _OCA = 0; // Nơi bắt đầu (offset) của phần nội dung attribute

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

	int getOA();

	void convertSectorToString(BYTE*, string*);
	void displayBootSector(BYTE*);
	void readInfor(string*);
	long long int convertHexToDec(string);
	long long int littleEndian(string*, string, unsigned int);
	string ConvertHextoText(string*, string, unsigned int);
	void readInforHeaderMFT(string*);
	string ConvertDectoHex(int);
	void readInfoAttribute(string*, int);
	string littleEndianMTF(string*, string, unsigned int);
	void print();
	void printHeaderMFT();
	void printHeaderAttribute();
};

//wstring ConvertWStr(string temp);