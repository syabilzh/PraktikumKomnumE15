# Laporan Praktikum 1

## 1. Program Algoritma Pemrosesan dengan Metode Regula Falsi (PPT 2)

### A. Langkah-Langkah
Pada praktikum ini, diminta untuk membuat suatu mengimplementasikan metode Regula Falsi menjadi suatu program yang dapat menampilkan proses iteratif numerik secara lengkap bersama dengan grafik fungsinya.

Kami memutuskan untuk menggunakan bahasa pemrograman `C++` dengan bantuan `gnuplot` untuk output grafiknya.

Pertama-tama, inisialisasi fungsi sesuai kebutuhan. Sebagai contoh, disini kami menggunakan fungsi $f(x) = e^{-x} - x$ yang terdapat di contoh soal PPT.
```C++
// ubah fungsi sesuai kebutuhan
double func(double x) {
    return pow(exp(1.0),-x)-x;  // Contoh fungsi: f(x) = e^(-x) - x (di ppt)
}
```

Selanjutnya, buat fungsi `regulaFalsi` menggunakan rumus iteratif $x_2 = x_1 - (f(x_2)-(x_2-x_1))/(f(x_2)-f(x_1))$ . Fungsi ini berjalan berkali-kali selama $x_2$ masih memenuhi syarat toleransi.
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

Pada fungsi `main`, terima input untuk $x_0$, $x_1$, dan juga batas toleransi yang diinginkan. Kemudian, jalankan `gnuplot` untuk menampilkan grafik.
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

Langkah pertama adalah mendefinisikan fungsi f(x) yang akan dicari akarnya. Karena C++ tidak dapat langsung membaca fungsi dalam bentuk string, maka digunakan parser untuk mengubah string menjadi fungsi matematika.
```C++
// evaluator fungsi dari string
double evaluate(string func_str, double x) {
    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double> expression_t;
    typedef exprtk::parser<double> parser_t;

    double var_x = x;// variabel x yang akan digunakan dalam ekspresi

    // buat symbol table dan daftarkan variabel serta fungsi yang diperlukan
    symbol_table_t symbol_table;
    symbol_table.add_variable("x", var_x);
    symbol_table.add_constants();

    // buat ekspresi dan daftarkan symbol table
    expression_t expression;
    expression.register_symbol_table(symbol_table);

    // buat parser dan kompilasi ekspresi dari string
    parser_t parser;
    if (!parser.compile(func_str, expression)) {
        throw runtime_error("Fungsi tidak valid!");
    }

    return expression.value();// evaluasi ekspresi dengan nilai x yang diberikan
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
5. Perbarui nilai:
    - $x_0 = x_1$,
    - $x_1 = x_2$
6. Ulangi hingga konvergen atau mencapai batas iterasi
    

Lalu pada fungsi `main`, program menerima input dari pengguna berupa:

1. Fungsi $f(x)$
2. Nilai awal $x_0$ dan $x_1$
3. Nilai toleransi
4. Maksimum iterasi
    
```C++
int main() {
    string func;
    double x0, x1, tol;
    int max_iter;

    cout << "=== METODE SECANT ===\n";
    cout << "Masukkan fungsi f(x): ";
    getline(cin, func);

    cout << "Masukkan x0: ";
    cin >> x0;

    cout << "Masukkan x1: ";
    cin >> x1;

    cout << "Masukkan toleransi: ";
    cin >> tol;

    cout << "Masukkan max iterasi: ";
    cin >> max_iter;

    try {// panggil metode secant dengan fungsi yang diberikan
        secant(func, x0, x1, tol, max_iter);
    } catch (exception& e) {// tangani error jika fungsi tidak valid atau terjadi pembagian nol
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
```

### B. Screenshot

<img width="450" height="300" alt="image" src="https://github.com/user-attachments/assets/f47f8816-56ad-46ab-b9ba-631477131ae3" />
&nbsp;
<img width="739" height="373" alt="image" src="https://github.com/user-attachments/assets/f42a591a-c10d-4e1a-9d32-ac2a765853cf" />
&nbsp;
<img width="737" height="324" alt="image" src="https://github.com/user-attachments/assets/31e847ce-0e26-49b3-a1dc-923e14692dd3" />
&nbsp;
<img width="736" height="325" alt="image" src="https://github.com/user-attachments/assets/8361113b-ecad-458e-87a2-1b40f40646a9" />

### C. Full Code

```C++
#include <iostream>
#include <cmath>
#include <string>
#include "exprtk.hpp" // open source library untuk parsing dan evaluasi ekspresi matematika

using namespace std;

// evaluator fungsi dari string
double evaluate(string func_str, double x) {
    typedef exprtk::symbol_table<double> symbol_table_t;
    typedef exprtk::expression<double> expression_t;
    typedef exprtk::parser<double> parser_t;

    double var_x = x;// variabel x yang akan digunakan dalam ekspresi

    // buat symbol table dan daftarkan variabel serta fungsi yang diperlukan
    symbol_table_t symbol_table;
    symbol_table.add_variable("x", var_x);
    symbol_table.add_constants();

    // buat ekspresi dan daftarkan symbol table
    expression_t expression;
    expression.register_symbol_table(symbol_table);

    // buat parser dan kompilasi ekspresi dari string
    parser_t parser;
    if (!parser.compile(func_str, expression)) {
        throw runtime_error("Fungsi tidak valid!");
    }

    return expression.value();// evaluasi ekspresi dengan nilai x yang diberikan
}

// metode secant
void secant(string func, double x0, double x1, double tol, int max_iter) {
    double x2;

    for (int i = 1; i <= max_iter; i++) {
        double f_x0 = evaluate(func, x0);
        double f_x1 = evaluate(func, x1);

        if (f_x1 - f_x0 == 0) {
            cout << "Error: pembagian nol!\n";
            return;
        }

        x2 = x1 - f_x1 * (x1 - x0) / (f_x1 - f_x0);

        cout << "Iterasi " << i << ": x = " << x2 << endl;

        if (abs(x2 - x1) < tol) {
            cout << "\nAkar ditemukan: " << x2 << endl;
            return;
        }

        x0 = x1;
        x1 = x2;
    }

    cout << "\nTidak konvergen.\n";
}

int main() {
    string func;
    double x0, x1, tol;
    int max_iter;

    cout << "=== METODE SECANT ===\n";
    cout << "Masukkan fungsi f(x): ";
    getline(cin, func);

    cout << "Masukkan x0: ";
    cin >> x0;

    cout << "Masukkan x1: ";
    cin >> x1;

    cout << "Masukkan toleransi: ";
    cin >> tol;

    cout << "Masukkan max iterasi: ";
    cin >> max_iter;

    try {// panggil metode secant dengan fungsi yang diberikan
        secant(func, x0, x1, tol, max_iter);
    } catch (exception& e) {// tangani error jika fungsi tidak valid atau terjadi pembagian nol
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
```
