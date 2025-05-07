#include <iostream>
#include <fstream>
#include <cmath>
#include <string>
#include <cstdlib>

using namespace std;

const double PI = 3.141592653589793;
const int N = 100;

void solicitarDatos(double &R, double &lambda, int &malla);
bool validarDatos(double R, double lambda, int malla);
double trapecio(double x, double y, double R);
void calcularPotencial(double R, double** potencial, int malla);
void guardarDatos(double** potencial, double R, int malla);
void GenerarGrafica();

int main(){
    double R, lambda;
    int malla;
    solicitarDatos(R, lambda, malla);
    if(!validarDatos(R, lambda, malla)){
        cout<<"Datos invalidos. Terminando el programa."<<endl;
        return 1;
    }
    double** potencial = new double*[malla];
    for(int i = 0; i < malla; i++){
        potencial[i] = new double[malla];
    }
    calcularPotencial(R, potencial, malla);
    guardarDatos(potencial, R, malla);
    GenerarGrafica();
    for(int i = 0; i < malla; i++){
        delete [] potencial[i];
    }
    delete [] potencial;
    return 0;
}

void solicitarDatos(double &R, double &lambda, int &malla){
    cout << "Ingrese el radio del circulo (R): ";
    cin >> R;
    cout << "Ingrese la densidad lineal de carga (lambda): ";
    cin >> lambda;
    cout << "Ingrese el numero de puntos en la malla: ";
    cin >> malla;
}

bool validarDatos(double R, double lambda, int malla){
    if(R <= 0.0 || lambda <= 0.0 || malla <= 0){
        cout << "Error: El radio, la densidad de carga y el numero de puntos deben ser positivos." << endl;
        return false;
    }
    return true;
}

double trapecio(double x, double y, double R){
    double h = (2 * PI) / N;
    double suma = 0.0;
    for(int i = 0; i <= N; i++){
        double theta = i * h;
        double r = sqrt(pow(x - R * cos(theta), 2) + pow(y - R * sin(theta), 2));
        double f = 1.0 / r;
        if(i == 0 || i == N)
            suma += f;
        else
            suma += 2 * f;
    }
    return R * h * suma / 2.0;
}

void calcularPotencial(double R, double** potencial, int malla){
    double paso = (4 * R) / (malla - 1);
    for(int i = 0; i < malla; i++){
        for(int j = 0; j < malla; j++){
            double x = -2 * R + i * paso;
            double y = -2 * R + j * paso;
            potencial[i][j] = trapecio(x, y, R);
        }
    }
}

void guardarDatos(double** potencial, double R, int malla){
    ofstream archivo("potencial.dat");
    double paso = (4 * R) / (malla - 1);
    for(int i = 0; i < malla; i++){
        for(int j = 0; j < malla; j++){
            double x = -2 * R + i * paso;
            double y = -2 * R + j * paso;
            archivo << x << " " << y << " " << potencial[i][j] << endl;
        }
        archivo << endl;
    }
    archivo.close();
}

void GenerarGrafica(){
    ofstream script("grafica.gnu");
    script << "set terminal pngcairo enhanced size 800,600\n";
    script << "set output 'potencial_3D.png'\n";
    script << "set pm3d\n";
    script << "set dgrid3d 30,30\n";
    script << "set hidden3d\n";
    script << "set xlabel 'X'\n";
    script << "set ylabel 'Y'\n";
    script << "set zlabel 'V(x,y)'\n";
    script << "set title 'Potencial electrico de un anillo con densidad lineal de carga'\n";
    script << "set view 60, 30\n";
    script << "splot 'potencial.dat' using 1:2:3 with lines\n";
    script.close();
    system("gnuplot grafica.gnu");
    cout << "Grafica generada en 'potencial_3D.png'." << endl;
}
