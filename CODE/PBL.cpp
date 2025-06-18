#include <iostream>
#include <string>
#include<fstream>
using namespace std;

struct Timee {
    int hour;
    int minute;
    int day, month, year;
};

class Car {
public:
    string licensePlate;
    Timee entryTime;
    Timee exitTime;
    bool LongTerm;
    bool isParked;
    bool hasLeft;
    bool isBooked;        // có phải là chỗ được đặt trước không
    Timee datChoTime;     // thời gian đặt chỗ

    // Constructor mặc định
    Car() {
        LongTerm = false;
        isParked = false;
        hasLeft = false;
        isBooked = false;
        licensePlate = "";
    }

    // Khởi tạo biển số, thời gian vào và tùy chọn thuê dài hạn
    Car(string bienSo, Timee vao, bool daiHan = false) {
        licensePlate = bienSo;
        entryTime = vao;
        LongTerm = daiHan;
        isParked = true;
        isBooked = false;
    }

    ~Car() {
    if (!licensePlate.empty()) {
        cout << "Huy xe: " << licensePlate << endl;
        }
    }
};

class BaiDoCoBan {
protected:
    string tenBai;
    Car cars[20];
    int soXeHienTai;
    int tongThuNhap;

public:
    BaiDoCoBan(string name = "") : tenBai(name), soXeHienTai(0) {} // Khởi tạo tên bãi đỗ và số xe hiện tại

    virtual void menu() = 0;

    virtual ~BaiDoCoBan() {
        cout << "Giai phong bai do: " << tenBai << endl;
    }

    // Hàm lấy tổng thu nhập
    int getTongThuNhap() const {
    return tongThuNhap;
}
    // Hàm lấy tên bãi đỗ
    string getTenBai() {
        return tenBai;
    }

    int tinhPhutChenhLech(Timee a, Timee b) {
    return (b.hour * 60 + b.minute) - (a.hour * 60 + a.minute);
    }
    
