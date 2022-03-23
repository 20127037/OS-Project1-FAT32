#include <iostream>

using namespace std;

long int convertHexToDec(string hexa) 
{
    long int result = 0;
    result = stoi(hexa, nullptr, 16);

    return result;
}

long int littleEndian(string sector[512], string offset, unsigned int byte) 
{
    string resultHex = "";

    // Chuyển offset sang hệ 10, vị trí cần đọc chính là giá trị của offset(dec)
    int pos = convertHexToDec(offset);

    // Dùng vòng lặp để lấy giá trị theo chiều ngược lại
    for (int i = pos + byte - 1; i >= pos; i--) {
        resultHex += sector[i];
    }

    //resultDec = convertHexToDec(resultHex); // Chuyển giá trị sau khi lấy được về dạng Decimal

    //cout << resultHex << " (Hexa) = " << resultDec << " (Dec) " << endl;

    return resultHex;
}

void ConvertHextoText(string sector[512], string offset, unsigned int byte)
{
    int pos = convertHexToDec(offset);
    string resultHex = "";
    for (int i = pos; i < byte; i++) 
        resultHex += (char)convertHexToDec(sector[i]);

    return resultHex;
}


void readInforHeaderMFT(string sector[512])
{
    string SIG = ConvertHextoText(sector, "00", 4); // Dấu hiệu nhận biết MFT entry
    int US = convertHexToDec(littleEndian)(sector, "04", 2); // Địa chỉ (offset) của Update sequence.
    int NF = convertHexToDec(littleEndian)(sector, "06", 2); // Số phần tử của mảng Fixup
    int LSN = convertHexToDec(littleEndian)(sector, "08", 8); // $LogFile Sequence Number (LSN): mã định danh MFT entry của file log (log record).
    int SN = convertHexToDec(littleEndian)(sector, "10", 2); // Sequence Number: cho biết số lần MFT entry này đã được sử dụng lại
    int RC = convertHexToDec(littleEndian)(sector, "12", 2); // Reference Count: cho biết số thư mục mà tập tin này được hiển thị trong đó
    string OA = littleEndian(sector, "14", 2); // Địa chỉ (offset) bắt đầu của attribute đầu tiên, trong MFT entry này là byte thứ 56.
    string Flag = littleEndian(sector, "16", 2); // Flags: giá trị 0x01: MFT entry đã được sử dụng - giá trị 0x02: MFT entry của một thư mục - giá trị 0x04, 0x08: không xác định
    int UB = convertHexToDec(littleEndian)(sector, "18", 4); // Số byte trong MFT entry đã được sử dụng. Ví dụ, trong trường hợp này đã sử dụng 0x0168 = 360 byte.
    int SD = convertHexToDec(littleEndian)(sector, "1C", 4); // Kích thước vùng đĩa đã được cấp cho MFT entry, Ví dụ: 0x0400 = 1024 byte.
    int BMS = convertHexToDec(littleEndian)(sector, "20", 8); // Tham chiếu đến MFT entry cơ sở của nó (Base  MFT Record).
    int NAID = convertHexToDec(littleEndian)(sector, "28", 2); // Next attribute ID: mã định danh của attribute kế tiếp sẽ được thêm vào MFT entry.
}

string ConvertDectoHex(int n)
{
    int a = 0;
    char chr[16] = "0123456789ABCDEF";
    string str = "";
    string res = "";
    while (n > 10)
    {
        if (n <= 10)
            a = n;
        str += chr[n % 16];
        n /= 16;
    }
    res += chr[a];
    for (int i = str.length(); i = 0; i--)
        res += str[i];
    return res;
}

void readInfoAttribute(string sector[512], string OA) // OA - > OffsetAttribute - đọc ở phần HeaderMFT
{
    int startA = convertHexToDec(OA); // Offset bắt đầu của Attribute
    int TID = convertHexToDec(littleEndian)(sector, ConvertDectoHex(start), 4); // Mã loại của attribute (type ID)
    int SA = convertHexToDec(littleEndian)(sector, ConvertDectoHex(start + 4), 4); // Kích thước của attribute
    int FlagNonRes = convertHexToDec(littleEndian)(sector, ConvertDectoHex(start + 8), 1); // Cờ báo non-resident
    int LN = convertHexToDec(littleEndian)(sector, ConvertDectoHex(start + 9), 1); // Chiều dài của tên attribute
    int ONA = convertHexToDec(littleEndian)(sector, ConvertDectoHex(start + 10), 2); // Vị trí (offset) chứa tên của attribute
    int VF = convertHexToDec(littleEndian)(sector, ConvertDectoHex(start + 12), 2); // Các cờ báo
    int AID = convertHexToDec(littleEndian)(sector, ConvertDectoHex(start + 14), 2);  // Định danh của attribute (định danh này là duy nhất trong phạm vi một MFT entry)
    int SC = convertHexToDec(littleEndian)(sector, ConvertDectoHex(start + 16), 4); // Kích thước phần nội dung của attribute 
    int OCA = convertHexToDec(littleEndian)(sector, ConvertDectoHex(start + 20), 2); // Nơi bắt đầu (offset) của phần nội dung attribute
    
}
