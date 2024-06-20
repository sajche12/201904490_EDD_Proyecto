#ifndef LISTA_CIRCULAR_DOBLE_H
#define LISTA_CIRCULAR_DOBLE_H

#include <iostream>
#include <string>

using namespace std;

template <typename Tipo>
struct Nodo {
    Tipo dato;
    Nodo<Tipo>* siguiente;
    Nodo<Tipo>* anterior;

    Nodo(const Tipo& valor) : dato(valor), siguiente(nullptr), anterior(nullptr) {}
};

template <typename Tipo>
class ListaCircularDoble {
private:
    Nodo<Tipo>* primero;
    Nodo<Tipo>* ultimo;
    int tamano;

public:
    ListaCircularDoble() : primero(nullptr), ultimo(nullptr), tamano(0) {}

    ~ListaCircularDoble() {
        vaciar();
    }

    void insertar(const Tipo& valor) {
        Nodo<Tipo>* nuevoNodo = new Nodo<Tipo>(valor);

        if (estaVacia()) {
            primero = nuevoNodo;
            ultimo = nuevoNodo;
            nuevoNodo->siguiente = nuevoNodo;
            nuevoNodo->anterior = nuevoNodo;
        } else {
            nuevoNodo->anterior = ultimo;
            nuevoNodo->siguiente = primero;
            ultimo->siguiente = nuevoNodo;
            primero->anterior = nuevoNodo;
            ultimo = nuevoNodo;
        }

        tamano++;
    }

    void eliminar(const Tipo& valor) {
        if (estaVacia()) {
            return;
        }

        Nodo<Tipo>* actual = primero;

        do {
            if (actual->dato == valor) {
                if (actual == primero && actual == ultimo) {
                    primero = nullptr;
                    ultimo = nullptr;
                } else if (actual == primero) {
                    primero = actual->siguiente;
                    ultimo->siguiente = primero;
                    primero->anterior = ultimo;
                } else if (actual == ultimo) {
                    ultimo = actual->anterior;
                    ultimo->siguiente = primero;
                    primero->anterior = ultimo;
                } else {
                    actual->anterior->siguiente = actual->siguiente;
                    actual->siguiente->anterior = actual->anterior;
                }

                delete actual;
                tamano--;
                return;
            }

            actual = actual->siguiente;
        } while (actual != primero);
    }

    bool estaVacia() const {
        return tamano == 0;
    }

    void vaciar() {
        if (!estaVacia()) {
            Nodo<Tipo>* actual = primero;

            do {
                Nodo<Tipo>* temp = actual;
                actual = actual->siguiente;
                delete temp;
            } while (actual != primero);

            primero = nullptr;
            ultimo = nullptr;
            tamano = 0;
        }
    }

    void imprimir() const {
        if (estaVacia()) {
            std::cout << "La lista esta vacia" << std::endl;
            return;
        }

        Nodo<Tipo>* actual = primero;

        do {
            std::cout << actual->dato << " ";
            actual = actual->siguiente;
        } while (actual != primero);

        std::cout << std::endl;
    }

    int obtenerTamano() const {
        return tamano;
    }

    Tipo obtenerElemento(int indice) const {
        if (indice < 0 || indice >= tamano) {
            throw std::out_of_range("Indice fuera de rango");
        }

        Nodo<Tipo>* actual = primero;
        for (int i = 0; i < indice; i++) {
            actual = actual->siguiente;
        }

        return actual->dato;
    }

    Tipo* buscarPorNumeroDeRegistro(const string& numeroDeRegistro) const {
        if (estaVacia()) {
            return nullptr;
        }

        Nodo<Tipo>* actual = primero;

        do {
            if (actual->dato.getNumeroRegistro() == numeroDeRegistro) {
                return &(actual->dato);
            }

            actual = actual->siguiente;
        } while (actual != primero);

        return nullptr;
    }

    // Metodo para eliminar por numero de registro
    void eliminarPorNumeroDeRegistro(const string& numeroDeRegistro) {
        if (estaVacia()) {
            return;
        }

        Nodo<Tipo>* actual = primero;

        do {
            if (actual->dato.getNumeroRegistro() == numeroDeRegistro) {
                if (actual == primero && actual == ultimo) {
                    primero = nullptr;
                    ultimo = nullptr;
                } else if (actual == primero) {
                    primero = actual->siguiente;
                    ultimo->siguiente = primero;
                    primero->anterior = ultimo;
                } else if (actual == ultimo) {
                    ultimo = actual->anterior;
                    ultimo->siguiente = primero;
                    primero->anterior = ultimo;
                } else {
                    actual->anterior->siguiente = actual->siguiente;
                    actual->siguiente->anterior = actual->anterior;
                }

                delete actual;
                tamano--;
                return;
            }

            actual = actual->siguiente;
        } while (actual != primero);
    }
};

#endif // LISTA_CIRCULAR_DOBLE_H