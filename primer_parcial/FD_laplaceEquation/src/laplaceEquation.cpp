#include "laplaceEquation.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <iomanip>
#include <sys/stat.h>
#include <unistd.h> // Para mkdir en Linux
#include <cstdlib>
#include <cmath>
#include <Eigen/Dense>

/**
 * @brief Solicita al usuario los parámetros necesarios para resolver la ecuación de Laplace.
 *
 * @param fronteraIzquierda Valor constante para la frontera izquierda.
 * @param base Valor constante para la base.
 * @param escalera Valor constante para la escalera saliente.
 * @param nx Número de divisiones en la dirección x de la malla.
 * @param ny Número de divisiones en la dirección y de la malla.
 * @param lambda Parámetro de sobrerrelajación para el método de Gauss-Seidel.
 * @param criterioConvergencia Porcentaje de error relativo para la convergencia.
 * @param opcionImplementacion Entero que indica la implementación a usar (1 para manual, 2 para Eigen).
 * @param opcionGrafica Entero que indica la herramienta de graficación a usar (1 para Python, 2 para Gnuplot).
 */
void IngresarDatos(double& fronteraIzquierda, double& base, double& escalera,
                   int& nx, int& ny, double& lambda, double& criterioConvergencia,
                   int& opcionImplementacion, int& opcionGrafica) {
    std::cout << "Ingrese los siguientes parámetros:" << std::endl;

    // Función auxiliar para leer doubles con validación
    auto leerDouble = [](const std::string& mensaje, double& valor) {
        while (true) {
            std::cout << mensaje;
            std::cin >> valor;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Entrada no válida. Intente de nuevo.\n";
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }
    };

    // Función auxiliar para leer enteros con validación
    auto leerEntero = [](const std::string& mensaje, int& valor) {
        while (true) {
            std::cout << mensaje;
            std::cin >> valor;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Entrada no válida. Intente de nuevo.\n";
            } else {
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                break;
            }
        }
    };

    leerDouble("Condición de frontera izquierda: ", fronteraIzquierda);
    leerDouble("Condición de frontera base: ", base);
    leerDouble("Condición de frontera escalera: ", escalera);
    leerEntero("Número de divisiones en x (nx): ", nx);
    leerEntero("Número de divisiones en y (ny): ", ny);
    leerDouble("Parámetro de sobrerrelajación (lambda): ", lambda);
    leerDouble("Criterio de convergencia (porcentaje de error): ", criterioConvergencia);

    // Leer opción de implementación
    while (true) {
        leerEntero("Opción de implementación (1: Manual C++, 2: Eigen): ", opcionImplementacion);
        if (opcionImplementacion == 1 || opcionImplementacion == 2) {
            break;
        }
        std::cout << "Opción no válida. Ingrese 1 o 2.\n";
    }

    // Leer opción de graficación
    while (true) {
        leerEntero("Opción de graficación (1: Python/Matplotlib, 2: Gnuplot): ", opcionGrafica);
        if (opcionGrafica == 1 || opcionGrafica == 2) {
            break;
        }
        std::cout << "Opción no válida. Ingrese 1 o 2.\n";
    }
}

/**
 * @brief Verifica si los parámetros ingresados por el usuario son válidos.
 *
 * @param fronteraIzquierda Valor de la frontera izquierda a verificar.
 * @param base Valor de la base a verificar.
 * @param escalera Valor de la escalera saliente a verificar.
 * @param nx Número de divisiones en la dirección x de la malla a verificar.
 * @param ny Número de divisiones en la dirección y de la malla a verificar.
 * @param lambda Parámetro de sobrerrelajación a verificar.
 * @param criterioConvergencia Criterio de convergencia a verificar.
 * @param opcionImplementacion Opción de implementación a verificar.
 * @param opcionGrafica Opción de graficación a verificar.
 * @return Un código de error (0 si no hay error).
 */
