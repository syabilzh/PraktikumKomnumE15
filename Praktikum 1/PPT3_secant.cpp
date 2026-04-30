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

    cout << "=== METODE SECANT (FUNGSI BEBAS) ===\n";
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

    try {
        secant(func, x0, x1, tol, max_iter);
    } catch (exception& e) {
        cout << "Error: " << e.what() << endl;
    }

    return 0;
}
