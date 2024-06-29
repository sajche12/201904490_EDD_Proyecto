#ifndef LISTA_CIRCULAR_DOBLE_H
#define LISTA_CIRCULAR_DOBLE_H

#include "avion.h"
#include <iostream>
#include <string>
using namespace std;

class Nodo {
public:
    Avion avion;
    Nodo* siguiente;
    Nodo* anterior;

    Nodo(Avion avion) : avion(avion), siguiente(nullptr), anterior(nullptr) {}
};

class ListaCircularDoble {
private:
    Nodo* cabeza;
    Nodo* cola;

public:
    ListaCircularDoble() : cabeza(nullptr), cola(nullptr) {}

    bool estaVacia() const {
        return cabeza == nullptr;
    }

    void agregarAvion(const Avion& avion) {
        Nodo* nuevoNodo = new Nodo(avion);
        if (estaVacia()) {
            cabeza = cola = nuevoNodo;
            cabeza->siguiente = cabeza;
            cabeza->anterior = cabeza;
        } else {
            cola->siguiente = nuevoNodo;
            nuevoNodo->anterior = cola;
            nuevoNodo->siguiente = cabeza;
            cabeza->anterior = nuevoNodo;
            cola = nuevoNodo;
        }
    }

    void mostrarLista() const {
        if (estaVacia()) {
            cout << "La lista está vacía." << endl;
            return;
        }
        Nodo* actual = cabeza;
        do {
            mostrarAvion(actual->avion);
            actual = actual->siguiente;
        } while (actual != cabeza);
    }

    void mostrarAvion(const Avion& avion) const {
        cout << "Vuelo: " << avion.getVuelo() << endl;
        cout << "Número de registro: " << avion.getNumeroDeRegistro() << endl;
        cout << "Modelo: " << avion.getModelo() << endl;
        cout << "Capacidad: " << avion.getCapacidad() << endl;
        cout << "Aerolínea: " << avion.getAerolinea() << endl;
        cout << "Ciudad de destino: " << avion.getCiudadDestino() << endl;
        cout << "Estado: " << avion.getEstado() << endl;
        cout << "-------------------------------" << endl;
    }

        void generarDot() const {
        ofstream archivo("listaCircularDoble.dot");
        

        archivo << "digraph G {\n";
        archivo << "  rankdir = LR;\n";

        if (!estaVacia()) {
            Nodo* actual = cabeza;
            int indice = 0;
            do {
                archivo << "  nodo" << indice << " [label=\"";
                archivo << "Vuelo: " << actual->avion.getVuelo() << endl;
                archivo << "Número de registro: " << actual->avion.getNumeroDeRegistro() << endl;
                archivo << "Modelo: " << actual->avion.getModelo() << endl;
                archivo << "Ciudad de destino: " << actual->avion.getCiudadDestino() << endl;
                archivo << "Estado: " << actual->avion.getEstado() << endl;
                archivo << "\"];\n";

                indice++;
                actual = actual->siguiente;
            } while (actual != cabeza);

            actual = cabeza;
            indice = 0;
            do {
                archivo << "  nodo" << indice << " -> nodo" << ((indice + 1) % cantidadNodos()) << ";\n";
                archivo << "  nodo" << ((indice + 1) % cantidadNodos()) << " -> nodo" << indice << ";\n";
                indice++;
                actual = actual->siguiente;
            } while (actual != cabeza);
        }

        archivo << "}\n";
        archivo.close();

        system("dot -Tpng listaCircularDoble.dot -o listaCircularDoble.png");

        system("start listaCircularDoble.png");
    }

    int cantidadNodos() const {
        if (estaVacia()) return 0;
        int contador = 0;
        Nodo* actual = cabeza;
        do {
            contador++;
            actual = actual->siguiente;
        } while (actual != cabeza);
        return contador;
    }

    void eliminarPorNumeroDeRegistro(const string& numeroDeRegistro) {
        if (estaVacia()) return; // Si la lista está vacía, no hay nada que eliminar.

        Nodo* actual = cabeza;
        Nodo* aEliminar = nullptr;

        // Buscar el nodo a eliminar.
        do {
            if (actual->avion.getNumeroDeRegistro() == numeroDeRegistro) {
                aEliminar = actual;
                break;
            }
            actual = actual->siguiente;
        } while (actual != cabeza);

        if (aEliminar == nullptr) return; // Si no se encontró, no hay nada que hacer.

        // Si el nodo a eliminar es el único nodo en la lista.
        if (aEliminar->siguiente == aEliminar) {
            cabeza = cola = nullptr;
        } else {
            // Si hay más de un nodo en la lista.
            aEliminar->anterior->siguiente = aEliminar->siguiente;
            aEliminar->siguiente->anterior = aEliminar->anterior;

            // Si el nodo a eliminar es la cabeza o la cola, actualizarlos.
            if (aEliminar == cabeza) {
                cabeza = aEliminar->siguiente;
            }
            if (aEliminar == cola) {
                cola = aEliminar->anterior;
            }
        }

        delete aEliminar; // Liberar la memoria del nodo eliminado.
    }

    Avion* obtenerPorNumeroDeRegistro(const string& numeroDeRegistro) {
        if (estaVacia()) return nullptr; // Si la lista está vacía, no hay aviones que retornar.

        Nodo* actual = cabeza;
        do {
            if (actual->avion.getNumeroDeRegistro() == numeroDeRegistro) {
                return &(actual->avion); // Retorna la dirección del avión encontrado.
            }
            actual = actual->siguiente;
        } while (actual != cabeza);

        return nullptr; // Si no se encuentra el avión, retorna nullptr.
    }
};

#endif // LISTA_CIRCULAR_DOBLE_H