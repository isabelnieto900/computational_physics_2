#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std;

double potencial(double x, double y, double R, double V0);
void solicitarDatos(double &R, double &V0, int &nPuntos);
void generarDatos(double R, double V0, int nPuntos);
void generarGrafica();

int main(){
    double R, V0;
    int nPuntos;
    solicitarDatos(R, V0, nPuntos);
    generarDatos(R, V0, nPuntos);
    generarGrafica();
    return 0;
}

double potencial(double x, double y, double R, double V0){
    double r = sqrt(x*x + y*y);
    if(r <= R)
        return V0;
    else
        return V0 * (R / r);
}

void solicitarDatos(double &R, double &V0, int &nPuntos){
    do{
        cout << "Ingrese el radio del círculo (R > 0): ";
        cin >> R;
    } while(R <= 0);
    cout << "Ingrese el valor del potencial en el contorno (V0): ";
    cin >> V0;
    do{
        cout << "Ingrese la cantidad de puntos (nPuntos > 1) en cada eje para la malla: ";
        cin >> nPuntos;
    } while(nPuntos < 2);
}

void generarDatos(double R, double V0, int nPuntos){
    ofstream archivo("potencial-laplace.dat");
    if(!archivo){
        cerr << "Error al abrir el archivo de salida." << endl;
        return;
    }
    double xmin = -2.0 * R, xmax = 2.0 * R, ymin = -2.0 * R, ymax = 2.0 * R;
    double dx = (xmax - xmin) / (nPuntos - 1);
    double dy = (ymax - ymin) / (nPuntos - 1);
    for(int i = 0; i < nPuntos; ++i){
        double x = xmin + i * dx;
        for(int j = 0; j < nPuntos; ++j){
            double y = ymin + j * dy;
            double V = potencial(x, y, R, V0);
            archivo << x << " " << y << " " << V << endl;
        }
        archivo << endl;
    }
    archivo.close();
    cout << "Datos generados en el archivo 'potencial-laplace.dat'." << endl;
}

void generarGrafica(){
    ofstream script("grafica.gnu");
    if(!script){
        cerr << "Error al crear el script de Gnuplot." << endl;
        return;
    }
    script << "set terminal pngcairo enhanced size 800,600\n";
    script << "set output 'potencial_3D.png'\n";
    script << "set pm3d\n";
    script << "set dgrid3d 30,30\n";
    script << "set hidden3d\n";
    script << "set xlabel 'X'\n";
    script << "set ylabel 'Y'\n";
    script << "set zlabel 'V'\n";
    script << "set title 'Potencial eléctrico'\n";
    script << "splot 'potencial-laplace.dat' using 1:2:3 with lines\n";
    script.close();
    system("gnuplot grafica.gnu");
    cout << "Gráfica generada en 'potencial_3D.png'." << endl;
}