int VerificarDatos(double fronteraIzquierda, double base, double escalera,
                   int nx, int ny, double lambda, double criterioConvergencia,
                   int opcionImplementacion, int opcionGrafica) {
    if (fronteraIzquierda < 0 || base < 0 || escalera < 0) {
        std::cerr << "Error: Al menos uno de los valores de frontera es menor que cero." << std::endl;
        return 1;
    }
    if (nx <= 0 || ny <= 0) {
        std::cerr << "Error: El número de divisiones nx y ny debe ser mayor que cero." << std::endl;
        return 3;
    }
    if (lambda <= 0 || lambda > 2) {
        std::cerr << "Error: El valor de lambda debe estar en el rango (0, 2]." << std::endl;
        return 4;
    }
    if (criterioConvergencia <= 0) {
        std::cerr << "Error: El criterio de convergencia debe ser positivo." << std::endl;
        return 5;
    }
    if (opcionImplementacion != 1 && opcionImplementacion != 2) {
        std::cerr << "Error: La opción de implementación no es válida (debe ser 1 o 2)." << std::endl;
        return 6;
    }
    if (opcionGrafica != 1 && opcionGrafica != 2) {
        std::cerr << "Error: La opción de graficación no es válida (debe ser 1 o 2)." << std::endl;
        return 7;
    }
    return 0;
}

/**
 * @brief Resuelve la ecuación de Laplace por diferencias finitas con sobrerrelajación.
 *
 * @param fronteraIzquierda Valor constante de la condición de frontera en el lado izquierdo.
 * @param base Valor constante de la condición de frontera en la base.
 * @param escalera Valor constante de la condición de frontera en la escalera saliente.
 * @param nx Número de divisiones en la dirección x de la malla.
 * @param ny Número de divisiones en la dirección y de la malla.
 * @param lambda Parámetro de sobrerrelajación (0 < lambda <= 2).
 * @param opcionImplementacion Entero que indica la implementación a usar (1 para manual, 2 para Eigen).
 * @return La matriz de la solución.
 */
