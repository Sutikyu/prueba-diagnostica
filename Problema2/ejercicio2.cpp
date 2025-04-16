/*
2) Dado un número entero no negativo n, a) genere los coeficientes del polinomio (x+1)n, muestre
   el resultado de polinomio y b) muestre por pasos el cálculo para x dado, f(x) = (x+1)n, según el
   polinomio generado.
*/

#include <iostream>
#include <vector>
#include <cmath>
using namespace std;

// Función que construye el triángulo de Pascal hasta la fila n (inclusive)
vector<long long> construirTrianguloPascal(int nivel) {
    vector<vector<long long>> triangulo(nivel + 1);

    for (int fila = 0; fila <= nivel; fila++) {
        triangulo[fila].resize(fila + 1);
        triangulo[fila][0] = triangulo[fila][fila] = 1; // Bordes son 1

        for (int col = 1; col < fila; col++) {
            triangulo[fila][col] = triangulo[fila - 1][col - 1] + triangulo[fila - 1][col];
        }
    }

    return triangulo[nivel]; // Retorna la fila n
}

// Función para mostrar el polinomio generado a partir de los coeficientes
void imprimirPolinomioExpandido(int exponente, const vector<long long>& coeficientes) {
    cout << "(x+1)^" << exponente << " = ";

    for (int i = exponente; i >= 0; i--) {
        long long c = coeficientes[exponente - i];

        if (i != exponente)
            cout << " + ";

        if (c != 1 || i == 0)
            cout << c;

        if (i >= 2)
            cout << "x^" << i;
        else if (i == 1)
            cout << "x";
    }
    cout << "\n";
}

// Función para evaluar f(x) = (x+1)^n usando los coeficientes del polinomio
void evaluarPolinomioPasoAPaso(int exponente, const vector<long long>& coeficientes, double x) {
    double resultado = 0.0;
    cout << "\nEvaluacion de f(" << x << ") = (x+1)^" << exponente << ":\n";

    for (int i = exponente; i >= 0; i--) {
        long long c = coeficientes[exponente - i];
        double potencia = pow(x, i);
        double termino = c * potencia;

        cout << "Termino para x^" << i << ": ";
        if (c != 1 || i == 0)
            cout << c << " * ";
        cout << "(" << x << ")^" << i << " = " << c << " * " << potencia << " = " << termino << "\n";

        resultado += termino;
    }

    cout << "Resultado final = " << resultado << "\n";
}

// Función principal
int main() {
    int n;
    double x;

    cout << "Ingrese un numero entero no negativo para n: ";
    cin >> n;

    if (n < 0) {
        cout << "El numero debe ser no negativo.\n";
        return 1;
    }

    // Parte A: generar los coeficientes del polinomio usando el triángulo de Pascal
    vector<long long> coeficientes = construirTrianguloPascal(n);

    // Mostrar el polinomio en forma desarrollada
    imprimirPolinomioExpandido(n, coeficientes);

    // Parte B: Evaluar el polinomio en un valor dado
    cout << "\nIngrese un valor para x: ";
    cin >> x;

    evaluarPolinomioPasoAPaso(n, coeficientes, x);

    return 0;
}
