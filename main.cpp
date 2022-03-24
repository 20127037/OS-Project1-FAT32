﻿#include "FAT32.h"
#include "NTFS.h"

void displayBootSector(BYTE* sector, int size) {
    cout << "Offset" << setw(9) << setfill(' ') << '0';
    for (int i = 0; i < 15; i++)
    {
        if ((i + 1) % 8 == 0) cout << "   ";
        cout << setw(3) << setfill(' ') << hex << i + 1;
    }
    for (int i = 0; i < size; i++) {
        if (i % 8 == 0 and i % 16 != 0) cout << "   ";
        if (i % 16 == 0) {
            cout << endl;
            cout << setw(2) << setfill('0') << i / 16 << '0' << setw(10) << setfill(' ') << ' ';
            cout << setw(2) << setfill('0') << hex << int(sector[i]) << " ";
        }
        else cout << setw(2) << setfill('0') << hex << int(sector[i]) << " ";
    }
}

int main(int argc, char** argv)
{
    FAT32 T;
    NTFS N;
    BYTE sector[512];
    string namePath;
    cout << "Read disk: ";
    cin >> namePath;
    string disk = "\\\\.\\" + namePath + ":";
    wstring temp = ConvertWStr(disk);
    LPCWSTR drive = temp.c_str();

    ReadSector(drive, 0, sector, 512); // ??c ? USB



    cout << endl << "--------------------------" << endl;
    string res[512];
    BYTE RDET[512];


    T.convertSectorToString(sector, res, 512);

    string ntfs = ConvertHextoText(res, "03", 8); // kiểm tra NTFS
    string type = ConvertHextoText(res, "52", 8); // ktra có phải FAT32 ko?
    
    if (type == "FAT32   ") // Xuất  FAT32
    {
        T.displayBootSector(sector); // Display bảng boot sector
        T.readInfor(res);
        T.print();
    }
    else if (ntfs == "NTFS    ") // Xuất NTFS
    {
        N.displayBootSector(sector);
        cout << endl << "----------------------------------------" << endl;
        //N.convertSectorToString(sector, res);
        N.readInfor(res);
        N.print();
        cout << endl << "----------------------------------------" << endl;
    }
    else cout << "\nDay khong phai FAT32 hay NTFS. Vui long kiem tra lai o dia doc.";

    //vector<int> rDetClusters = clusterArray(T, T.getSCOR(), drive); //cần <20s để tạo bảng
    //vector<BYTE> rdetData = byteArray(T, rDetClusters, drive);

    //EntryRdet(rdetData);
    //ReadEntries(0, 0, rdetData, true, volume, txtFiles);
    //đọc bảng fat
   // /*T.setStartingByteRDET();
   // ReadSector(drive, T.getStartingByteRDET(), RDET);
   //displayBootSector(RDET);*/

 //NTFS N;
 //string res[512];
 ////BYTE RDET[512];
 //N.displayBootSector(sector);
 //cout << endl;
 //N.convertSectorToString(sector, res);
 //N.readInfor(res);
 //N.print();
 //cout << endl << endl;
/*  N.readInforHeaderMFT(res);
  N.printHeaderMFT();*/


    return 0;
}

