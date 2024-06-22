#ifndef ARBOL_BINARIO_H
#define ARBOL_BINARIO_H

#include <iostream>
#include <string>

// Definimos la estructura del objeto a almacenar
struct PilotoBB {
    std::string nombre;
    std::string nacionalidad;
    std::string numero_de_id;
    std::string vuelo;
    int horas_de_vuelo;
    std::string tipo_de_licencia;
};

// Definimos la estructura del nodo del árbol
struct NodoBB {
    PilotoBB datos;
    NodoBB* izquierdo;
    NodoBB* derecho;

    NodoBB(const PilotoBB& piloto) : datos(piloto), izquierdo(nullptr), derecho(nullptr) {}
};

// Definimos la clase del Árbol Binario de Búsqueda
class ArbolBinarioBusqueda {
private:
    NodoBB* raiz;
    mutable bool esPrimerValor = true;

    // Función auxiliar para insertar_piloto un nodo en el árbol
    NodoBB* insertar_piloto(NodoBB* nodo, const PilotoBB& piloto) {
        if (nodo == nullptr) {
            return new NodoBB(piloto);
        }

        if (piloto.horas_de_vuelo < nodo->datos.horas_de_vuelo) {
            nodo->izquierdo = insertar_piloto(nodo->izquierdo, piloto);
        } else {
            nodo->derecho = insertar_piloto(nodo->derecho, piloto);
        }

        return nodo;
    }

    // Función auxiliar para buscar un nodo en el árbol
    NodoBB* buscar(NodoBB* nodo, int horas_de_vuelo) const {
        if (nodo == nullptr || nodo->datos.horas_de_vuelo == horas_de_vuelo) {
            return nodo;
        }

        if (horas_de_vuelo < nodo->datos.horas_de_vuelo) {
            return buscar(nodo->izquierdo, horas_de_vuelo);
        } else {
            return buscar(nodo->derecho, horas_de_vuelo);
        }
    }

    // Modificación de la función enOrden para incluir el nivel de profundidad
    void enOrden(NodoBB* nodo) const {
        if (nodo != nullptr) {
            enOrden(nodo->izquierdo);
            if (!esPrimerValor) {
                std::cout << ", ";
            }
            std::cout << nodo->datos.horas_de_vuelo;
            esPrimerValor = false;
            enOrden(nodo->derecho);
        }
    }   

    // Método para recorrer el árbol en preorden
    void preOrden(NodoBB* nodo) const {
        if (nodo != nullptr) {
            if (!esPrimerValor) {
                std::cout << ", ";
            }
            std::cout << nodo->datos.horas_de_vuelo;
            esPrimerValor = false;
            preOrden(nodo->izquierdo);
            preOrden(nodo->derecho);
        }
    }

    // Método para recorrer el árbol en postorden
    void postOrden(NodoBB* nodo) const {
        if (nodo != nullptr) {
            postOrden(nodo->izquierdo);
            postOrden(nodo->derecho);
            if (!esPrimerValor) {
                std::cout << ", ";
            }
            std::cout << nodo->datos.horas_de_vuelo;
            esPrimerValor = false;
        }
    }

public:
    ArbolBinarioBusqueda() : raiz(nullptr) {}

    // Función para insertar_piloto un nuevo piloto en el árbol
    void insertar(const PilotoBB& piloto) {
        raiz = insertar_piloto(raiz, piloto);
    }

    // Función para buscar un piloto por sus horas de vuelo
    PilotoBB* buscar(int horas_de_vuelo) const {
        NodoBB* nodo = buscar(raiz, horas_de_vuelo);
        if (nodo != nullptr) {
            return &nodo->datos;
        } else {
            return nullptr;
        }
    }

    // Antes de llamar a cualquiera de los métodos de recorrido (enOrden, preOrden, postOrden),
    // asegúrate de restablecer esPrimerValor a true.
    void mostrarEnOrden() const {
        esPrimerValor = true;
        enOrden(raiz);
        std::cout << std::endl; // Para finalizar la línea después del último valor
    }

    // Funciones para exponer recorridos preorden y postorden al usuario
    void mostrarPreOrden() const {
        esPrimerValor = true;
        preOrden(raiz);
        std::cout << std::endl;
    }

    void mostrarPostOrden() const {
        esPrimerValor = true;
        postOrden(raiz);
        std::cout << std::endl;
    }
};

#endif // ARBOL_BINARIO_H