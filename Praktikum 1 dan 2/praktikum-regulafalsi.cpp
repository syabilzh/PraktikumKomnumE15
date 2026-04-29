#include <iostream>
#include <cmath>
#include <fstream>
#include <cstdlib>  // Untuk system()

using namespace std;

// ubah fungsi sesuai kebutuhan
double func(double x) {
    return pow(exp(1.0),-x)-x;  // Contoh fungsi: f(x) = e^(-x) - x (di ppt)
}

// Fungsi untuk menulis data fungsi ke file
void writeDataToFile(double x0, double x1, const char* filename) {
    ofstream file;
    file.open(filename);
    double step = (x1 - x0) / 1000;  // Membagi interval x menjadi 1000 titik
    for (double x = x0; x <= x1; x += step) {
        file << x << " " << func(x) << endl;  // Menulis x dan f(x) ke file
    }
    file.close();
}

// Metode Regula Falsi
double regulaFalsi(double x0, double x1, int digits) 
{
    double x2, prev_x2;
    double tol = pow(10, -digits);
    int iterasi = 2;
    
    do {
        prev_x2 = x1;
        x2 = x1 - (func(x1) * (x1 - x0)) / (func(x1) - func(x0));
        cout << "x" << iterasi++ << " = " << x2 << endl; 

        /*
            UPDATE BATAS
            note:   f(x0) selalu negatif, f(x1) selalu positif
                    batas baru nanti harus nyesuain ini, jangan sampe salah tuker.
        */
        if (func(x2) * func(x0) < 0) // f(x2) * (-) = (-) --> artinya x2 positif (batas kanan baru)
            x1 = x2;
        else 
            x0 = x2; // kalo sebaliknya --> artinya x2 negatif (batas kiri baru)
        
    } while (fabs(x2 - prev_x2) >= tol);  // berhenti karena selisih lebih kecil dari toleransi
    
    return x2;
}

int main() 
{
    double x0, x1;
    int digits;

    cout << "Masukkan nilai x0: ";
    cin >> x0;
    cout << "Masukkan nilai x1: ";
    cin >> x1;
    cout << "Masukkan jumlah digit ketelitian yang diinginkan: ";
    cin >> digits;

    // nulis data graph buat dibaca sm gnuplot
    writeDataToFile(x0, x1, "graph_data.txt");

    // panggil metode Regula Falsi
    double hasil = regulaFalsi(x0, x1, digits);
    cout << "Akar persamaan adalah: " << hasil << endl;

    // buka graph_data.txt pake gnuplot
    system("gnuplot -p -e \"set grid; \
        set style line 1 lc rgb '#808080' lt 1 lw 1; \
        set grid linestyle 1; \
        set grid xtics linestyle 2; \
        set grid ytics linestyle 3; \
        set border 3 linewidth 2 linecolor rgb 'black'; \
        set zeroaxis linestyle 1 lw 2 linecolor rgb 'black'; \
        plot 'graph_data.txt' with lines title 'f(x) = e^(-x) - x'\"");

    // Tunggu input agar terminal tidak langsung tertutup
    system("pause");

    return 0;
}
