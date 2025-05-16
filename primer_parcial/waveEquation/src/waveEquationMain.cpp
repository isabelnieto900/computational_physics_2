#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

using namespace std;

// Parámetros globales
const double c = 3.0;         // velocidad de la onda (c = 3)
const double L = 4.0;         // longitud del dominio en x
const int Na = 100;           // malla analítica
const int Nn = 50;           // malla numérica

// Prototipos
double solicitarTiempo();
double analytic(double x, double t);
void solve_fdm(int N, double L, double t_target, vector<double>& y_num);
void guardarDatos(double t, const vector<double>& y_num);
void graficarDatos(double t);

int main() {
    // 1) Solicitar tiempo
    double t = solicitarTiempo();

    // 2) Resolver numéricamente
    vector<double> y_num;
    solve_fdm(Nn, L, t, y_num);

    // 3) Guardar datos en archivos
    guardarDatos(t, y_num);

    // 4) Graficar con Gnuplot
    graficarDatos(t);

    return 0;
}

// Lee tiempo desde consola
double solicitarTiempo() {
    double t;
    cout << "Introduce el tiempo t (en segundos): ";
    cin >> t;
    return t;
}

// Solución analítica
double analytic(double x, double t) {
    return 2.0 * sin(M_PI * x) * cos(c * M_PI * t);
}

// Esquema explícito de diferencias finitas
void solve_fdm(int N, double L, double t_target, vector<double>& y_num) {
    double dx = L / N;
    double dt = dx / c * 0.9;                 // CFL: r = c*dt/dx = 0.9
    int steps = static_cast<int>(t_target / dt);
    dt = t_target / steps;

    y_num.assign(N+1, 0.0);
    vector<double> y_prev(N+1), y_curr(N+1), y_next(N+1);

    // Condición inicial y_t(x,0)=0, y(x,0)=2 sin(pi x)
    for (int i = 0; i <= N; ++i) {
        double x = i * dx;
        y_prev[i] = 2.0 * sin(M_PI * x);
        y_curr[i] = y_prev[i];
    }
    y_prev[0] = y_prev[N] = 0.0;
    y_curr[0] = y_curr[N] = 0.0;

    double r2 = pow(c*dt/dx, 2);
    for (int n = 1; n <= steps; ++n) {
        for (int i = 1; i < N; ++i) {
            y_next[i] = 2*y_curr[i] - y_prev[i]
                      + r2*(y_curr[i+1] - 2*y_curr[i] + y_curr[i-1]);
        }
        y_next[0] = y_next[N] = 0.0;
        y_prev = y_curr;
        y_curr = y_next;
    }
    y_num = y_curr;
}

// Escribe dataA.dat y dataN.dat
void guardarDatos(double t, const vector<double>& y_num) {
    ofstream dataA("dataA.dat");
    for (int i = 0; i <= Na; ++i) {
        double x = L * i / Na;
        dataA << x << "\t" << analytic(x, t) << "\n";
    }
    dataA.close();

    ofstream dataN("dataN.dat");
    for (int i = 0; i <= Nn; ++i) {
        double x = L * i / Nn;
        dataN << x << "\t" << y_num[i] << "\n";
    }
    dataN.close();
}

// Genera script y lo ejecuta
void graficarDatos(double t) {
    ofstream script("script.gnuplot");
    script << "set title sprintf(\"Onda analítica vs numérica: t = %.3f s\", " << t << ")\n";
    script << "set xlabel 'x'\n";
    script << "set ylabel 'y(x,t)'\n";
    script << "set xrange [0:4]\n";
    script << "set yrange [-2.5:2.5]\n";
    script << "set grid\n";
    script << "plot 'dataA.dat' using 1:2 with lines lw 2 lc rgb 'blue' dt 2 title 'Analítica', \
";
    script << "     'dataN.dat' using 1:2 with lines lw 2 lc rgb 'red' title 'Numérica'\n";
    script.close();

    system("gnuplot -persist script.gnuplot");
}