     // Them xe vao bai do
    bool themXe(string bienSo, Timee entryTime) {
        kiemTraVaHuyDatCho(entryTime);
        // Kiểm tra biển số đã có trong bãi đỗ chưa
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].licensePlate == bienSo && cars[i].isParked) {
            cout << "Xe " << bienSo << " da co trong bai.\n";
            return false;
            }
        }
        // Kiểm tra nếu xe đã đặt chỗ trước
        for (int i = 0; i < 20; i++) {
        if (cars[i].licensePlate == bienSo && cars[i].isBooked && !cars[i].isParked) {
            // Đã đặt chỗ → ghi nhận xe đã đến
            cars[i].isBooked = false;
            cars[i].isParked = true;
            cars[i].entryTime = entryTime;
            cout << "Xe " << bienSo << " da den dung gio va duoc ghi nhan.\n";
            ghiXeVaoFile(cars[i]);
            return true;
        }
        }
        if (soXeHienTai >= 20) {
            cout << "Bai do da day.\n";
            return false;
        }
        // Thêm xe mới vào bãi đỗ
        cars[soXeHienTai] = Car(bienSo, entryTime);
        cars[soXeHienTai].isParked = true;
        cars[soXeHienTai].isBooked = false;
        ghiXeVaoFile(cars[soXeHienTai]);
        soXeHienTai++;
        cout << "Da them xe vao bai: " << bienSo << endl;
        return true;
    }

    // Xuất xe và tính phí
    // Nếu xe đã đăng ký thuê dài hạn thì không tính phí
    // Tích hợp các ưu đãi
    void xuatXeVaTinhPhi(string bienSo, Timee exitTime) {
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].licensePlate == bienSo && cars[i].isParked) {
                cars[i].exitTime = exitTime;
                cars[i].isParked = false;
                cars[i].hasLeft = true;

                if (!cars[i].LongTerm) {
                    int VC = 0;
                    if (cars[i].entryTime.day == 1 || cars[i].entryTime.day == 30) VC += 5;
                    if (cars[i].entryTime.day == cars[i].entryTime.month) VC += 10;
                    if (cars[i].entryTime.day == 30 && cars[i].entryTime.month == 4) VC += 10;

                    int entryMinutes = cars[i].entryTime.hour * 60 + cars[i].entryTime.minute;
                    int exitMinutes = cars[i].exitTime.hour * 60 + cars[i].exitTime.minute;
                    int soPhut = exitMinutes - entryMinutes;
                    int soGio = soPhut / 60;
                    int soPhutLe = soPhut % 60;
                    int phi = 0;

                    if (cars[i].exitTime.day == cars[i].entryTime.day &&
                        cars[i].exitTime.month == cars[i].entryTime.month &&
                        cars[i].exitTime.year == cars[i].entryTime.year) {
                        if (soGio < 5) phi = soGio * 20000 + soPhutLe * 20000 / 60;
                        else phi = 100000 + (soGio - 5) * 25000 + soPhutLe * 25000 / 60;
                    } else {
                        int soNgay = cars[i].exitTime.day - cars[i].entryTime.day;
                        if (cars[i].exitTime.month != cars[i].entryTime.month ||
                            cars[i].exitTime.year != cars[i].entryTime.year)
                            soNgay += 30;
                        phi = soNgay * 120000;
                    }

                    int phiSauGiam = phi * (100 - VC) / 100;
                    tongThuNhap += phiSauGiam;
                    ghiHoaDon(cars[i], phiSauGiam, VC);
                    cout << "Phi cho xe " << bienSo << ": " << phiSauGiam
                         << " VND (Da ap dung " << VC << "% uu dai)\n";
                } else {
                    cout << "Xe " << bienSo << " da dang ky thue dai han. Khong tinh phi.\n";
                }

                ghiXeRaFile(cars[i]);
                cout << "Xe " << bienSo << " da xuat khoi bai do luc "
                     << cars[i].exitTime.hour << "h" << cars[i].exitTime.minute << "p.\n";

                for (int j = i; j < soXeHienTai - 1; j++)
                    cars[j] = cars[j + 1];
                soXeHienTai--;
                return;
            }
        }
        cout << "Khong tim thay xe trong bai.\n";
    }
    
    // Đặt chỗ trước cho xe
    // Nếu bãi đỗ đã đầy thì không cho đặt chỗ
    bool datChoTruoc(string bienSo, Timee tgDat) {
        if (soXeHienTai >= 20) {
        cout << "Khong the dat cho. Bai da day.\n";
        return false;
        }
        for (int i = 0; i < 20; i++) {
        if (!cars[i].isParked && cars[i].licensePlate.empty()) { // tìm vị trí trống
            cars[i].licensePlate = bienSo;
            cars[i].isBooked = true;
            cars[i].datChoTime = tgDat;
            soXeHienTai++;
            cout << "Da dat truoc cho xe " << bienSo << ". Vui long den trong 30 phut.\n";
            return true;
        }
        }
    return false;
    }

    // Kiểm tra và hủy đặt chỗ nếu quá hạn
    // Nếu xe đã đặt chỗ quá 30 phút thì hủy đặt chỗ
    void kiemTraVaHuyDatCho(Timee hienTai) {
        for (int i = 0; i < 20; i++) {
            if (cars[i].isBooked && !cars[i].isParked) {
                int chenhlech = tinhPhutChenhLech(cars[i].datChoTime, hienTai);
                if (chenhlech > 30) {
                    cout << "Huy dat cho cua xe " << cars[i].licensePlate << " do qua han.\n";
                    cars[i] = Car(); // xóa thông tin
                    soXeHienTai--;
                }
            }
        }
    }

    // // Luu du lieu vao file
    // void luuDuLieu() {
    //     ofstream outFile(tenBai + "_data.txt");
    //     if (!outFile) {
    //         cout << "Khong mo duoc file de luu du lieu.\n";
    //         return;
    //     }

    //     outFile << soXeHienTai << " " << tongThuNhap << "\n";
    //     for (int i = 0; i < soXeHienTai; i++) {
    //         Car& c = cars[i];
    //         outFile << c.licensePlate << " "
    //                 << c.entryTime.hour << " " << c.entryTime.minute << " "
    //                 << c.entryTime.day << " " << c.entryTime.month << " " << c.entryTime.year << " "
    //                 << c.LongTerm << " " << c.isParked << " " << c.hasLeft << "\n";
    //     }
    //     outFile.close();
    // }
    // Ghi xe vao vao file
    void ghiXeVaoFile(const Car& car) {
        ofstream file("ds_xe_vao.txt", ios::app);
        if (!file) {
            return;
        }
        Timee vao = car.entryTime;
        file << "Xe: " << car.licensePlate << ", Vao: "
             << vao.hour << ":" << (vao.minute < 10 ? "0" : "") << vao.minute << " "
             << vao.day << "/" << vao.month << "/" << vao.year << endl;
        file.close();
    }

    // Ghi xe ra vao file
    void ghiXeRaFile(const Car& car) {
        ofstream file("ds_xe_ra.txt", ios::app);
        if (!file) {
            return;
        }
        Timee ra = car.exitTime;
        file << "Xe: " << car.licensePlate << ", Ra: "
             << ra.hour << ":" << (ra.minute < 10 ? "0" : "") << ra.minute << " "
             << ra.day << "/" << ra.month << "/" << ra.year << endl;
        file.close();
    }

   
    // Kiểm tra thời gian hợp lệ
    // Chỉ cho phép năm 2025 trở lên, tháng từ 1 đến 12, ngày từ 1 đến 31, giờ từ 0 đến 23, phút từ 0 đến 59
    bool thoiGianHopLe(Timee t) {
        if (t.hour < 0 || t.hour >= 24) 
        return false;
        if (t.minute < 0 || t.minute >= 60) 
        return false;
        if (t.month < 1 || t.month > 12) 
        return false;
        if (t.day < 1 || t.day > 31) 
        return false; 
        if (t.year < 2025) 
        return false;

    // Kiểm tra số ngày tối đa theo tháng
        int maxDay = 31;
        if (t.month == 4 || t.month == 6 || t.month == 9 || t.month == 11) maxDay = 30;
        else if (t.month == 2) {
            bool isLeap = (t.year % 4 == 0 && (t.year % 100 != 0 || t.year % 400 == 0));
            maxDay = isLeap ? 29 : 28;
        }

        if (t.day > maxDay) return false;

        return true;
    }

    // Đăng ký thuê dài hạn
    void dangKyDaiHan(string bienSo) {
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].licensePlate == bienSo && cars[i].isParked) {
                cars[i].LongTerm = true;
                cout << "Xe " << bienSo << " da dang ky thue dai han. Phi: 1.200.000 VND/thang\n";
                tongThuNhap += 1200000; // Cộng thêm thu nhập
                ghiHoaDon(cars[i], 1200000, 0); // Ghi hóa đơn thuê dài hạn
                return;
            }
        }
        cout << "Khong tim thay xe dang dau trong bai.\n";
    }
    
    // Tìm xe đã ra khỏi bãi
    // Tìm kiếm trong file ds_xe_ra.txt
    void timXeDaRa(string bienSo) {
        ifstream file("ds_xe_ra.txt");
        if (!file) {
            cout << "Khong mo duoc file ds_xe_ra.txt\n";
            return;
        }
        string line;
        bool found = false;
        while (getline(file, line)) {
            if (line.find(bienSo) != string::npos) {
                cout << "Tim thay: " << line << endl;
                found = true;
            }
        }
        if (!found) cout << "Khong tim thay xe trong danh sach da ra.\n";
        file.close();
    }

    // Ghi hóa đơn vào file hoa_don.txt
    void ghiHoaDon(const Car& car, int phiSauGiam, int VC) {
        ofstream file("hoa_don.txt", ios::app);
        if (!file) return;
        file << "Bien so: " << car.licensePlate << ", Vao: "
             << car.entryTime.hour << ":" << car.entryTime.minute << " "
             << car.entryTime.day << "/" << car.entryTime.month << "/" << car.entryTime.year
             << ", Ra: " << car.exitTime.hour << ":" << car.exitTime.minute << " "
             << car.exitTime.day << "/" << car.exitTime.month << "/" << car.exitTime.year
             << ", Uu dai: " << VC << "%"
             << ", Thanh tien: " << phiSauGiam << " VND\n";
        file.close();
    }

    // Tìm xe theo biển số
    bool timXe(string bienSo) { 
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].licensePlate == bienSo) {
                cout << "Tim thay xe " << bienSo << " trong bai " << tenBai << ".\n";
                return true;
            }
        }
        return false;
    }

    // Xem danh sách xe đang đỗ
    void xemDanhSachXe() {
        khungTieuDe("DANH SACH XE DANG DO");
        if (soXeHienTai == 0) {
            cout << "Khong co xe nao dang do trong bai " << tenBai << ".\n";
            return;
        }
        cout << "Danh sach xe dang do tai bai " << tenBai << ":\n";
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].isParked) {
                cout << i + 1 << ". " << cars[i].licensePlate;
                if (cars[i].LongTerm) {
                    cout << " (Dang ky dai han)";
                }
                cout << endl;
            }
        }
    }

    // Đếm số xe đang đỗ
    int demSoXeDangDo() {
        int dem = 0;
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].isParked == true) {
                dem++;
            }
        }
        return dem;
    }

    // Đếm số xe đã rời đi
    int demSoXeRoiDi () {
        int dem = 0;
        for (int i = 0; i < soXeHienTai; i++) {
            if (!cars[i].isParked && cars[i].hasLeft) {
                dem++;
            }
        }
        return dem;
    }
    
    // Đếm số xe đã đăng ký thuê dài hạn
    int demSoXeDangKyDH () {
        int dem = 0;
        for (int i = 0; i < soXeHienTai; i++)  {
            if(cars[i].LongTerm) {
                dem++; 
            }
        }
        return dem;
    }
    
    // Hàm tạo khung tiêu đề
    void khungTieuDe(string tieuDe) {
        int width = 40;
        int pad = (width - tieuDe.length() - 4) / 2;
        cout << "\n+";
        for (int i = 0; i < width; i++) cout << "=";
        cout << "+\n|";
        for (int i = 0; i < pad; i++) cout << " ";
        cout << tieuDe;
        for (int i = 0; i < width - pad - tieuDe.length(); i++) cout << " ";
        cout << "|\n+";
        for (int i = 0; i < width; i++) cout << "-";
        cout << "+\n";
    }

    // Xem danh sách xe đã vào bãi
    void xemXeVaoTuFile() {
        ifstream fileVao("ds_xe_vao.txt");

        khungTieuDe("DANH SACH XE DA VAO");
        if (!fileVao) {
            cout << "Khong mo duoc file ds_xe_vao.txt\n";
        } else {
            string line;
            while (getline(fileVao, line)) {
            cout << line << endl;
            }
            fileVao.close();
            // Mở file bằng Notepad (nếu muốn)
            //system("start ds_xe_vao.txt");
        }
    }

    // Xem danh sách xe đã ra khỏi bãi
    void xemXeRaTuFile() {
        ifstream fileRa("ds_xe_ra.txt");

        khungTieuDe("DANH SACH XE DA RA");
        if (!fileRa) {
            cout << "Khong mo duoc file ds_xe_ra.txt\n";
        } else {
            string line;
            while (getline(fileRa, line)) {
                cout << line << endl;
            }
            fileRa.close();
            //system("start ds_xe_ra.txt"); (Xem tu file neu can)
        }
    }
    void hienThiThongKe() {
        int dangDo = 0;
        int daRoi = 0;
        int daiHan = 0;

        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].isParked) dangDo++;
            else daRoi++;

            if (cars[i].LongTerm) daiHan++;
        }
        khungTieuDe("THONG KE CHO BAI " + tenBai);
        cout << "So xe dang do: " << dangDo << endl;
        cout << "So xe da roi di: " << daRoi << endl;
        cout << "So xe dang thue dai han: " << daiHan << endl;
        cout << "Tong thu nhap: " << tongThuNhap << " VND\n";
    }

    // Kiểm tra nếu bãi đỗ đã đông
    bool Kiemtra() { //Caảnh báo nếu bãi đỗ đã đông
        if (soXeHienTai >= 17) {
            cout << "Bai do dang co nhieu xe, Ban co muon tiep tuc?(y/n)\n";
            return false; // Không cho thêm xe mới
        }
            return true;
    }
    void hienThiBangDoXe() {
    cout << "========= " << tenBai << " =========\n";
    for (int i = 0; i < 20; i++) {
        if (i % 5 == 0) cout << "| ";
        cout << (cars[i].isParked ? "X" : "O") << " | ";
        if (i % 5 == 4) cout << endl;
    }
    cout << "O = Trong   X = Co xe\n";
} 
};

