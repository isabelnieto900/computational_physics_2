/**
 * @file     laplaceEquation.cpp
 * @brief    Implementación de funciones para resolver la ecuación de onda y visualizar resultados con Gnuplot.
 * @author   -
 * @date     2025-04-27
 * @version  1.0.0
 * @license  MIT
 */
#include "../include/waveEquation.h"
const double c = 3.0;         // velocidad de la onda (c = 3)
const double L = 4.0;         // longitud del dominio en x
const int Na = 100;           // malla analítica
const int Nn = 50;           // malla numérica

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
// system("rm script.gnuplot dataA.dat dataN.dat");
