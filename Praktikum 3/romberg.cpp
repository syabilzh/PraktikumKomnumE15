/*
 * ============================================================
 *   INTEGRASI ROMBERG
 *   Metode Numerik - Tugas Praktikum #3
 * ============================================================
 *
 * Cara mengubah fungsi:
 *   Edit bagian "FUNGSI YANG DIINTEGRASIKAN" di bawah,
 *   lalu compile ulang.
 *
 * Compile  : g++ -o romberg romberg.cpp -lm
 * Jalankan : ./romberg
 * ============================================================
 */

#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

/* ============================================================
 *   FUNGSI YANG DIINTEGRASIKAN
 *   Ubah bagian ini untuk mengganti fungsi
 * ============================================================ */

// Contoh fungsi saat ini: f(x) = e^x
// Untuk mengganti, edit isi fungsi f(x) dan NAMA_FUNGSI di bawah.

double f(double x) {
    return exp(x);
}

const string NAMA_FUNGSI = "f(x) = e^x";

/* ============================================================
 *   END FUNGSI
 * ============================================================ */


// Warna terminal (ANSI escape codes)
const string RESET  = "\033[0m";
const string BOLD   = "\033[1m";
const string CYAN   = "\033[36m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string MAGENTA= "\033[35m";
const string RED    = "\033[31m";
const string DIM    = "\033[2m";

// Hitung Trapezoidal dengan n sub-interval
double trapezoidal(double a, double b, int n) {
    double h = (b - a) / n;
    double sum = f(a) + f(b);
    for (int i = 1; i < n; i++) {
        sum += 2.0 * f(a + i * h);
    }
    return (h / 2.0) * sum;
}

// ============================================================
//   ROMBERG INTEGRATION
//   Menggunakan tabel Richardson Extrapolation
// ============================================================
// R[i][j] = estimasi integral pada level i, orde 2(j+1)
// R[i][0] = Trapezoidal dengan 2^i sub-interval
// R[i][j] = (4^j * R[i][j-1] - R[i-1][j-1]) / (4^j - 1)

vector<vector<double>> romberg(double a, double b, int maxLevel, double tol) {
    vector<vector<double>> R(maxLevel + 1, vector<double>(maxLevel + 1, 0.0));

    // Isi kolom pertama: Trapezoidal dengan 2^i pias
    for (int i = 0; i <= maxLevel; i++) {
        int n = 1 << i;  // 2^i
        R[i][0] = trapezoidal(a, b, n);
    }

    // Richardson Extrapolation
    for (int j = 1; j <= maxLevel; j++) {
        for (int i = j; i <= maxLevel; i++) {
            double factor = pow(4.0, j);
            R[i][j] = (factor * R[i][j-1] - R[i-1][j-1]) / (factor - 1.0);
        }
    }

    return R;
}

// Cari level konvergensi (|R[i][i] - R[i-1][i-1]| < tol)
int cariLevelKonvergen(const vector<vector<double>>& R, int maxLevel, double tol) {
    for (int i = 1; i <= maxLevel; i++) {
        if (fabs(R[i][i] - R[i-1][i-1]) < tol) {
            return i;
        }
    }
    return maxLevel;
}

// Hitung error relatif terhadap nilai analitik (jika diketahui)
double errorRelatif(double estimasi, double analitik) {
    if (fabs(analitik) < 1e-15) return 0.0;
    return fabs((analitik - estimasi) / analitik) * 100.0;
}

void cetakGaris(int panjang, char karakter = '-') {
    for (int i = 0; i < panjang; i++) cout << karakter;
    cout << endl;
}

void cetakHeader() {
    cout << endl;
    cout << BOLD << CYAN;
    cetakGaris(60, '=');
    cout << "   INTEGRASI ROMBERG" << endl;
    cout << "   Metode Numerik - Tugas Praktikum #3" << endl;
    cetakGaris(60, '=');
    cout << RESET << endl;
}

void cetakInfoFungsi(double a, double b, int maxLevel, double tol) {
    cout << BOLD << "Konfigurasi:" << RESET << endl;
    cout << "  Fungsi   : " << YELLOW << NAMA_FUNGSI << RESET << endl;
    cout << "  Batas    : [" << a << ", " << b << "]" << endl;
    cout << "  Max level: " << maxLevel << " (maks 2^" << maxLevel 
         << " = " << (1 << maxLevel) << " pias)" << endl;
    cout << "  Toleransi: " << scientific << setprecision(2) << tol << RESET << endl;
    cout << fixed;
    cout << endl;
}

// Cetak tabel Romberg lengkap
void cetakTabelRomberg(const vector<vector<double>>& R, int maxLevel, int levelKonvergen) {
    cout << BOLD << CYAN << "Tabel Romberg (Richardson Extrapolation):" << RESET << endl;
    cout << DIM << "  R[i][j]: i = level Trapezoidal, j = orde ekstrapolasi" << RESET << endl;
    cout << endl;

    // Header kolom
    cout << "  " << setw(6) << "Level" << " | " << setw(5) << "n pias";
    for (int j = 0; j <= maxLevel; j++) {
        cout << " | " << setw(14) << ("O(" + to_string(j) + ")");
    }
    cout << endl;
    cetakGaris(70 + maxLevel * 17);

    // Baris tabel
    for (int i = 0; i <= maxLevel; i++) {
        int n = 1 << i;
        cout << "  " << setw(6) << i << " | " << setw(6) << n;

        for (int j = 0; j <= maxLevel; j++) {
            if (j > i) {
                cout << " |               ";
            } else {
                // Tandai diagonal (estimasi terbaik tiap level)
                bool isDiagonal = (j == i);
                bool isKonvergen = (i == levelKonvergen && j == levelKonvergen);

                if (isKonvergen) {
                    cout << " | " << BOLD << GREEN;
                    cout << setw(14) << fixed << setprecision(8) << R[i][j];
                    cout << RESET;
                } else if (isDiagonal) {
                    cout << " | " << YELLOW;
                    cout << setw(14) << fixed << setprecision(8) << R[i][j];
                    cout << RESET;
                } else {
                    cout << " | " << setw(14) << fixed << setprecision(8) << R[i][j];
                }
            }
        }
        cout << endl;
    }

    cout << endl;
    cout << DIM << "  " << YELLOW << "Kuning" << RESET << DIM 
         << " = diagonal (estimasi terbaik per level)" << RESET << endl;
    cout << DIM << "  " << GREEN << "Hijau" << RESET << DIM 
         << "  = level konvergen" << RESET << endl;
    cout << endl;
}

// Cetak perbandingan Trapezoidal vs Romberg
void cetakPerbandingan(const vector<vector<double>>& R, int maxLevel, 
                        int levelKonvergen, double nilaiAnalitik, bool adaAnalitik) {
    cout << BOLD << CYAN << "Perbandingan Konvergensi:" << RESET << endl;
    cetakGaris(70);
    cout << setw(8) << "Level" 
         << setw(6) << "n" 
         << setw(18) << "Trapezoidal"
         << setw(18) << "Romberg";
    if (adaAnalitik) {
        cout << setw(14) << "Err Trap(%)"
             << setw(14) << "Err Romberg(%)";
    }
    cout << endl;
    cetakGaris(70);

    for (int i = 0; i <= maxLevel; i++) {
        int n = 1 << i;
        bool isKonvergen = (i == levelKonvergen);

        if (isKonvergen) cout << BOLD << GREEN;
        
        cout << setw(8) << i
             << setw(6) << n
             << setw(18) << fixed << setprecision(8) << R[i][0]
             << setw(18) << fixed << setprecision(8) << R[i][i];

        if (adaAnalitik) {
            cout << setw(14) << fixed << setprecision(4) << errorRelatif(R[i][0], nilaiAnalitik)
                 << setw(14) << fixed << setprecision(4) << errorRelatif(R[i][i], nilaiAnalitik);
        }

        if (isKonvergen) cout << "  << KONVERGEN" << RESET;
        cout << endl;
    }
    cetakGaris(70);
}

int main() {
    // ============================
    //   PARAMETER INPUT
    // ============================
    double a, b;
    int maxLevel;
    double tol;
    double nilaiAnalitik;
    bool adaAnalitik;

    cetakHeader();

    cout << BOLD << "Masukkan parameter integrasi:" << RESET << endl;
    
    cout << "  Batas bawah (a) = ";
    cin >> a;
    
    cout << "  Batas atas  (b) = ";
    cin >> b;

    cout << "  Max level Romberg (contoh: 5 berarti maks 2^5=32 pias) = ";
    cin >> maxLevel;
    if (maxLevel < 1) maxLevel = 1;
    if (maxLevel > 10) maxLevel = 10;  // batasi agar tabel tetap terbaca

    cout << "  Toleransi error (contoh: 1e-6) = ";
    cin >> tol;

    cout << "  Ada nilai analitik? (1=ya, 0=tidak) : ";
    cin >> adaAnalitik;
    if (adaAnalitik) {
        cout << "  Nilai analitik = ";
        cin >> nilaiAnalitik;
    }

    // ============================
    //   HITUNG
    // ============================
    cout << endl;
    cetakInfoFungsi(a, b, maxLevel, tol);

    vector<vector<double>> R = romberg(a, b, maxLevel, tol);
    int levelKonvergen = cariLevelKonvergen(R, maxLevel, tol);

    // ============================
    //   OUTPUT
    // ============================

    // Tabel Romberg hanya tampil kalau levelnya tidak terlalu besar
    if (maxLevel <= 6) {
        cetakTabelRomberg(R, maxLevel, levelKonvergen);
    } else {
        cout << YELLOW << "(Tabel detail tidak ditampilkan karena maxLevel > 6)" 
             << RESET << endl << endl;
    }

    cetakPerbandingan(R, maxLevel, levelKonvergen, nilaiAnalitik, adaAnalitik);

    // Hasil akhir
    cout << endl;
    cout << BOLD << CYAN;
    cetakGaris(60, '=');
    cout << "  HASIL AKHIR" << endl;
    cetakGaris(60, '=');
    cout << RESET;

    double hasil = R[levelKonvergen][levelKonvergen];
    cout << "  Fungsi       : " << YELLOW << NAMA_FUNGSI << RESET << endl;
    cout << "  Batas        : [" << a << ", " << b << "]" << endl;
    cout << "  Level konvergen: " << levelKonvergen 
         << " (menggunakan " << (1 << levelKonvergen) << " pias)" << endl;
    cout << "  Hasil Romberg: " << BOLD << GREEN 
         << fixed << setprecision(10) << hasil << RESET << endl;

    if (adaAnalitik) {
        double er = errorRelatif(hasil, nilaiAnalitik);
        cout << "  Nilai analitik : " << fixed << setprecision(10) << nilaiAnalitik << endl;
        cout << "  Error relatif  : " << setprecision(6) << er << "%" << endl;
    }

    cout << endl;

    // Penjelasan singkat mengapa Romberg lebih unggul
    cout << BOLD << "Keunggulan Romberg vs Trapezoidal:" << RESET << endl;
    cout << "  Trapezoidal level 0 (n=1) : " 
         << fixed << setprecision(8) << R[0][0] << endl;
    cout << "  Romberg level " << levelKonvergen 
         << " (n=" << (1 << levelKonvergen) << " pias)  : " 
         << fixed << setprecision(8) << hasil << endl;

    if (adaAnalitik) {
        double errTrap0 = errorRelatif(R[0][0], nilaiAnalitik);
        double errTrapN = errorRelatif(R[levelKonvergen][0], nilaiAnalitik);
        double errRomberg = errorRelatif(hasil, nilaiAnalitik);
        cout << endl;
        cout << "  Trap (1 pias)    error: " << setprecision(4) << errTrap0 << "%" << endl;
        cout << "  Trap (" << (1<<levelKonvergen) << " pias)   error: " 
             << setprecision(4) << errTrapN << "%" << endl;
        cout << "  Romberg final    error: " << BOLD << GREEN 
             << setprecision(6) << errRomberg << "%" << RESET << endl;
        cout << endl;
        cout << DIM << "  --> Romberg mencapai akurasi tinggi dengan pias jauh lebih sedikit" << endl;
        cout << "      dibanding Trapezoidal murni dengan tingkat error setara." << RESET << endl;
    }

    cout << endl;
    return 0;
}