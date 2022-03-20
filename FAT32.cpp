﻿#include "FAT32.h"

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

string FAT32::convertSectorToString(BYTE sector[512]) {
    stringstream ss;
    for (int i = 0; i < 512; i++) {
        ss << hex << int(sector[i]);
    }

    string result = ss.str();

    return result;
}

void FAT32::displayBootSector(BYTE sector[512]) {
    cout << "Offset" << setw(9) << setfill(' ') << '0';
    for (int i = 0; i < 15; i++)
    {
        if ((i + 1) % 8 == 0) cout << "   ";
        cout << setw(3) << setfill(' ') << hex << i + 1;
    }
    for (int i = 0; i < 512; i++) {
        if (i % 8 == 0 and i % 16 != 0) cout << "   ";
        if (i % 16 == 0) {
            cout << endl;
            cout << setw(2) << setfill('0') << i / 16 << '0' << setw(10) << setfill(' ') << ' ';
            cout << setw(2) << setfill('0') << hex << int(sector[i]) << " ";
        }
        else cout << setw(2) << setfill('0') << hex << int(sector[i]) << " ";
    }
}

void FAT32::readInfor(string sector[512]) {
    _SC = littleEndian(sector, "0D", 1);
    _SB = littleEndian(sector, "0E", 2);
    _NF = littleEndian(sector, "10", 1);
    _SV = littleEndian(sector, "20", 4);
    _SF = littleEndian(sector, "24", 4);
    _SCOR = littleEndian(sector, "2C", 4);
    _ExtraInforSector = littleEndian(sector, "30", 2);
    _BackupBootSector = littleEndian(sector, "32", 2);
    _FAT = littleEndian(sector, "15", 1);
}

void FAT32::print() {
    cout << "Số sector trên cluster: " << _SC << endl;
    cout << "Số sector thuộc vùng Bootsector: " << _SB << endl;
    cout << "Số bảng FAT: " << _NF << endl;
    cout << "Kích thước bảng volume: " << _SV << endl;
    cout << "Kích thước mỗi bảng FAT: " << _SF << endl;
    cout << "Cluster bắt đầu của RDET: " << _SCOR << endl;
    cout << "Sector chứa thông tin phụ: " << _ExtraInforSector << endl;
    cout << "Sector chứa bản lưu của Boot Sector: " << _BackupBootSector << endl;
    cout << "Loại FAT: " << /*_FAT*/ endl;
}

long int FAT32::convertHexToDec(string hexa) {
    long int result = 0;
    result = stoi(hexa, nullptr, 16);

    return result;
}

long int FAT32::littleEndian(string sector[512], string offset, unsigned int byte) {
    string resultHex = "";
    long int resultDec = 0;
    int pos = convertHexToDec(offset);

    for (int i = pos + byte - 1; i >= pos; i--) {
        resultHex += sector[i];
    }

    resultDec = convertHexToDec(resultHex);

    //cout << resultHex << " (Hexa) = " << resultDec << " (Dec) " << endl;

    return resultDec;
}