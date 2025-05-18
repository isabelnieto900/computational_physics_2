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

using namespace std;
double solicitarTiempo();
double analytic(double x, double t);
void solve_fdm(int N, double L, double t_target, vector<double>& y_num);
void guardarDatos(double t, const vector<double>& y_num);
void graficarDatos(double t);


#endif // WAVEEQUATION_H