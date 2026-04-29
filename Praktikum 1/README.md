# Laporan Praktikum 1

## 1. Program Algoritma Pemrosesan dengan Metode Regula Falsi (PPT 2)

### A. Langkah-Langkah
Pada praktikum ini, diminta untuk membuat suatu mengimplementasikan metode Regula Falsi menjadi suatu program yang dapat menampilkan proses iteratif numerik secara lengkap bersama dengan grafik fungsinya.

Kami memutuskan untuk menggunakan bahasa pemrograman `C++` dengan bantuan `gnuplot` untuk output grafiknya.

Pertama-tama, inisialisasi fungsi sesuai kebutuhan. Sebagai contoh, disini kami menggunakan fungsi $f(x) = e^(-x) - x$ yang terdapat di contoh soal PPT.
```C++
// ubah fungsi sesuai kebutuhan
double func(double x) {
    return pow(exp(1.0),-x)-x;  // Contoh fungsi: f(x) = e^(-x) - x (di ppt)
}
```

Selanjutnya, buat fungsi `regulaFalsi` menggunakan rumus iteratif $x2 = x1 - (f(x2)-(x2-x1))/(f(x2)-f(x1))$ . Fungsi ini berjalan berkali-kali selama $x2$ masih memenuhi syarat toleransi.
```C++
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
```

Fungsi `writeDataToFile` berfungsi untuk menyimpan data koordinat ke dalam `graph_data.txt` yang nantinya akan dibaca oleh `gnuplot`.
```C++
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
```

Pada fungsi `main`, terima input untuk $x0$, $x1$, dan juga batas toleransi yang diinginkan. Kemudian, jalankan `gnuplot` untuk menampilkan grafik.
```C++
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
```

### B. Screenshot
<img width="1916" height="1090" alt="image" src="https://github.com/user-attachments/assets/a8cbf868-2bff-4e03-bc17-311e96fb9033" />

### C. Full Code
```C++
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
```

---

## 2. Program Algoritma Pemrosesan dengan Metode Secant

### A. Langkah-Langkah
Pada praktikum ini, diminta untuk mengimplementasikan metode numerik Secant ke dalam sebuah program yang mampu menampilkan proses iterasi hingga menemukan akar persamaan.

Langkah pertama adalah mendefinisikan fungsi f(x) yang akan dicari akarnya. Dalam program ini, pengguna dapat memilih beberapa fungsi yang telah disediakan melalui menu.
```C++
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
```

Selanjutnya, Metode Secant menggunakan dua tebakan awal $x_0$ dan $x_1$, lalu menghitung nilai pendekatan berikutnya menggunakan rumus:

$x_{n+1} = x_n - f(x_n) \frac{x_{n-x} - x_{n-1}}{f(x_n)-f(x_{n-1})}$

```C++
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
```
Mekanisme dalam metode Secant berjalan dengan langkah sebagai berikut:
    1. Hitung nilai $f(x_0)$ dan $f(x_1)$
    2. Gunakan rumus Secant untuk mendapatkan x
    3. Tampilkan hasil iterasi
    4. Periksa: Jika $|x_2 - x_1| < toleransi$, maka iterasi dihentikan
​    5. Perbarui nilai: 
        - $x_0 = x_1$,
        - $x_1 = x_2$
    6. Ulangi hingga konvergen atau mencapai batas iterasi
    

Lalu pada fungsi `main`, program menerima input dari pengguna berupa:
    1. Pilihan fungsi
    2. Nilai awal $x_0$ dan $x_1$
    3. Nilai toleransi
    4. Maksimum iterasi
    
```C++
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
```

### B. Screenshot

<img width="450" height="300" alt="image" src="https://github.com/user-attachments/assets/98ef610a-7b39-4ffb-bcc8-31cf3d9d08c2" />
&nbsp;
<img width="450" height="300" alt="image" src="https://github.com/user-attachments/assets/13626a98-23f6-48ca-842d-f9c72a57bca8" />
&nbsp;
<img width="450" height="300" alt="image" src="https://github.com/user-attachments/assets/4345b974-ba0a-49c8-9470-5ce5915880e3" />
&nbsp;
<img width="450" height="300" alt="image" src="https://github.com/user-attachments/assets/40326199-4549-4c9a-b6a0-4a17ed1649f1" />


### C. Full Code

```C++
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
```
