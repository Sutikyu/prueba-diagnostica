/*
1) Dado una cadena C, valide si C se encuentra en notación FEN (Forsyth-Edwards Notation), Forsyth–Edwards
Notation. FEN es un sistema estándar para describir posiciones específicas en partidas de ajedrez, permitiendo reiniciar
el juego desde una posición dada. Desarrollado inicialmente por David Forsyth y ampliado por Steven J. Edwards, FEN se utiliza
en la Notación de Juego Portátil para definir posiciones iniciales distintas a la estándar (Wikipedia, 2025).
*/

/*
K: Rey
Q: Dama
R: Torre
B: Alfil
N: Caballo
P: Peón
*/

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cctype> // Para la funcion isdigit
#include <algorithm> // Para la funcion sort

using namespace std;

// Verifica si un caracter es una pieza valida de ajedrez.
bool esCaracterDePieza(char c) {
    return (c == 'K' || c == 'Q' || c == 'R' || c == 'B' || c == 'N' || c == 'P' ||
            c == 'k' || c == 'q' || c == 'r' || c == 'b' || c == 'n' || c == 'p');
}

// Valida una fila individual de la posicion del tablero FEN.
bool validarFila(const string& fila) {
    int contador = 0;
    for (char c : fila) {
        if (isdigit(c)) {
            contador += c - '0';
        } else if (esCaracterDePieza(c)) {
            contador++;
        } else {
            return false; // Caracter invalido encontrado
        }
    }
    return contador == 8;
}

// Valida la primera parte de la FEN: la posicion del tablero.
bool validarPosicionTablero(const string& posicion) {
    stringstream ss(posicion);
    string fila;
    int numFilas = 0;

    while (getline(ss, fila, '/')) {
        numFilas++;
        if (!validarFila(fila)) {
            cout << "Error en la fila de la posicion del tablero: " << fila << endl;
            return false;
        }
    }

    return numFilas == 8;
}

// Valida la segunda parte de la FEN: el jugador activo.
bool validarJugadorActivo(const string& jugador) {
    return (jugador.length() == 1 && (jugador[0] == 'w' || jugador[0] == 'b'));
}

// Valida la tercera parte de la FEN: los derechos de enroque.
bool validarDerechosEnroque(const string& enroque) {
    if (enroque == "-") {
        return true;
    }

    string validos = "KQkq";
    for (char c : enroque) {
        if (validos.find(c) == string::npos) {
            cout << "Error: Caracter invalido en los derechos de enroque: " << c << endl;
            return false;
        }
    }

    string ordenado = enroque;
    sort(ordenado.begin(), ordenado.end());
    for (size_t i = 0; i < ordenado.length() - 1; ++i) {
        if (ordenado[i] == ordenado[i + 1]) {
            cout << "Error: Caracter duplicado en los derechos de enroque: " << ordenado[i] << endl;
            return false;
        }
    }

    return true;
}

// Valida la cuarta parte de la FEN: el objetivo de peon al paso.
bool validarObjetivoPeonPaso(const string& paso) {
    if (paso == "-") {
        return true;
    }

    if (paso.length() != 2) {
        cout << "Error: El objetivo de peon al paso debe tener 2 caracteres (ej. 'a3' o 'h6')." << endl;
        return false;
    }

    char columna = paso[0];
    char fila = paso[1];

    if (columna < 'a' || columna > 'h') {
        cout << "Error: Columna invalida en el objetivo de peon al paso: " << columna << ". Debe estar entre 'a' y 'h'." << endl;
        return false;
    }

    if (fila != '3' && fila != '6') {
        cout << "Error: Fila invalida en el objetivo de peon al paso: " << fila << ". Debe ser '3' o '6'." << endl;
        return false;
    }

    return true;
}

