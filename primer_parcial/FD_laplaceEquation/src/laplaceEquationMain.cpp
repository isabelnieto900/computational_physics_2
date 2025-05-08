#include "laplaceEquation.h"
#include <iostream>
#include <string>

/**
 * @brief Función principal del programa para resolver la ecuación de Laplace.
 *
 * @return Código de salida del programa.
 */
int main() {
    double fronteraIzquierda, base, escalera, lambda, criterioConvergencia;
    int nx, ny, opcionImplementacion, opcionGrafica;
    std::string nombreArchivo;

    // 1. Obtener los datos de entrada del usuario
    IngresarDatos(fronteraIzquierda, base, escalera, nx, ny, lambda, criterioConvergencia, opcionImplementacion, opcionGrafica);

    // 2. Verificar los datos ingresados
    int codigoError = VerificarDatos(fronteraIzquierda, base, escalera, nx, ny, lambda, criterioConvergencia, opcionImplementacion, opcionGrafica);
    if (codigoError != 0) {
        std::cerr << "Error en los datos de entrada. El programa terminará." << std::endl;
        return codigoError;
    }

    // 3. Resolver la ecuación de Laplace
    std::cout << "Resolviendo la ecuación de Laplace..." << std::endl;
    std::vector<std::vector<double>> solucion = SolucionDF(fronteraIzquierda, base, escalera, nx, ny, lambda, opcionImplementacion);

     if (solucion.empty())
    {
        std::cerr << "Error al resolver la ecuación de Laplace. El programa terminará." << std::endl;
        return 1;
    }

    // 4. Generar el archivo de datos
    std::cout << "Generando archivo de datos..." << std::endl;
    nombreArchivo = GenerarDatos(solucion, nx, ny, lambda, fronteraIzquierda, base, escalera);
    if (nombreArchivo == "") {
        std::cerr << "Error al generar el archivo de datos. El programa terminará." << std::endl;
        return 1;
    }

    // 5. Graficar la solución
    std::cout << "Graficando la solución..." << std::endl;
    Graficar(nombreArchivo, opcionGrafica);

    std::cout << "Programa terminado." << std::endl;
    return 0;
}

