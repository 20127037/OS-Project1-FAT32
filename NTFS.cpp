#include "NTFS.h"

int NTFS::getBytesPerSector() {
    return _BytesPerSector;
}
int NTFS::getSectorsPerCluster() {
    return _SectorsPerCluster;
}
long long NTFS::getLogicalClusterNumberforMFT() {
    return _LogicalClusterNumberforMFT;
}

// Chuyển từ kiểu Byte của sector đọc từ USB thành string
void NTFS::convertSectorToString(BYTE sector[512], string sector_str[512]) {
    stringstream ss;
    string result;
    for (int i = 0; i < 512; i++) {
        ss.str("");
        ss.clear();
        ss << hex << int(sector[i]);
        result = ss.str();
        sector_str[i] = result;
    }
}

void NTFS::displayBootSector(BYTE sector[512]) {
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
void NTFS::readInfor(string sector[512]) {
    _BytesPerSector = littleEndian(sector, "0B", 2); // (sector, offset, số byte)
    _SectorsPerCluster = littleEndian(sector, "0D", 1);
    _MediaDescriptor = littleEndian(sector, "15", 1);
    _SectorsPerTrack = littleEndian(sector, "18", 2);
    _NumberOfHeads = littleEndian(sector, "1A", 2);
    _HiddenSectors = littleEndian(sector, "1C", 4);
    _TotalsSectors = littleEndian(sector, "28", 8);
    _LogicalClusterNumberforMFT = littleEndian(sector, "30", 8);
    _LogicalClusterNumberforMFTMirror = littleEndian(sector, "38", 8);
    _ClustersPerFileRecordSegment = littleEndian(sector, "40", 1);
    _ClustersPerIndexBuffer = littleEndian(sector, "44", 1);
}

void NTFS::print() {
    cout << dec << "Bytes per sector: " << _BytesPerSector << endl;
    cout << dec << "Sectors per cluster: " << _SectorsPerCluster << endl;
    cout << dec << "Media Descriptor: " << _MediaDescriptor << endl;
    cout << dec << "Sectors Per Track: " << _SectorsPerTrack << endl;
    cout << dec << "Number Of Heads: " << _NumberOfHeads << endl;
    cout << dec << "Hidden Sectors: " << _HiddenSectors << endl;
    cout << dec << "Totals Sectors: " << _TotalsSectors << endl;
    cout << dec << "Logical Cluster Number for MFT: " << _LogicalClusterNumberforMFT << endl;
    cout << dec << "Logical Cluster Number for MFT Mirror: " << _LogicalClusterNumberforMFTMirror << endl;
    cout << dec << "Clusters Per File Record Segment (Byte): " << _ClustersPerFileRecordSegment << endl;
    cout << dec << "Clusters Per Index Buffer: " << _ClustersPerIndexBuffer << endl;
}

// Hàm chuyển từ Hexa thành Decimal
long long int NTFS::convertHexToDec(string hexa) {
    long long int result = 0;
    result = stoi(hexa, nullptr, 16);

    return result;
}

long long int NTFS::littleEndian(string sector[512], string offset, unsigned int byte) {
    string resultHex = "";
    long long int resultDec = 0;

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

string NTFS::ConvertHextoText(string sector[512], string offset, unsigned int byte)
{
    int pos = convertHexToDec(offset);
    string resultHex = "";
    for (int i = pos; i < byte; i++)
        resultHex += (char)convertHexToDec(sector[i]);

    return resultHex;
}


void NTFS::readInforHeaderMFT(string sector[512])
{
    _SIG = ConvertHextoText(sector, "00", 4); // Dấu hiệu nhận biết MFT entry
    _US = littleEndian(sector, "04", 2); // Địa chỉ (offset) của Update sequence.
    _NF = littleEndian(sector, "06", 2); // Số phần tử của mảng Fixup
    //_LSN = littleEndian(sector, "08", 8); // $LogFile Sequence Number (LSN): mã định danh MFT entry của file log (log record).
    _SN = littleEndian(sector, "10", 2); // Sequence Number: cho biết số lần MFT entry này đã được sử dụng lại
    _RC = littleEndian(sector, "12", 2); // Reference Count: cho biết số thư mục mà tập tin này được hiển thị trong đó
    _OA = littleEndian(sector, "14", 2); // Địa chỉ (offset) bắt đầu của attribute đầu tiên, trong MFT entry này là byte thứ 56.
    _Flag = ConvertDectoHex(littleEndian(sector, "16", 2)); // Flags: giá trị 0x01: MFT entry đã được sử dụng - giá trị 0x02: MFT entry của một thư mục - giá trị 0x04, 0x08: không xác định
    _UB = littleEndian(sector, "18", 4); // Số byte trong MFT entry đã được sử dụng. Ví dụ, trong trường hợp này đã sử dụng 0x0168 = 360 byte.
    _SD = littleEndian(sector, "1C", 4); // Kích thước vùng đĩa đã được cấp cho MFT entry, Ví dụ: 0x0400 = 1024 byte.
    //_BMS = littleEndian(sector, "20", 8); // Tham chiếu đến MFT entry cơ sở của nó (Base  MFT Record).
    _NAID = littleEndian(sector, "28", 2); // Next attribute ID: mã định danh của attribute kế tiếp sẽ được thêm vào MFT entry.
}

string NTFS::ConvertDectoHex(int n)
{
    string res = "";
    stringstream ss;

    ss << hex << n;

    res = ss.str();

    return res;
}

void NTFS::readInfoAttribute(string sector[512], int _OA) // OA - > OffsetAttribute - đọc ở phần HeaderMFT
{
    int start = _OA; // Offset bắt đầu của Attribute
    _TID = littleEndian(sector, ConvertDectoHex(start), 4); // Mã loại của attribute (type ID)
    _SA = littleEndian(sector, ConvertDectoHex(start + 4), 4); // Kích thước của attribute
    _FlagNonRes = littleEndian(sector, ConvertDectoHex(start + 8), 1); // Cờ báo non-resident
    _LN = littleEndian(sector, ConvertDectoHex(start + 9), 1); // Chiều dài của tên attribute
    _ONA = littleEndian(sector, ConvertDectoHex(start + 10), 2); // Vị trí (offset) chứa tên của attribute
    _VF = littleEndian(sector, ConvertDectoHex(start + 12), 2); // Các cờ báo
    _AID = littleEndian(sector, ConvertDectoHex(start + 14), 2);  // Định danh của attribute (định danh này là duy nhất trong phạm vi một MFT entry)
    _SC = littleEndian(sector, ConvertDectoHex(start + 16), 4); // Kích thước phần nội dung của attribute 
    _OCA = littleEndian(sector, ConvertDectoHex(start + 20), 2); // Nơi bắt đầu (offset) của phần nội dung attribute
}

int NTFS::getOA()
{
    return _OA;
}

void NTFS::printHeaderMFT()
{
    cout << "Dau hieu nhan biet MFT entry:  " << _SIG << endl;
    cout << "Dia chi (offset) cua Update sequence: " << dec << _US << endl;
    cout << "So phan tu cua mang Fixup: " << dec << _NF << endl;
    //cout << "$LogFile Sequence Number (LSN): mã định danh MFT entry của file log (log record): " << dec << _LSN << endl;
    cout << "Sequence Number: " << dec << _SN << endl;
    cout << "Reference Count: " << dec << _RC << endl;
    cout << "Dia chi (offset) bat dau cua attribute dau tien: " << dec << _OA << endl;
    cout << "Flags: " << dec << _Flag << " (gia tri 0x01: MFT entry da duoc su dung - gia tri 0x02: MFT entry cua mot thu muc - gia tri 0x04, 0x08: khong xac dinh)" << endl;
    cout << "So byte trong MFT entry da duoc su dung: " << dec << _UB << endl;
    cout << "Kich thuoc vung dia da duoc cap cho MFT entry: " << dec << _SD << endl;
    //cout << "Tham chiếu đến MFT entry cơ sở của nó (Base  MFT Record): " << dec << _BMS << endl;
    cout << "Next attribute ID: " << dec << _NAID << endl;
}

void NTFS::printHeaderAttribute() {
    cout << dec << "Type ID : " << _TID << endl;
    cout << dec << "Kich thuoc cua attribute: " << _SA << endl;
    cout << dec << "Flag non-resident: " << _FlagNonRes << endl;
    cout << dec << "Chieu dai cua ten attribute: " << _LN << endl;
    cout << dec << "Vi tri chua ten cua attribute: " << _ONA << endl;
    cout << dec << "Flags: " << _VF << endl;
    cout << dec << "Dinh danh cua attribute: " << _AID << endl;
    cout << dec << "Kich thuoc pha nnoi dung cua attribute : " << _SC << endl;
    cout << dec << "Noi bat dau cua phan noi dung attribute: " << _OCA << endl;
}