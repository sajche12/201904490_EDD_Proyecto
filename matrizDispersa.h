#include <iostream>
#include <string>

// Estructura del nodo para representar un elemento no nulo
template <typename T>
struct NodoMatriz {
    int fila;
    int columna;
    T valor;
    NodoMatriz* siguiente;

    NodoMatriz(int f, int c, T v) : fila(f), columna(c), valor(v), siguiente(nullptr) {}
};

// Estructura para representar la matriz dispersa
template <typename T>
struct MatrizDispersa {
    int filas;
    int columnas;
    NodoMatriz<T>* cabeza;

    MatrizDispersa(int f, int c) : filas(f), columnas(c), cabeza(nullptr) {}

    // Función para agregar un elemento a la matriz dispersa
    void agregarElemento(int f, int c, T v) {
        NodoMatriz<T>* nuevoNodo = new NodoMatriz<T>(f, c, v);

        if (cabeza == nullptr) {
            cabeza = nuevoNodo;
        } else {
            NodoMatriz<T>* actual = cabeza;
            while (actual->siguiente != nullptr) {
                actual = actual->siguiente;
            }
            actual->siguiente = nuevoNodo;
        }
    }

    // Funcion buscarFila, recibe como parametro la fila y el valor y retorna un puntero al nodo que contiene el valor
    NodoMatriz<T>* buscarFila(int f, T v) {
        NodoMatriz<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->fila == f && actual->valor == v) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // Funcion buscarColumna, recibe como parametro la columna y el valor y retorna un puntero al nodo que contiene el valor
    NodoMatriz<T>* buscarColumna(int c, T v) {
        NodoMatriz<T>* actual = cabeza;
        while (actual != nullptr) {
            if (actual->columna == c && actual->valor == v) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    // Funcion eliminarElemento, recibe como parametro la fila, columna y valor y elimina el nodo que contiene el valor
    void eliminarElemento(int f, int c, T v) {
        NodoMatriz<T>* actual = cabeza;
        NodoMatriz<T>* anterior = nullptr;
        while (actual != nullptr) {
            if (actual->fila == f && actual->columna == c && actual->valor == v) {
                if (anterior == nullptr) {
                    cabeza = actual->siguiente;
                } else {
                    anterior->siguiente = actual->siguiente;
                }
                delete actual;
                return;
            }
            anterior = actual;
            actual = actual->siguiente;
        }
    }
};