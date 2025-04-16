/*
3. implemente la evaluación de expresiones aritméticas considerando los operadores +,-,*,/ y los
operandos pueden ser ingresados en notación científica (125E25,5E-8 para denotar 12x1025, 5x10-8
respectivamente), la entrada al programa será una cadena de caracteres con la expresión, ejemplo de expresión
a evaluar: (125E10 – 1e15)/5E-85*15.
*/

#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <cctype>
#include <stdexcept>
using namespace std;

// Tipos de tokens posibles: número, operador o paréntesis
enum TipoToken { NUMERO, OPERADOR, PARENTESIS };

// Cada token representa un número o un símbolo (operador o paréntesis)
struct Token {
    TipoToken tipo;
    double valor;
    char simbolo;
};

// Verifica si un carácter es un operador aritmético válido
bool esSimboloOperador(char c) {
    return (c == '+' || c == '-' || c == '*' || c == '/');
}

// Asigna la prioridad (precedencia) a los operadores
int obtenerPrioridadOperador(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

// Paso 1: Convierte la cadena de entrada en una lista de tokens (números, operadores y paréntesis)
vector<Token> extraerTokens(const string &expresion) {
    vector<Token> tokens;
    size_t i = 0;

    while (i < expresion.size()) {
        if (isspace(expresion[i])) {
            i++;
            continue;
        }

        // Detectar número con soporte para signo, punto decimal y notación científica
        if (isdigit(expresion[i]) || expresion[i] == '.' ||
            ((expresion[i] == '+' || expresion[i] == '-') && i + 1 < expresion.size() &&
             (isdigit(expresion[i + 1]) || expresion[i + 1] == '.'))) {

            size_t inicio = i;

            if (expresion[i] == '+' || expresion[i] == '-') {
                i++;
            }

            // Parte entera y/o decimal
            bool punto = false;
            while (i < expresion.size() && (isdigit(expresion[i]) || expresion[i] == '.')) {
                if (expresion[i] == '.') {
                    if (punto) break;
                    punto = true;
                }
                i++;
            }

            // Parte exponencial (por ejemplo E-8 o e+3)
            if (i < expresion.size() && (expresion[i] == 'E' || expresion[i] == 'e')) {
                i++;
                if (i < expresion.size() && (expresion[i] == '+' || expresion[i] == '-')) i++;
                if (i >= expresion.size() || !isdigit(expresion[i]))
                    throw runtime_error("Exponente inválido");
                while (i < expresion.size() && isdigit(expresion[i])) i++;
            }

            string numeroComoTexto = expresion.substr(inicio, i - inicio);
            double numero = stod(numeroComoTexto);

            Token token;
            token.tipo = NUMERO;
            token.valor = numero;
            tokens.push_back(token);
        }

        // Detectar operadores
        else if (esSimboloOperador(expresion[i])) {
            Token token;
            token.tipo = OPERADOR;
            token.simbolo = expresion[i];
            tokens.push_back(token);
            i++;
        }

        // Detectar paréntesis
        else if (expresion[i] == '(' || expresion[i] == ')') {
            Token token;
            token.tipo = PARENTESIS;
            token.simbolo = expresion[i];
            tokens.push_back(token);
            i++;
        }

        // Caracter no reconocido
        else {
            throw runtime_error("Símbolo no reconocido: " + string(1, expresion[i]));
        }
    }

    return tokens;
}

// Paso 2: Convierte la lista de tokens de notación infija a notación postfija
vector<Token> convertirAPostfijo(const vector<Token> &tokens) {
    vector<Token> salida;
    stack<Token> operadores;

    for (const Token &token : tokens) {
        if (token.tipo == NUMERO) {
            salida.push_back(token);
        }
        else if (token.tipo == OPERADOR) {
            while (!operadores.empty() &&
                   operadores.top().tipo == OPERADOR &&
                   obtenerPrioridadOperador(operadores.top().simbolo) >= obtenerPrioridadOperador(token.simbolo)) {
                salida.push_back(operadores.top());
                operadores.pop();
            }
            operadores.push(token);
        }
        else if (token.tipo == PARENTESIS) {
            if (token.simbolo == '(') {
                operadores.push(token);
            } else {
                while (!operadores.empty() && operadores.top().simbolo != '(') {
                    salida.push_back(operadores.top());
                    operadores.pop();
                }
                if (operadores.empty())
                    throw runtime_error("Paréntesis desbalanceados");
                operadores.pop(); // quitar el '('
            }
        }
    }

    while (!operadores.empty()) {
        if (operadores.top().tipo == PARENTESIS)
            throw runtime_error("Paréntesis desbalanceados");
        salida.push_back(operadores.top());
        operadores.pop();
    }

    return salida;
}

// Paso 3: Evalúa la expresión en notación postfija
double resolverPostfija(const vector<Token> &tokens) {
    stack<double> pila;

    for (const Token &token : tokens) {
        if (token.tipo == NUMERO) {
            pila.push(token.valor);
        } else if (token.tipo == OPERADOR) {
            if (pila.size() < 2)
                throw runtime_error("Faltan operandos para el operador");

            double b = pila.top(); pila.pop();
            double a = pila.top(); pila.pop();
            double resultado;

            switch (token.simbolo) {
                case '+': resultado = a + b; break;
                case '-': resultado = a - b; break;
                case '*': resultado = a * b; break;
                case '/':
                    if (b == 0) throw runtime_error("División por cero");
                    resultado = a / b; break;
                default:
                    throw runtime_error("Operador no reconocido");
            }
            pila.push(resultado);
        }
    }

    if (pila.size() != 1)
        throw runtime_error("Expresión mal formada");

    return pila.top();
}

// Función principal
int main() {
    cout << "Ingrese una expresion para evaluar:\n";
    string expresion;
    getline(cin, expresion);

    try {
        vector<Token> tokens = extraerTokens(expresion);
        vector<Token> postfija = convertirAPostfijo(tokens);
        double resultado = resolverPostfija(postfija);
        cout << "\nResultado: " << resultado << "\n";
    } catch (const exception &e) {
        cerr << "Error: " << e.what() << "\n";
    }

    return 0;
}
