/**
 * @file     waveEquation.h
 * @brief    Declaraciones de funciones para resolver la ecuación de Onda y condiciones de frontera.
 * @author   
 * @date     2025-05-18
 * @version  1.0.0
 * @license  MIT
 */

#ifndef WAVEEQUATION_H
#define WAVEEQUATION_H
#define _USE_MATH_DEFINES
#include <cmath>
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cstdlib>
#include <iomanip> // ✅ para setw y setprecision
#include <limits> // ⬅️ Agregar esta línea si no está

using namespace std;
double solicitarTiempo();
double analytic(double x, double t);
void solve_fdm(int N, double L, double t_target, vector<double>& y_num);
void guardarDatos(double t, const vector<double>& y_num);
void graficarDatos();
int contarLineas(const string& file);
void guardarDatosgifA(int Nt);
void guardarDatosgifN(int Nt);
void generarGif2DN(const string& dataFile, const string& gifName, const string& scriptName, double t_max);
void generarGif2DA(const string& dataFile, const string& gifName, const string& scriptName, double t_max);

const double c = 3.0;         // velocidad de la onda (c = 3)
const double L = 4.0;         // longitud del dominio en x
const int Na = 100;           // malla analítica
const int Nn = 10;           // malla numérica
const double t_max = 5.0;     // tiempo máximo fijo para animación


#endif // WAVEEQUATION_H