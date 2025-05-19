#include "../include/waveEquation.h"

int main() {
    //tiempo máximo fijo para animación
    // 1) Solicitar tiempo
    double t = solicitarTiempo();

    // 2) Resolver numéricamente
    vector<double> y_num;
    solve_fdm(Nn, L, t, y_num);

    // 3) Guardar datos en archivos
    guardarDatos(t, y_num);

    // 4) Graficar con Gnuplot
    graficarDatos();
    
    int Nt = 50; // número de fotogramas

    // Generar archivo con todos los tiempos
    guardarDatosgifA(Nt);
    guardarDatosgifN(Nt);

    // Crear GIF animado 2D
    generarGif2DA("onda_anim.dat", "onda_evolucion.gif", "animacion2D.gnu", t_max);
    generarGif2DN("onda_animN.dat", "onda_evolucionN.gif", "animacion2DN.gnu", t_max);
    

    return 0;
}