std::vector<std::vector<double>> SolucionDF(double fronteraIzquierda, double base, double escalera,
                                           int nx, int ny, double lambda, int opcionImplementacion) {
    std::vector<std::vector<double>> solucion_std;

    if (opcionImplementacion == 1) {
        // Implementación manual en C++
        std::vector<std::vector<double>> solucion(ny + 1, std::vector<double>(nx + 1, 0.0));

        // Aplicar condiciones de frontera
        for (int j = 0; j <= ny; ++j) {
            solucion[j][0] = fronteraIzquierda;
        }
        for (int i = 0; i <= nx; ++i) {
            solucion[0][i] = base;
        }
        int escalera_base_j = ny / 3;
        int escalera_altura = ny / 3;
        int escalera_ancho = nx / 4;
        for (int i = escalera_ancho; i <= 2 * escalera_ancho; ++i) {
            solucion[escalera_base_j][i] = escalera;
        }
        for (int j = escalera_base_j; j <= escalera_base_j + escalera_altura; ++j) {
            solucion[j][2 * escalera_ancho] = escalera;
        }
        for (int i = 2 * escalera_ancho; i <= 3 * escalera_ancho; ++i) {
            solucion[escalera_base_j + escalera_altura][i] = escalera;
        }

        double error_max = 1.0;
        double tolerancia = 1e-6;
        int iteracion = 0;
        int max_iteraciones = 10000;
        std::vector<std::vector<double>> solucion_anterior = solucion;

        while (error_max > tolerancia && iteracion < max_iteraciones) {
            error_max = 0.0;
            iteracion++;

            for (int j = 1; j < ny; ++j) {
                for (int i = 1; i < nx; ++i) {
                    if (!((j > escalera_base_j && j < escalera_base_j + escalera_altura + 1) && (i > escalera_ancho && i < 3 * escalera_ancho + 1))) {
                        double u_nuevo = 0.25 * (solucion[j+1][i] + solucion[j-1][i] + solucion[j][i+1] + solucion[j][i-1]);
                        solucion[j][i] = (1 - lambda) * solucion_anterior[j][i] + lambda * u_nuevo;
                        double error_punto = std::abs(solucion[j][i] - solucion_anterior[j][i]);
                        if (error_punto > error_max) {
                            error_max = error_punto;
                        }
                    }
                }
            }
            solucion_anterior = solucion;
        }
        if (error_max <= tolerancia) {
            std::cout << "Solución manual convergió en " << iteracion << " iteraciones. Error máximo: " << error_max << std::endl;
        } else {
            std::cout << "Solución manual no convergió después de " << max_iteraciones << " iteraciones. Error máximo: " << error_max << std::endl;
        }
        solucion_std = solucion;

    } else if (opcionImplementacion == 2) {
        // Implementación con Eigen
        Eigen::MatrixXd solucion_eigen(ny + 1, nx + 1);
        solucion_eigen.setZero();

        // Aplicar condiciones de frontera
        solucion_eigen.col(0).fill(fronteraIzquierda);
        solucion_eigen.row(0).fill(base);
        int escalera_base_j = ny / 3;
        int escalera_altura = ny / 3;
        int escalera_ancho = nx / 4;
        for (int i = escalera_ancho; i <= 2 * escalera_ancho; ++i) {
            solucion_eigen(escalera_base_j, i) = escalera;
        }
        for (int j = escalera_base_j; j <= escalera_base_j + escalera_altura; ++j) {
            solucion_eigen(j, 2 * escalera_ancho) = escalera;
        }
        for (int i = 2 * escalera_ancho; i <= 3 * escalera_ancho; ++i) {
            solucion_eigen(escalera_base_j + escalera_altura, i) = escalera;
        }

        double error_max = 1.0;
        double tolerancia = 1e-6;
        int iteracion = 0;
        int max_iteraciones = 10000;
        Eigen::MatrixXd solucion_anterior_eigen = solucion_eigen;

        while (error_max > tolerancia && iteracion < max_iteraciones) {
            error_max = 0.0;
            iteracion++;

            for (int j = 1; j < ny; ++j) {
                for (int i = 1; i < nx; ++i) {
                   if (!((j > escalera_base_j && j < escalera_base_j + escalera_altura + 1) && (i > escalera_ancho && i < 3 * escalera_ancho + 1))) {
                        double u_nuevo = 0.25 * (solucion_eigen(j+1,i) + solucion_eigen(j-1,i) + solucion_eigen(j,i+1) + solucion_eigen(j,i-1));
                        solucion_eigen(j,i) = (1 - lambda) * solucion_anterior_eigen(j,i) + lambda * u_nuevo;
                        double error_punto = std::abs(solucion_eigen(j,i) - solucion_anterior_eigen(j,i));
                        if (error_punto > error_max) {
                            error_max = error_punto;
                        }
                    }
                }
            }
            solucion_anterior_eigen = solucion_eigen;
        }

        if (error_max <= tolerancia) {
            std::cout << "Solución Eigen convergió en " << iteracion << " iteraciones. Error máximo: " << error_max << std::endl;
        } else {
            std::cout << "Solución Eigen no convergió después de " << max_iteraciones << " iteraciones. Error máximo: " << error_max << std::endl;
        }

        solucion_std.resize(ny + 1);
        for (int j = 0; j <= ny; ++j) {
            solucion_std[j].resize(nx + 1);
            for (int i = 0; i <= nx; ++i) {
                solucion_std[j][i] = solucion_eigen(j, i);
            }
        }
    } else {
        std::cerr << "Opción de implementación no válida." << std::endl;
        return std::vector<std::vector<double>>();
    }

    return solucion_std;
}

