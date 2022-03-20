#include <windows.h>
#include <iostream>
#include <iomanip>
#include <stdio.h>
using namespace std;
int ReadSector(LPCWSTR  drive, int readPoint, BYTE sector[512])
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

    SetFilePointer(device, readPoint, NULL, FILE_BEGIN);//Set a Point to Read

    if (!ReadFile(device, sector, 512, &bytesRead, NULL))
    {
        printf("ReadFile: %u\n", GetLastError());
    }
    else
    {
        printf("Success!\n");
    }
}
void displayBootSector(BYTE sector[512]) {
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
int HexaToDecConverter(string Hexa)
{
    //temp
        int result;
        return result;
}
int main(int argc, char ** argv)
{
    
    BYTE sector[512];
    ReadSector(L"\\\\.\\E:", 0, sector); // Đọc ổ USB
    displayBootSector(sector); // Display bảng boot sector

    return 0;
}
