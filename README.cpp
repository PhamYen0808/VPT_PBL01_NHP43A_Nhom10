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

    Car() {
        LongTerm = false;
        isParked = false;
    }

    Car(string bienSo, Timee vao, bool daiHan = false) {
        licensePlate = bienSo;
        entryTime = vao;
        LongTerm = daiHan;
        isParked = true;
    }

    ~Car() {
        cout << "Huy xe: " << licensePlate << endl;
    }
};

class BaiDoCoBan {
protected:
    string tenBai;
    Car cars[20];
    int soXeHienTai;
    int tongThuNhap;

public:
    BaiDoCoBan(string name = "") : tenBai(name), soXeHienTai(0) {}

    virtual void menu() = 0;

    virtual ~BaiDoCoBan() {
        cout << "Giai phong bai do: " << tenBai << endl;
    }

    string getTenBai() {
        return tenBai;
    }

    void luuDuLieu() {
        ofstream outFile(tenBai + "_data.txt");
        if (!outFile) {
            cout << "Khong mo duoc file de luu du lieu.\n";
            return;
        }

        outFile << soXeHienTai << " " << tongThuNhap << "\n";
        for (int i = 0; i < soXeHienTai; i++) {
            Car& c = cars[i];
            outFile << c.licensePlate << " "
                    << c.entryTime.hour << " " << c.entryTime.minute << " "
                    << c.entryTime.day << " " << c.entryTime.month << " " << c.entryTime.year << " "
                    << c.LongTerm << " " << c.isParked << " " << c.hasLeft << "\n";
        }
        outFile.close();
    }
    void docDuLieu() {
        ifstream inFile(tenBai + "_data.txt");
        if (!inFile) {
            cout << "Khong tim thay du lieu luu tru cho bai " << tenBai << ".\n";
            return;
        }

        inFile >> soXeHienTai >> tongThuNhap;
        for (int i = 0; i < soXeHienTai; i++) {
            Car& c = cars[i];
            inFile >> c.licensePlate
                >> c.entryTime.hour >> c.entryTime.minute
                >> c.entryTime.day >> c.entryTime.month >> c.entryTime.year
                >> c.LongTerm >> c.isParked >> c.hasLeft;
        }
        inFile.close();
    }

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


    bool themXe(string bienSo, Timee entryTime) {
        if (soXeHienTai >= 20) {
            cout << "Bai do da day.\n";
            return false;
        }
        cars[soXeHienTai] = Car(bienSo, entryTime);
        ghiXeVaoFile(cars[soXeHienTai]);
        soXeHienTai++;
        cout << "Da them xe vao bai: " << bienSo << endl;
        return true;
    }

    bool thoiGianHopLe(Timee t) {
        if (t.hour < 0 || t.hour >= 24) 
        return false;
        if (t.minute < 0 || t.minute >= 60) 
        return false;
        if (t.month < 1 || t.month > 12) 
        return false;
        if (t.day < 1 || t.day > 31) 
        return false; 
        if (t.year =! 2025) 
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

    void dangKyDaiHan(string bienSo) {
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].licensePlate == bienSo && cars[i].isParked) {
                cars[i].LongTerm = true;
                cout << "Xe " << bienSo << " da dang ky thue dai han. Phi: 1.200.000 VND/thang\n";
                return;
            }
        }
        cout << "Khong tim thay xe dang dau trong bai.\n";
    }
    
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

    bool timXe(string bienSo) { 
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].licensePlate == bienSo) {
                cout << "Tim thay xe " << bienSo << " trong bai " << tenBai << ".\n";
                return true;
            }
        }
        return false;
    }

    void xemDanhSachXe() {
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

    int demSoXeDangDo() {
        int dem = 0;
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].isParked == true) {
                dem++;
            }
        }
        return dem;
    }

    int demSoXeRoiDi () {
        int dem = 0;
        for (int i = 0; i < soXeHienTai; i++) {
            if (!cars[i].isParked && cars[i].hasLeft) {
                dem++;
            }
        }
        return dem;
    }

    int demSoXeDangKyDH () {
        int dem = 0;
        for (int i = 0; i < soXeHienTai; i++)  {
            if(cars[i].LongTerm) {
                dem++; 
            }
        }
        return dem;
    }

    void xemXeVaoTuFile() {
        ifstream fileVao("ds_xe_vao.txt");
        ifstream fileRa("ds_xe_ra.txt");

        cout << "\n--- DANH SACH XE DA VAO ---\n";
        if (!fileVao) {
            cout << "Khong mo duoc file ds_xe_vao.txt\n";
        } else {
            string line;
            while (getline(fileVao, line)) {
                cout << line << endl;
            }
            fileVao.close();
            //  Mở file bằng Notepad
            system("start ds_xe_vao.txt");
        }

    }
     void xemXeRaTuFile() {
        ifstream fileRa("ds_xe_ra.txt");
        cout << "\n--- DANH SACH XE DA RA ---\n";
        if (!fileRa) {
            cout << "Khong mo duoc file ds_xe_ra.txt\n";
        } else {
            string line;
            while (getline(fileRa, line)) {
                cout << line << endl;
            }
            fileRa.close();
            //  Mở file bằng Notepad
            system("start ds_xe_ra.txt");
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
        cout << "\n--- THONG KE CHO BAI " << tenBai << " ---\n";
        cout << "So xe dang do: " << dangDo << endl;
        cout << "So xe da roi di: " << daRoi << endl;
        cout << "So xe dang thue dai han: " << daiHan << endl;
        cout << "Tong thu nhap: " << tongThuNhap << " VND\n";
    }
};

