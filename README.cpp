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

public:
    BaiDoCoBan(string name = "") : tenBai(name), soXeHienTai(0) {}

    virtual void menu() = 0;

    virtual ~BaiDoCoBan() {
        cout << "Giai phong bai do: " << tenBai << endl;
    }

    string getTenBai() {
        return tenBai;
    }
    void ghiXeVaoFile(const Car& car) {
        ofstream file("ds_xe_vao.txt", ios::app);
        if (!file) {
            cout << "Khong the mo file de ghi xe vao.\n";
            return;
        }
        Timee vao = car.entryTime;
        file << "Xe: " << car.licensePlate << ", Vao: "
             << vao.hour << ":" << (vao.minute < 10 ? "0" : "") << vao.minute << " "
             << vao.day << "/" << vao.month << "/" << vao.year << endl;
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

    bool xuatXe(string bienSo, Timee exitTime) {
    for (int i = 0; i < soXeHienTai; i++) {
        if (cars[i].licensePlate == bienSo && cars[i].isParked) {
            cars[i].exitTime = exitTime;
            cars[i].isParked = false;

            cout << "Xe " << bienSo << " da xuat khoi bai do luc ";
            cout << cars[i].exitTime.hour << "h" << cars[i].exitTime.minute << "p.\n";

            // Xoá xe ra khỏi mảng bằng cách dịch các phần tử phía sau lên
            for (int j = i; j < soXeHienTai - 1; j++) {
                cars[j] = cars[j + 1];
            }

            soXeHienTai--; // Giảm số lượng xe đang có

            return true;
        }
    }
    cout << "Khong tim thay xe trong bai.\n";
    return false;
    }

    void tinhPhi(string bienSo) {
        int tongVC=0;
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].licensePlate == bienSo) {
                if (cars[i].LongTerm) {
                    tongVC =0;
                }
                else {
                    if (cars[i].entryTime.day == 1 || cars[i].entryTime.day == 30){
                    tongVC = tongVC + 5;
                    }
                    if (cars[i].entryTime.day == cars[i].entryTime.month){
                    tongVC = tongVC + 10;
                    }
                    if (cars[i].entryTime.day == 30 && cars[i].entryTime.month == 4 ){
                    tongVC = tongVC + 10;
                    }
                }    
            }
            }
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].licensePlate == bienSo) {
                if (cars[i].LongTerm) {
                    cout << "Xe " << bienSo << " da dang ky thue dai han. Khong tinh phi.\n";
                    return;
                }

                int entryMinutes = cars[i].entryTime.hour * 60 + cars[i].entryTime.minute;
                int exitMinutes = cars[i].exitTime.hour * 60 + cars[i].exitTime.minute;
                int soPhut = exitMinutes - entryMinutes;
                int soGio = soPhut / 60;
                int soPhutLe = soPhut % 60;
                int phi = 0;

                if (cars[i].exitTime.day == cars[i].entryTime.day) {
                    if (soGio < 5) {
                        phi = soGio * 20000 + soPhutLe * 20000 / 60;
                    } else {
                        phi = 100000 + (soGio - 5) * 25000 + soPhutLe * 25000 / 60;
                    }
                } else {
                    int soNgay = cars[i].exitTime.day - cars[i].entryTime.day;
                    phi = soNgay * 120000;
                }

                cout << "Phi cho xe " << bienSo << ": " << phi * (100 - tongVC) / 100 << " VND\n";
                return;
            }
            else {
                cout << "Khong tim thay xe " << bienSo << " trong bai.\n";
            }
        }
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
    void ghiXeDaRaVaoFile() {
        ofstream file("ds_xe_roi.txt", ios::app);
        if (!file) {
            cout << "Khong the mo file de ghi.\n";
            return;
        }
        file << "===== DANH SACH XE ROI BAI: " << tenBai << " =====\n";
        for (int i = 0; i < soXeHienTai; i++) {
            if (!cars[i].isParked && cars[i].hasLeft) {
                Timee vao = cars[i].entryTime;
                Timee ra = cars[i].exitTime;
                file << "Xe: " << cars[i].licensePlate << ", Vao: "
                     << vao.hour << ":" << (vao.minute < 10 ? "0" : "") << vao.minute << " "
                     << vao.day << "/" << vao.month << "/" << vao.year << ", Ra: "
                     << ra.hour << ":" << (ra.minute < 10 ? "0" : "") << ra.minute << " "
                     << ra.day << "/" << ra.month << "/" << ra.year << endl;
            }
        }
        file.close();
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
            cout << "2. Xuat xe\n";
            cout << "3. Tinh phi\n";
            cout << "4. Dang ky thue dai han\n";
            cout << "5. Xem danh sach xe\n";
            cout << "0. Quay lai\n";
            cout << "Chon chuc nang: ";
            cin >> luaChon;

            string bienSo;
            Timee t;

            switch (luaChon) {
                case 1:
                    cout << "Nhap bien so: "; cin >> bienSo;
                    cout << "Nhap thoi gian vao (ngay thang nam gio phut): ";
                    cin >> t.day >> t.month >> t.year >> t.hour >> t.minute;
                    themXe(bienSo, t);
                    break;
                case 2:
                    cout << "Nhap bien so: "; cin >> bienSo;
                    cout << "Nhap thoi gian ra (ngay thang nam gio phut): ";
                    cin >> t.day >> t.month >> t.year >> t.hour >> t.minute;
                    xuatXe(bienSo, t);
                    break;
                case 3:
                    cout << "Nhap bien so: "; cin >> bienSo;
                    tinhPhi(bienSo);
                    break;
                case 4:
                    cout << "Nhap bien so: "; cin >> bienSo;
                    dangKyDaiHan(bienSo);
                    break;
                case 5:
                    xemDanhSachXe();
                    break;
                case 0:
                    break;
                default:
                    cout << "Lua chon khong hop le.\n";
            }
        } while (luaChon != 0);
    }
};

int main() {
    BaiDo lienChieu("Lien Chieu");
    BaiDo thanhKhe("Thanh Khe");
    BaiDo sonTra("Son Tra");
    BaiDo haiChau("Hai Chau");

    int luaChon;
    do {
        cout << "\n===== MENU QUAN LY BAI DO =====\n";
        cout << "1. Lien Chieu\n2. Thanh Khe\n3. Son Tra\n4. Hai Chau\n5. Tim xe\n6. Xem so xe moi bai\n7. Thoat\n";
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
                cout << "So xe dang do tai Lien Chieu: " << lienChieu.demSoXeDangDo() << endl;
                cout << "So xe dang do tai Thanh Khe: " << thanhKhe.demSoXeDangDo() << endl;
                cout << "So xe dang do tai Son Tra: " << sonTra.demSoXeDangDo() << endl;
                cout << "So xe dang do tai Hai Chau: " << haiChau.demSoXeDangDo() << endl;
                break;
            }
            case 7:
                cout << "Dang thoat chuong trinh...\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 6);
    return 0;
}
