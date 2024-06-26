#ifndef MATRIZ_DISPERSA_H
#define MATRIZ_DISPERSA_H

#include <iostream>
#include "avion.h"
#include "piloto.h"

using namespace std;

// Definición del nodo de la lista
struct Nodo {
    string vuelo;
    string ciudad_destino;
    string numero_de_id;
    Nodo* siguiente;

    Nodo(string v, string cd, string id)
        : vuelo(v), ciudad_destino(cd), numero_de_id(id), siguiente(nullptr) {}
};

// Definición de la lista enlazada
class ListaEnlazada {
public:
    Nodo* cabeza;

    ListaEnlazada() : cabeza(nullptr) {}

    // Agregar un nodo a la lista
    void agregar(string vuelo, string ciudad_destino, string numero_de_id) {
        Nodo* nuevo = new Nodo(vuelo, ciudad_destino, numero_de_id);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    // Imprimir la lista
    void imprimir() {
        Nodo* actual = cabeza;
        while (actual != nullptr) {
            cout << "Vuelo: " << actual->vuelo << ", Ciudad Destino: " << actual->ciudad_destino 
                    << ", Numero de Registro: " << actual->numero_de_id << endl;
            actual = actual->siguiente;
        }
    }
};

#endif // MATRIZ_DISPERSA_H