class BaiDo : public BaiDoCoBan {
public:
    BaiDo(string name = "") : BaiDoCoBan(name) {}

    void menu() override {
        int luaChon;
        do {
            cout << "\n--- Quan ly bai: " << tenBai << " ---\n";
            cout << "1. Them xe\n";
            cout << "2. Thanh toan\n";
            cout << "3. Dang ky thue dai han\n";
            cout << "4. Xem danh sach xe dang do\n";
            cout << "5. Xem danh sach xe vao\n";
            cout << "6. Xem danh sach xe ra\n";
            cout << "7. Xem thong ke nha xe\n";
            cout << "0. Quay lai\n";
            cout << "Chon chuc nang: ";
            cin >> luaChon;

            string bienSo;
            Timee t;

            switch (luaChon) {
                case 1:
                    cout << "Nhap bien so: "; cin >> bienSo;
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
int main() {
    BaiDo lienChieu("Lien Chieu");
    BaiDo thanhKhe("Thanh Khe");
    BaiDo sonTra("Son Tra");
    BaiDo haiChau("Hai Chau");

    lienChieu.docDuLieu();
    thanhKhe.docDuLieu();
    sonTra.docDuLieu();
    haiChau.docDuLieu();

    int luaChon;
    string pin;
    cout << "Nhap ma PIN de truy cap chuong trinh: ";
    cin >> pin;
    if (pin != "1234") {
        cout << "Ma PIN sai. Dang thoat...\n";
    return 0;
}
    do {
        cout << "\n===== MENU QUAN LY BAI DO =====\n";
        cout << "1. Lien Chieu\n2. Thanh Khe\n3. Son Tra\n4. Hai Chau\n5. Tim xe dang do\n6. Tim xe da roi di\n7. Xem so xe moi bai\n8. Thoat\n";
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
                cout << "So xe dang do tai Lien Chieu: " << lienChieu.demSoXeDangDo() << endl;
                cout << "So xe dang do tai Thanh Khe: " << thanhKhe.demSoXeDangDo() << endl;
                cout << "So xe dang do tai Son Tra: " << sonTra.demSoXeDangDo() << endl;
                cout << "So xe dang do tai Hai Chau: " << haiChau.demSoXeDangDo() << endl;
                break;
            }
            case 8: {
                char xacNhan;
                cout << "Ban co chac chan muon thoat? (y/n): ";
                cin >> xacNhan;
                if (xacNhan == 'y' || xacNhan == 'Y') {
                lienChieu.luuDuLieu();
                thanhKhe.luuDuLieu();
                sonTra.luuDuLieu();
                haiChau.luuDuLieu();
                cout << "Dang luu du lieu va thoat chuong trinh...\n";
                return 0;
                } else {
                luaChon = -1; // quay lại menu chính
                }
                break;
        }
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 7);
    return 0;
}
