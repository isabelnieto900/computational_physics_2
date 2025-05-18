#include "../include/waveEquation.h"

// Prototipos

int main() {
    // Parámetros globales
    //const double c = 3.0;         // velocidad de la onda (c = 3)
    const double L = 4.0;         // longitud del dominio en x
    //const int Na = 100;           // malla analítica
    const int Nn = 50;           // malla numérica
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

