#include "FAT32.h"

// Chuyển từ kiểu Byte của sector đọc từ USB thành string
void FAT32::convertSectorToString(BYTE* sector, string* sector_str,int size) {
    stringstream ss;
    string result;
    for (int i = 0; i < size; i++) {
        ss.str("");
        ss.clear();
        ss << hex << int(sector[i]);
        result = ss.str();
        sector_str[i] = result;
    }
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

// Đọc thông tin từ bảng Boot Sector 
void FAT32::readInfor(string sector[512]) {
    _BP = littleEndian(sector, "0B", 2); // Offset B - số byte 2 - Số byte mỗi sector
    _SC = littleEndian(sector, "0D", 1); // (sector, offset, số byte)
    _SB = littleEndian(sector, "0E", 2);
    _NF = littleEndian(sector, "10", 1);
    _SV = littleEndian(sector, "20", 4);
    _SF = littleEndian(sector, "24", 4);
    _SCOR = littleEndian(sector, "2C", 4);
    _ExtraInforSector = littleEndian(sector, "30", 2);
    _BackupBootSector = littleEndian(sector, "32", 2);
}

void FAT32::print() {
    cout << dec << "So byte moi sector: " << _BP << endl;
    cout << dec <<"So sector tren cluser: " << _SC << endl;
    cout << dec << "So sector thuoc vung Bootsector: " << _SB << endl;
    cout << dec << "So bang FAT: " << _NF << endl;
    cout << dec << "Kich thuoc bang volume: " << _SV << endl;
    cout << dec << "Kich thuoc moi bang FAT: " << _SF << endl;
    cout << dec << "Cluster bat dau cua RDET: " << _SCOR << endl;
    cout << dec << "Sector chua thong tin phu: " << _ExtraInforSector << endl;
    cout << dec << "Sector chua ban luu cua Boot Sector: " << _BackupBootSector << endl;
}

long int FAT32::getBP() {
    return _BP;
}


long int FAT32::getSC() {
    return _SC;
}
long int FAT32::getSB() {
    return _SB;
}
long int FAT32::getNF() {
    return _NF;
}
long int FAT32::getSV() {
    return _SV;
}
long int FAT32::getSF() {
    return _SF;
}
long int FAT32::getSCOR() {
    return _SCOR;
}
long int FAT32::getExtraInforSector() {
    return _ExtraInforSector;
}
long int FAT32::getBackupBootSector() {
    return _BackupBootSector;
}

void FAT32::setStartingByteRDET() {
    StartingByteRDET = (_NF * _SF + _SB) * 512;
}
long int FAT32::getStartingByteRDET() {
    return StartingByteRDET;
}
// Hàm chuyển từ Hexa thành Decimal
long int FAT32::convertHexToDec(string hexa) {
    long int result = 0;
    result = stoi(hexa, nullptr, 16);

    return result;
}

long int FAT32::littleEndian(string sector[512], string offset, unsigned int byte) {
    string resultHex = "";
    long int resultDec = 0;

    // Chuyển offset sang hệ 10, vị trí cần đọc chính là giá trị của offset(dec)
    int pos = convertHexToDec(offset); 

    // Dùng vòng lặp để lấy giá trị theo chiều ngược lại
    for (int i = pos + byte - 1; i >= pos; i--) {
        if (sector[i] == "0")
            resultHex += "00";
        else
        resultHex += sector[i];
    }

    resultDec = convertHexToDec(resultHex); // Chuyển giá trị sau khi lấy được về dạng Decimal

    //cout << resultHex << " (Hexa) = " << resultDec << " (Dec) " << endl;

    return resultDec;
}