/**
 * @brief Genera un archivo con los datos de la solución de la ecuación de Laplace.
 *
 * @param solucion La matriz bidimensional que contiene los valores de la solución.
 * @param nx Número de divisiones en la dirección x de la malla.
 * @param ny Número de divisiones en la dirección y de la malla.
 * @param lambda Parámetro de sobrerrelajación utilizado.
 * @param fronteraIzquierda Valor de la frontera izquierda.
 * @param base Valor de la base.
 * @param escalera Valor de la escalera saliente.
 * @return El nombre del archivo generado.
 */
std::string GenerarDatos(const std::vector<std::vector<double>>& solucion,
                           int nx, int ny, double lambda,
                           double fronteraIzquierda, double base, double escalera) {
    // Obtener la fecha y hora actual
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&now_c), "%Y%m%d_%H%M%S");
    std::string timestamp = ss.str();

    // Crear el nombre del archivo
    std::string dirname = "generate_files";
    std::string filename = dirname + "/solucion_" + timestamp +
                           "_nx" + std::to_string(nx) +
                           "_ny" + std::to_string(ny) +
                           "_lambda" + std::to_string(lambda) +
                           "_fi" + std::to_string(static_cast<int>(fronteraIzquierda)) +
                           "_b" + std::to_string(static_cast<int>(base)) +
                           "_e" + std::to_string(static_cast<int>(escalera)) +
                           ".dat";

    // Crear el directorio si no existe
    int dir_result = mkdir(dirname.c_str(), 0777); // Usar mkdir para Linux
    if (dir_result != 0 && errno != EEXIST) {
        std::cerr << "Error al crear el directorio " << dirname << std::endl;
        return "";
    }

    // Abrir el archivo y escribir los datos
    std::ofstream outputFile(filename);
    if (outputFile.is_open()) {
        for (const auto& row : solucion) {
            for (double value : row) {
                outputFile << value << " ";
            }
            outputFile << std::endl;
        }
        outputFile.close();
        std::cout << "Datos de la solución guardados en: " << filename << std::endl;
    } else {
        std::cerr << "No se pudo abrir el archivo: " << filename << std::endl;
        return "";
    }

    return filename;
}

/**
 * @brief Genera un gráfico de la solución de la ecuación de Laplace.
 *
 * @param nombreArchivo Nombre del archivo de datos.
 * @param opcionGrafica Opción de la herramienta de graficación (1: Python/Matplotlib, 2: Gnuplot).
 */
void Graficar(const std::string& nombreArchivo, int opcionGrafica) {
    std::string comando;
    int resultado;
    std::string graph_filename; // Para guardar el nombre del archivo del gráfico

    if (opcionGrafica == 1) {
        // Ejecutar script de Python
        comando = "python scripts/plot_laplace.py " + nombreArchivo;
        std::cout << "Ejecutando: " << comando << std::endl;
        resultado = system(comando.c_str());
        graph_filename = "graph/solucion_" + std::string(nombreArchivo) + ".png"; //Nombre del archivo
        if (resultado != 0) {
            std::cerr << "Error al ejecutar el script de Python. Código de salida: " << resultado << std::endl;
        } else {
            std::cout << "Gráfico guardado en: " << graph_filename << std::endl; // Mostrar el nombre del archivo
        }
    } else if (opcionGrafica == 2) {
        // Ejecutar script de Gnuplot
        comando = "gnuplot -p -e \"set terminal png; set output 'graph/solucion.png'; splot '" + nombreArchivo + "' with pm3d\"";
        std::cout << "Ejecutando: " << comando << std::endl;
        resultado = system(comando.c_str());
        graph_filename = "graph/solucion.png";
        if (resultado != 0) {
            std::cerr << "Error al ejecutar Gnuplot. Código de salida: " << resultado << std::endl;
        } else {
            std::cout << "Gráfico guardado en: " << graph_filename << std::endl; // Mostrar el nombre del archivo
        }
    } else {
        std::cerr << "Opción de graficación no válida." << std::endl;
    }
}



