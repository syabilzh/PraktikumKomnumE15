#include <iostream>
#include <cmath>
#include <iomanip>
using namespace std;

// Fungsi yang bisa dipilih user
double f(double x, int pilihan) {
    switch(pilihan) {
        case 1: return x*x - 4;              // x^2 - 4
        case 2: return x*x*x - x - 2;        // x^3 - x - 2
        case 3: return cos(x) - x;           // cos(x) - x
        case 4: return exp(x) - 3*x;         // e^x - 3x
        default: return 0;
    }
}

void secant(int pilihan, double x0, double x1, double tol, int max_iter) {
    double x2;// Variabel untuk menyimpan hasil iterasi berikutnya
    
    cout << fixed << setprecision(6);// Set precision untuk output, agar lebih mudah dibaca, terutama untuk nilai akar yang ditemukan
    
    for(int i = 1; i <= max_iter; i++) {// Iterasi dimulai dari 1 hingga max_iter
        double f_x0 = f(x0, pilihan);// Hitung nilai fungsi di x0
        double f_x1 = f(x1, pilihan);// Hitung nilai fungsi di x1

        if(f_x1 - f_x0 == 0) {
            cout << "Error: pembagian nol!\n";
            return;
        }

        x2 = x1 - f_x1 * (x1 - x0) / (f_x1 - f_x0); // Rumus metode secant untuk menghitung iterasi berikutnya

        cout << "Iterasi " << i << ": x = " << x2 << endl;// Output hasil iterasi saat ini

        if(abs(x2 - x1) < tol) {// Cek apakah hasil iterasi sudah cukup dekat dengan akar yang sebenarnya, jika ya, maka iterasi dihentikan
            cout << "\nAkar ditemukan: " << x2 << endl;
            return;
        }

        x0 = x1;// Update x0 dengan nilai x1 untuk iterasi berikutnya
        x1 = x2;// Update x1 dengan nilai x2 untuk iterasi berikutnya
    }

    cout << "\nTidak konvergen dalam " << max_iter << " iterasi.\n";// Jika iterasi mencapai max_iter tanpa menemukan akar yang cukup dekat, maka output bahwa metode tidak konvergen
}

int main() {
    int pilihan;
    double x0, x1, tol;
    int max_iter;

    cout << "=== METODE SECANT ===\n";
    cout << "Pilih fungsi:\n";
    cout << "1. x^2 - 4\n";
    cout << "2. x^3 - x - 2\n";
    cout << "3. cos(x) - x\n";
    cout << "4. e^x - 3x\n";
    cout << "Pilihan: ";
    cin >> pilihan;

    cout << "Masukkan x0: ";
    cin >> x0;

    cout << "Masukkan x1: ";
    cin >> x1;

    cout << "Masukkan toleransi: ";
    cin >> tol;

    cout << "Masukkan max iterasi: ";
    cin >> max_iter;

    secant(pilihan, x0, x1, tol, max_iter);

    return 0;
}