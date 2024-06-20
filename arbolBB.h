#ifndef ARBOL_BINARIO_H
#define ARBOL_BINARIO_H

#include <iostream>

// Definición del nodo del árbol
template <typename T>
struct NodoBB {
    T dato;
    NodoBB* izquierda;
    NodoBB* derecha;
    
    NodoBB(T valor) : dato(valor), izquierda(nullptr), derecha(nullptr) {}
};

// Definición del árbol binario de búsqueda
template <typename T>
class ArbolBinarioBusqueda {
private:
    NodoBB<T>* raiz;

    // Función auxiliar para insertar un valor en el árbol
    NodoBB<T>* insertarNodo(NodoBB<T>* nodo, T valor) {
        if (nodo == nullptr) {
            return new NodoBB<T>(valor);
        }
        if (valor < nodo->dato) {
            nodo->izquierda = insertarNodo(nodo->izquierda, valor);
        } else if (valor > nodo->dato) {
            nodo->derecha = insertarNodo(nodo->derecha, valor);
        }
        return nodo;
    }

    // Función auxiliar para buscar un valor en el árbol
    bool buscarNodo(NodoBB<T>* nodo, T valor) {
        if (nodo == nullptr) {
            return false;
        }
        if (valor == nodo->dato) {
            return true;
        } else if (valor < nodo->dato) {
            return buscarNodo(nodo->izquierda, valor);
        } else {
            return buscarNodo(nodo->derecha, valor);
        }
    }

    // Función auxiliar para imprimir el árbol en orden
    void imprimirEnOrden(NodoBB<T>* nodo) {
        if (nodo != nullptr) {
            imprimirEnOrden(nodo->izquierda);
            std::cout << nodo->dato << " ";
            imprimirEnOrden(nodo->derecha);
        }
    }

public:
    // Constructor
    ArbolBinarioBusqueda() : raiz(nullptr) {}

    // Función para insertar un valor
    void insertar(T valor) {
        raiz = insertarNodo(raiz, valor);
    }

    // Función para buscar un valor
    bool buscar(T valor) {
        return buscarNodo(raiz, valor);
    }

    // Función para imprimir el árbol en orden
    void imprimir() {
        imprimirEnOrden(raiz);
        std::cout << std::endl;
    }
};

#endif // ARBOL_BINARIO_H