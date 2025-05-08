#ifndef LAPLACE_EQUATION_H
#define LAPLACE_EQUATION_H

#include <vector>
#include <string>

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
                   int& opcionImplementacion, int& opcionGrafica);

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
 * @return Un código de error:
 * - 0: No hay error, todos los datos son válidos.
 * - 1: Error: Al menos uno de los valores de frontera es menor que cero.
 * - 2: Error: Al menos una de las entradas no tiene un formato numérico válido.
 * - 3: Error: El número de divisiones nx o ny no es válido.
 * - 4: Error: El valor de lambda no está en el rango (0, 2].
 * - 5: Error: El criterio de convergencia no es positivo.
 * - 6: Error: La opción de implementación no es válida.
 * - 7: Error: La opción de graficación no es válida.
 */
int VerificarDatos(double fronteraIzquierda, double base, double escalera,
                   int nx, int ny, double lambda, double criterioConvergencia,
                   int opcionImplementacion, int opcionGrafica);

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
                                           int nx, int ny, double lambda, int opcionImplementacion);

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
                           double fronteraIzquierda, double base, double escalera);

/**
 * @brief Genera un gráfico de la solución de la ecuación de Laplace.
 *
 * @param nombreArchivo Nombre del archivo de datos.
 * @param opcionGrafica Opción de la herramienta de graficación (1: Python/Matplotlib, 2: Gnuplot).
 */
void Graficar(const std::string& nombreArchivo, int opcionGrafica);

#endif // LAPLACE_EQUATION_H