class BaiDo : public BaiDoCoBan {
public:
    BaiDo(string name = "") : BaiDoCoBan(name) {}
    void datChotuxa(){
        
    }
    void menu() override {
        int luaChon;
        do {
            khungTieuDe("QUAN LY BAI: " + tenBai);
            cout << "1. Them xe\n";
            cout << "2. Thanh toan\n";
            cout << "3. Dang ky thue dai han\n";
            cout << "4. Xem danh sach xe dang do\n";
            cout << "5. Xem danh sach xe vao\n";
            cout << "6. Xem danh sach xe ra\n";
            cout << "7. Xem thong ke nha xe\n";
            cout << "8. Dat cho truoc\n";
            cout << "0. Quay lai\n";
            cout << "Chon chuc nang: ";
            cin >> luaChon;

            string bienSo;
            Timee t;

            switch (luaChon) {
                case 1:
                    if (Kiemtra()==false)
                    {char xacNhan;
                    cin >> xacNhan;
                    if (xacNhan != 'n' && xacNhan != 'N') {
                        cout << "Vui long chon bai khac tu menu chinh.\n";
                        break;
                    }}
                    cout << "Nhap bien so: "; 
                    cin >> bienSo;
                    cout << "Nhap thoi gian vao (ngay thang nam): ";
                    cin >> t.day >> t.month >> t.year;
                    cout << "Nhap gio phut (0h-23h): ";
                    cin >> t.hour >> t.minute;
                    if (!thoiGianHopLe(t)) {
                        cout << "Thoi gian khong hop le!\n";
                        break;
                    }   
                    themXe(bienSo, t);
                    break;
                case 2:
                    cout << "Nhap bien so: "; cin >> bienSo;
                    cout << "Nhap thoi gian ra (ngay thang nam ): ";
                    cin >> t.day >> t.month >> t.year;
                    cout << "Nhap gio phut (0h-23h): ";
                    cin >> t.hour >> t.minute;
                    if (!thoiGianHopLe(t)) {
                        cout << "Thoi gian khong hop le!\n";
                        break;
                    }
                    xuatXeVaTinhPhi(bienSo, t); // THAY vì xuatXe(...)
                    break;
                case 3:
                    cout << "Nhap bien so: "; cin >> bienSo;
                    dangKyDaiHan(bienSo);
                    break;
                case 4:
                    xemDanhSachXe();
                    hienThiBangDoXe();
                    break;
                case 5:
                    xemXeVaoTuFile();
                    break;
                case 6: 
                    xemXeRaTuFile();
                    break;
                case 7:
                    hienThiThongKe();
                    break;
                case 8:
                    cout << "Nhap bien so xe muon dat truoc: ";
                    cin >> bienSo;
                    cout << "Nhap thoi gian dat (ngay thang nam): ";
                    cin >> t.day >> t.month >> t.year;
                    cout << "Nhap gio phut (0h-23h): ";
                    cin >> t.hour >> t.minute;
                    if (!thoiGianHopLe(t)) {
                    cout << "Thoi gian khong hop le!\n";
                    break;
                    }
                    datChoTruoc(bienSo, t);
                    break; 
                case 0:
                    break;
                default:
                    cout << "Lua chon khong hop le.\n";
            }
        } while (luaChon != 0);
    }
};
void timXeDaRaChung(string bienSo, BaiDoCoBan& bai) { // Khai báo riêng vì không thuộc riêng bãi nào
        cout << "\n[" << bai.getTenBai() << "]\n";
        bai.timXeDaRa(bienSo);
    }
