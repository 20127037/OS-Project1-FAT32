#include "FAT32.h"
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

        //Cây thư mục:
        vector<int> rDetClusters = clusterArray(T, T.getSCOR(), drive); //cần <20s để tạo bảng // giảm bộ nhớ về 2048 có thể chỉnh sửa lại
        vector<BYTE> rdetData = byteArray(T, rDetClusters, drive);

        BYTE bangFat1[512];
        ReadSector(drive, T.getSB() * T.getBP(), bangFat1, 512);

        cout << endl << "----------------------------------------" << endl;
        EntryRdet(rdetData, bangFat1, T, drive);
    }
    else if (ntfs == "NTFS    ") // Xuất NTFS
    {
        BYTE sec[512];
        LARGE_INTEGER firstByte;

        // BPB
        N.displayBootSector(sector); // Bắt đầu từ offset 0
        cout << endl << "----------------------------------------" << endl;
        N.convertSectorToString(sector, res);
        N.readInfor(res);
        N.print();

        cout << endl << "----------------------------------------" << endl;

        // HEADER MFT
        firstByte.QuadPart = (long long int)(N.getBytesPerSector() * N.getSectorsPerCluster() * N.getLogicalClusterNumberforMFT());
        readSector(drive, firstByte, sec);
        string res1[512];
        N.displayBootSector(sec); // Bắt đầu từ offset firstByte

        cout << endl << "----------------------------------------" << endl;

        N.convertSectorToString(sec, res1);
        N.readInforHeaderMFT(res1);
        N.printHeaderMFT();

        cout << endl << "----------------------------------------" << endl;

        // HEADER ATTRIBUTE
        N.readInfoAttribute(res1, N.getOA());
        N.printHeaderAttribute();
    }
    else cout << "\nDay khong phai FAT32 hay NTFS. Vui long kiem tra lai o dia doc.";


    //ReadEntries(0, 0, rdetData, true, volume, txtFiles);
    //đọc bảng fat
   // /*T.setStartingByteRDET();
   // ReadSector(drive, T.getStartingByteRDET(), RDET);
   //displayBootSector(RDET);*/

    return 0;
}

