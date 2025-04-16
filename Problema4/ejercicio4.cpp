/*
4) Para un programa en un lenguaje L, dado un cadena C escrito en L, muestre y cuente
la ocurrencia de la palabra E en C.
*/

#include <iostream>
#include <string>
#include <vector>

using namespace std;

int main() {
    string cadenaC = "Este es un ejemplo de un programa de ejemplo. El ejemplo es sencillo.";
    string palabraE = "ejemplo";
    vector<int> indicesOcurrencias;
    int contadorOcurrencias = 0;

    // Buscamos la palabra dentro del texto
    for (int i = 0; (i = cadenaC.find(palabraE, i)) != string::npos; ++i) {
        indicesOcurrencias.push_back(i);
        contadorOcurrencias++;
    }

    // Mostramos las ocurrencias y el conteo en pantalla
    cout << "Texto: " << cadenaC << endl;
    cout << "Palabra buscada: " << palabraE << endl;

    if (contadorOcurrencias > 0) {
        cout << "\nLa palabra '" << palabraE << "' se encontro en los siguientes indices:" << endl;
        for (int indice : indicesOcurrencias) {
            cout << "- Indice: " << indice << endl;
        }
        cout << " Total de ocurrencias: " << contadorOcurrencias << endl;
    } else {
        cout << "La palabra '" << palabraE << "' no se encontro en el texto." << endl;
    }

    return 0;
}