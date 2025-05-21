# PBL1
#include <iostream>
#include <string>
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

    Car() {
        LongTerm = false;
        isParked = false;
    }
};

class BaiDo {
private:
    string tenBai;
    Car cars[20];
    int soXeHienTai;

public:
    BaiDo(string name = "") {
        tenBai = name;
        soXeHienTai = 0;
    }

    string getTenBai() {
        return tenBai;
    }

    bool themXe(string bienSo, Timee entryTime) {
        if (soXeHienTai >= 20) {
            cout << "Bai do da day.\n";
            return false;
        }
        cars[soXeHienTai].licensePlate = bienSo;
        cars[soXeHienTai].entryTime = entryTime;
        cars[soXeHienTai].isParked = true;
        soXeHienTai++;
        cout << "Da them xe vao bai: " << bienSo << endl;
        return true;
    }

    bool xuatXe(string bienSo, Timee exitTime) {
        for (int i = 0; i < soXeHienTai; i++) {
            if (cars[i].licensePlate == bienSo && cars[i].isParked) {
                cars[i].exitTime = exitTime;
                cars[i].isParked = false;
                cout << "Xe " << bienSo << " da xuat khoi bai do.\n";
                return true;
            }
        }
        cout << "Khong tim thay xe trong bai.\n";
        return false;
    }

    void tinhPhi(string bienSo) {
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

                cout << "Phi cho xe " << bienSo << ": " << phi << " VND\n";
                return;
            }
        }
        cout << "Xe khong ton tai trong bai.\n";
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
    void xemDanhsachXe () {
        if (soLuongXe == 0) {
            cout << "Khong co xe nao dang do trong bai" << tenBai << endl;
            return;
        }
        cout << "Danh sach xe dang do tai bai " <<tenBai <<":\n";
        for(int i = 0; i < soLuongXe; i++) {
            cout << i + 1 << "." << bienSoXe[i];
            if (dangKyDaiHan[i]) {
                cout << "Dang ky dai han";
            }
            cout << endl;
        }
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

    void menu() {
        int luaChon;
        do {
            cout << "\n--- Quan ly bai: " << tenBai << " ---\n";
            cout << "1. Them xe\n";
            cout << "2. Xuat xe\n";
            cout << "3. Tinh phi\n";
            cout << "4. Dang ky thue dai han\n";
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
        cout << "1. Lien Chieu\n2. Thanh Khe\n3. Son Tra\n4. Hai Chau\n5. Tim xe\n6. Thoat\n";
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
            case 6:
                cout << "Dang thoat chuong trinh...\n";
                break;
            default:
                cout << "Lua chon khong hop le!\n";
        }
    } while (luaChon != 6);

    return 0;
}
