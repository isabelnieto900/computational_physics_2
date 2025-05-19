/**
 * @file     laplaceEquation.cpp
 * @brief    Implementación de funciones para resolver la ecuación de onda y visualizar resultados con Gnuplot.
 * @author   Isabel Nieto y Camilo Huertas
 * @date     2025-04-27
 * @version  1.0.0
 * @license  MIT
 */
#include "../include/waveEquation.h"

// Lee tiempo desde consola
double solicitarTiempo() {
    double t;
    cout << "\n╔═══════════════════════════════════════════════════╗" << endl;
    cout << "║      RESOLVER LA ECUACIÓN DE ONDA (1D)              ║" << endl;
    cout << "╠═════════════════════════════════════════════════════╣" << endl;
    cout << "║  ∂²y     ∂²y                                        ║" << endl;
    cout << "║  ──── = 9────        0 < x < 4, t > 0               ║" << endl;
    cout << "║  ∂t²     ∂x²                                        ║" << endl;
    cout << "╠═════════════════════════════════════════════════════╣" << endl;
    cout << "║  y(0,t) = y(4,t) = 0         para t ≥ 0             ║" << endl;
    cout << "║                                                     ║" << endl;
    cout << "║  y(x,0) = 2·sen(πx)          para 0 ≤ x ≤ 4         ║" << endl;
    cout << "║                                                     ║" << endl;
    cout << "║  ∂y                                                 ║" << endl;
    cout << "║  ──(x,0) = 0                                        ║" << endl;
    cout << "║  ∂t                                                 ║" << endl;
    cout << "╠═════════════════════════════════════════════════════╣" << endl;
    cout << "║  Este código resuelve numéricamente la ecuación     ║" << endl;
    cout << "║  utilizando el método de diferencias finitas        ║" << endl;
    cout << "╚═════════════════════════════════════════════════════╝" << endl;
    cout << " >> Introduce el tiempo t (en segundos): ";
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
void graficarDatos() {
    string opcion;
    cin.ignore();
    cout << "\n¿Con qué desea visualizar la gráfica? [gnuplot/python]: ";
    getline(cin, opcion); // sin 

    if (opcion == "gnuplot") {
        system("gnuplot -persist scripts/plot_wave.gp");
    } else if (opcion == "python") {
        system("python3 scripts/plot_wave.py");
    } else {
        cout << "❌ Opción no reconocida. Escriba 'gnuplot' o 'python'.\n";
    }
}
int contarLineas(const string& file) {
    ifstream in(file);
    int n = 0;
    string line;
    while (getline(in, line)) ++n;
    return n;
}
void guardarDatosgifA(int Nt) {
    ofstream out("onda_anim.dat");
    for (int j = 0; j <= Nt; ++j) {
        double t = t_max * j / Nt;
        for (int i = 0; i <= Na; ++i) {
            double x = L * i / Na;
            out << t << "\t" << x << "\t" << analytic(x, t) << "\n";
        }
    }
    out.close();
}
void guardarDatosgifN(int Nt) {
    ofstream out("onda_animN.dat");
    vector<double> y_num;
    for (int j = 0; j <= Nt; ++j) {
        double t = t_max * j / Nt;
        solve_fdm(Nn, L, t, y_num);
        for (int i = 0; i <= Nn; ++i) {
            double x = L * i / Nn;
            out << t << "\t" << x << "\t" << y_num[i] << "\n";
        }
    }
    out.close();
}
void generarGif2DA(const string& dataFile, const string& gifName, const string& scriptName, double t_max) {
    int nLines = contarLineas(dataFile);
    int linesPerFrame = Na + 1;
    int frames = nLines / linesPerFrame;

    ofstream script(scriptName);
    script << "set terminal gif animate delay 10 size 800,600\n";
    script << "set output '" << gifName << "'\n";
    script << "set xlabel 'x'\n";
    script << "set ylabel 'y(x,t)'\n";
    script << "set xrange [0:" << L << "]\n";
    script << "set yrange [-2.5:2.5]\n";
    script << "set grid\n";

    for (int i = 0; i < frames; ++i) {
        int start = i * linesPerFrame;
        int end = start + linesPerFrame - 1;
        double t = i * t_max / frames;
        script << "  plot '" << dataFile << "' every ::" << start << "::" << end
               << " using 2:3 with lines lc rgb 'purple' title sprintf('t=%.2f'," << t << ")\n";
    }

    script << "unset output\n";
    script.close();

    system((string("gnuplot ") + scriptName).c_str());
}
void generarGif2DN(const string& dataFile, const string& gifName, const string& scriptName, double t_max) {
    int nLines = contarLineas(dataFile);
    int linesPerFrame = Nn + 1;
    int frames = nLines / linesPerFrame;

    ofstream script(scriptName);
    script << "set terminal gif animate delay 10 size 800,600\n";
    script << "set output '" << gifName << "'\n";
    script << "set xlabel 'x'\n";
    script << "set ylabel 'y(x,t)'\n";
    script << "set xrange [0:" << L << "]\n";
    script << "set yrange [-2.5:2.5]\n";
    script << "set grid\n";

    for (int i = 0; i < frames; ++i) {
        int start = i * linesPerFrame;
        int end = start + linesPerFrame - 1;
        double t = i * t_max / frames;
        script << "plot '" << dataFile << "' every ::" << start << "::" << end
               << " using 2:3 with lines lc rgb 'purple' title sprintf('t=%.2f'," << t << ")\n";
    }
    script << "unset output\n";
    script.close();

    system((string("gnuplot ") + scriptName).c_str());
}