// Valida la quinta parte de la FEN: el numero de medio movimientos.
bool validarNumeroMedioMovimientos(const string& medioMovimientos) {
    for (char c : medioMovimientos) {
        if (!isdigit(c)) {
            cout << "Error: El numero de medio movimientos debe ser un entero no negativo." << endl;
            return false;
        }
    }
    if (medioMovimientos.empty()) {
        cout << "Error: El numero de medio movimientos no puede estar vacio." << endl;
        return false;
    }
    return true;
}

// Valida la sexta parte de la FEN: el numero de movimientos completos.
bool validarNumeroMovimientosCompletos(const string& movimientosCompletos) {
    for (char c : movimientosCompletos) {
        if (!isdigit(c)) {
            cout << "Error: El numero de movimientos completos debe ser un entero positivo." << endl;
            return false;
        }
    }
    if (movimientosCompletos.empty() || stoi(movimientosCompletos) <= 0) {
        cout << "Error: El numero de movimientos completos debe ser mayor que 0." << endl;
        return false;
    }
    return true;
}

// Funcion principal para validar una cadena FEN completa.
bool validarFEN(const string& fen) {
    stringstream ss_fen(fen);
    string parte;
    vector<string> partes_fen;

    while (getline(ss_fen, parte, ' ')) {
        partes_fen.push_back(parte);
    }

    if (partes_fen.size() != 6) {
        cout << "Error: La cadena FEN debe tener 6 partes separadas por espacios." << endl;
        return false;
    }

    if (!validarPosicionTablero(partes_fen[0])) {
        return false;
    }

    if (!validarJugadorActivo(partes_fen[1])) {
        return false;
    }

    if (!validarDerechosEnroque(partes_fen[2])) {
        return false;
    }

    if (!validarObjetivoPeonPaso(partes_fen[3])) {
        return false;
    }

    if (!validarNumeroMedioMovimientos(partes_fen[4])) {
        return false;
    }

    if (!validarNumeroMovimientosCompletos(partes_fen[5])) {
        return false;
    }

    cout << "La cadena FEN tiene el formato correcto." << endl;
    return true;
}

int main() {
    // Una cadena FEN completamente valida
    string fen_valida = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    cout << "\"" << fen_valida << "\" es valido: " << (validarFEN(fen_valida) ? "Si" : "No") << endl;

    // Ejemplo con error en la Posicion del Tablero (fila corta)
    string fen_error_tablero = "rnbqkbnr/pppppppp/7/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
    cout << "\n\"" << fen_error_tablero << "\" es valido: " << (validarFEN(fen_error_tablero) ? "Si" : "No") << endl;

    // Ejemplo con error en el Jugador Activo (caracter incorrecto)
    string fen_error_jugador = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR W KQkq - 0 1";
    cout << "\n\"" << fen_error_jugador << "\" es valido: " << (validarFEN(fen_error_jugador) ? "Si" : "No") << endl;

    // Ejemplo con error en los Derechos de Enroque (caracter invalido)
    string fen_error_enroque = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KqX - 0 1";
    cout << "\n\"" << fen_error_enroque << "\" es valido: " << (validarFEN(fen_error_enroque) ? "Si" : "No") << endl;

    // Ejemplo con error en el Objetivo de Peon al Paso (fila incorrecta)
    string fen_error_paso = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq e4 0 1";
    cout << "\n\"" << fen_error_paso << "\" es valido: " << (validarFEN(fen_error_paso) ? "Si" : "No") << endl;

    // Ejemplo con error en el Numero de Medio Movimientos (no es numero)
    string fen_error_medio_mov = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - a 1";
    cout << "\n\"" << fen_error_medio_mov << "\" es valido: " << (validarFEN(fen_error_medio_mov) ? "Si" : "No") << endl;

    // Ejemplo con error en el Numero de Movimientos Completos (cero)
    string fen_error_mov_completo = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 0";
    cout << "\n\"" << fen_error_mov_completo << "\" es valido: " << (validarFEN(fen_error_mov_completo) ? "Si" : "No") << endl;

    return 0;
}