void goiYBaiTrongNhat(BaiDoCoBan& a, BaiDoCoBan& b, BaiDoCoBan& c, BaiDoCoBan& d) {
    BaiDoCoBan* baiTot = &a;
    if (b.demSoXeDangDo() < baiTot->demSoXeDangDo()) baiTot = &b;
    if (c.demSoXeDangDo() < baiTot->demSoXeDangDo()) baiTot = &c;
    if (d.demSoXeDangDo() < baiTot->demSoXeDangDo()) baiTot = &d;

    cout << "Goi y: Nen chon bai " << baiTot->getTenBai() << " vi it xe nhat!\n";
}

string currentPIN = "1234"; // mặc định

void docPIN() {
    ifstream file("pin.txt");
    if (file) {
        getline(file, currentPIN);
        file.close();
    }
}

void ghiPIN() {
    ofstream file("pin.txt");
    if (file) {
        file << currentPIN;
        file.close();
    }
}
BaiDo lienChieu("Lien Chieu");
BaiDo thanhKhe("Thanh Khe");
BaiDo sonTra("Son Tra"); 
BaiDo haiChau("Hai Chau");

int main() {
    docPIN(); // Đọc PIN từ file nếu có
    cout << "\n--- CHAO MUNG DEN VOI HE THONG QUAN LY BAI DO ---\n";
    cout << "------------------------------------------------------\n";
    int luaChon;
    string pin;
    cout << "Nhap ma PIN de truy cap chuong trinh: ";
    cin >> pin;
    if (pin != currentPIN) {
        cout << "Ma PIN sai. Dang thoat...\n";
    return 0;
}
    do {
        cout << "\n===== MENU QUAN LY BAI DO =====\n";
        cout << "1. Lien Chieu\n2. Thanh Khe\n3. Son Tra\n4. Hai Chau\n5. Tim xe dang do\n6. Tim xe da roi di\n7. Thong ke moi bai\n8. Tim bai trong nhat.\n9. Doi ma Pin\n10. Thoat\n";
        cout << "Chon chuc nang: ";
        cin >> luaChon;

        switch (luaChon) {
            case 1: lienChieu.menu(); break;
            case 2: thanhKhe.menu(); break;
            case 3: sonTra.menu(); break;
            case 4: haiChau.menu(); break;
            case 5: {
                string bienSo;
                cout << "Nhap bien so can tim: ";
                cin >> bienSo;
                bool found = lienChieu.timXe(bienSo) || thanhKhe.timXe(bienSo) || sonTra.timXe(bienSo) || haiChau.timXe(bienSo);
                if (!found) cout << "Khong tim thay xe trong cac bai.\n";
                break;
            }
            case 6: {
                string bienSo;
                cout << "Nhap bien so xe da ra can tim: ";
                cin >> bienSo;
                timXeDaRaChung(bienSo, lienChieu);
                timXeDaRaChung(bienSo, thanhKhe);
                timXeDaRaChung(bienSo, sonTra);
                timXeDaRaChung(bienSo, haiChau);
                break;
            }
            case 7: {
                int doanhThuLienChieu = lienChieu.getTongThuNhap();
                int doanhThuThanhKhe = thanhKhe.getTongThuNhap();
                int doanhThuSonTra = sonTra.getTongThuNhap();
                int doanhThuHaiChau = haiChau.getTongThuNhap();
                int tongDoanhThu = doanhThuLienChieu + doanhThuThanhKhe + doanhThuSonTra + doanhThuHaiChau;

                cout << "So xe dang do tai Lien Chieu: " << lienChieu.demSoXeDangDo() << ", Doanh thu: " << doanhThuLienChieu << " VND\n";
                cout << "So xe dang do tai Thanh Khe: " << thanhKhe.demSoXeDangDo() << ", Doanh thu: " << doanhThuThanhKhe << " VND\n";
                cout << "So xe dang do tai Son Tra: " << sonTra.demSoXeDangDo() << ", Doanh thu: " << doanhThuSonTra << " VND\n";
                cout << "So xe dang do tai Hai Chau: " << haiChau.demSoXeDangDo() << ", Doanh thu: " << doanhThuHaiChau << " VND\n";

                cout << ">> Tong doanh thu cua tat ca bai: " << tongDoanhThu << " VND\n";
                break;
            }   
            
            case 8: {
                cout << "\n--- Goi y bai trong nhat ---\n";
                goiYBaiTrongNhat(lienChieu, thanhKhe, sonTra, haiChau);
                break;
            }
            case 9: {
                string oldPIN, newPIN;
                cout << "Nhap ma PIN hien tai: ";
                cin >> oldPIN;
                if (oldPIN != currentPIN) {
                cout << "Sai ma PIN. Khong the doi.\n";
                break;
                }
                cout << "Nhap ma PIN moi: ";
                cin >> newPIN;
                currentPIN = newPIN;
                ghiPIN();
                cout << "Da doi ma PIN thanh cong!\n";
                break;
            }
            case 0: {
                char xacNhan;
                cout << "Ban co chac chan muon thoat? (y/n): ";
                cin >> xacNhan;
                if (xacNhan == 'y' || xacNhan == 'Y') {
                return 0;
                } else {
                luaChon = -1; // quay lại menu chính
                }
                break;
        }
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 0);
    return 0;
}
