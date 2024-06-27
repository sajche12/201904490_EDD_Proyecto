#ifndef ARBOLB_H
#define ARBOLB_H

#include <iostream>
#include <string>
#include "avion.h" // Asumimos que este archivo ya existe

const int ORDEN = 5;
const int MAX_CLAVES = ORDEN - 1;
const int MAX_HIJOS = ORDEN;

class NodoB {
public:
    Avion* claves[MAX_CLAVES];
    NodoB* hijos[MAX_HIJOS];
    int numClaves;
    bool esHoja;

    NodoB() {
        for (int i = 0; i < MAX_CLAVES; i++) {
            claves[i] = nullptr;
        }
        for (int i = 0; i < MAX_HIJOS; i++) {
            hijos[i] = nullptr;
        }
        numClaves = 0;
        esHoja = true;
    }

    ~NodoB() {
        for (int i = 0; i < numClaves; i++) {
            delete claves[i];
        }
        if (!esHoja) {
            for (int i = 0; i <= numClaves; i++) {
                delete hijos[i];
            }
        }
    }
};

class ArbolB {
private:
    NodoB* raiz;

    void dividirHijo(NodoB* padre, int indice, NodoB* hijo) {
        NodoB* nuevoNodo = new NodoB();
        nuevoNodo->esHoja = hijo->esHoja;
        nuevoNodo->numClaves = MAX_CLAVES / 2;

        for (int j = 0; j < MAX_CLAVES / 2; j++) {
            nuevoNodo->claves[j] = hijo->claves[j + MAX_CLAVES / 2 + 1];
        }

        if (!hijo->esHoja) {
            for (int j = 0; j < MAX_HIJOS / 2; j++) {
                nuevoNodo->hijos[j] = hijo->hijos[j + MAX_HIJOS / 2];
            }
        }

        hijo->numClaves = MAX_CLAVES / 2;

        for (int j = padre->numClaves; j >= indice + 1; j--) {
            padre->hijos[j + 1] = padre->hijos[j];
        }

        padre->hijos[indice + 1] = nuevoNodo;

        for (int j = padre->numClaves - 1; j >= indice; j--) {
            padre->claves[j + 1] = padre->claves[j];
        }

        padre->claves[indice] = hijo->claves[MAX_CLAVES / 2];
        padre->numClaves++;
    }

    void insertarNoLleno(NodoB* nodo, Avion* avion) {
        int i = nodo->numClaves - 1;

        if (nodo->esHoja) {
            while (i >= 0 && avion->getNumeroDeRegistro() < nodo->claves[i]->getNumeroDeRegistro()) {
                nodo->claves[i + 1] = nodo->claves[i];
                i--;
            }

            nodo->claves[i + 1] = avion;
            nodo->numClaves++;
        } else {
            while (i >= 0 && avion->getNumeroDeRegistro() < nodo->claves[i]->getNumeroDeRegistro()) {
                i--;
            }

            i++;

            if (nodo->hijos[i]->numClaves == MAX_CLAVES) {
                dividirHijo(nodo, i, nodo->hijos[i]);

                if (avion->getNumeroDeRegistro() > nodo->claves[i]->getNumeroDeRegistro()) {
                    i++;
                }
            }
            insertarNoLleno(nodo->hijos[i], avion);
        }
    }

    void generarDotRecursivo(NodoB* nodo, std::ofstream& archivo, int& contadorNodos) {
        if (!nodo) return;

        int idNodoActual = contadorNodos++;

        std::stringstream contenidoNodo;
        contenidoNodo << "Nodo" << idNodoActual << " [label=\"";
        for (int i = 0; i < nodo->numClaves; i++) {
            if (i > 0) contenidoNodo << "|";
            contenidoNodo << nodo->claves[i]->getNumeroDeRegistro();
        }
        contenidoNodo << "\"];\n";
        archivo << contenidoNodo.str();

        if (!nodo->esHoja) {
            for (int i = 0; i <= nodo->numClaves; i++) {
                int idHijo = contadorNodos;
                generarDotRecursivo(nodo->hijos[i], archivo, contadorNodos);
                archivo << "Nodo" << idNodoActual << " -> Nodo" << idHijo << ";\n";
            }
        }
    }

public:
    ArbolB() {
        raiz = new NodoB();
    }

    ~ArbolB() {
        if (raiz) {
            delete raiz;
        }
    }

    void insertar(Avion* avion) {
        if (!avion) {
            throw std::invalid_argument("No se puede insertar un avión nulo");
        }

        if (!raiz) {
            raiz = new NodoB();
        }

        if (raiz->numClaves == MAX_CLAVES) {
            NodoB* nuevoNodo = new NodoB();
            nuevoNodo->esHoja = false;
            nuevoNodo->hijos[0] = raiz;
            dividirHijo(nuevoNodo, 0, raiz);
            insertarNoLleno(nuevoNodo, avion);
            raiz = nuevoNodo;
        } else {
            insertarNoLleno(raiz, avion);
        }
    }

    Avion* buscar(const std::string& numero_de_registro) {
        if (numero_de_registro.empty()) {
            throw std::invalid_argument("El número de registro no puede estar vacío");
        }
        return buscarRecursivo(raiz, numero_de_registro);
    }

    void generarVisualizacion() {
        std::ofstream archivo("arbolB.dot");
        if (!archivo.is_open()) {
            throw std::runtime_error("No se pudo abrir el archivo para escribir");
        }

        archivo << "digraph ArbolB {\n";
        archivo << "node [shape=record];\n";

        int contadorNodos = 0;
        generarDotRecursivo(raiz, archivo, contadorNodos);

        archivo << "}\n";
        archivo.close();

        system("dot -Tpng arbolB.dot -o arbolB.png");
        system("arbolB.png");

    }

private:
    Avion* buscarRecursivo(NodoB* nodo, const std::string& numero_de_registro) {
        int i = 0;
        while (i < nodo->numClaves && numero_de_registro > nodo->claves[i]->getNumeroDeRegistro()) {
            i++;
        }

        if (i < nodo->numClaves && numero_de_registro == nodo->claves[i]->getNumeroDeRegistro()) {
            return nodo->claves[i];
        }

        if (nodo->esHoja) {
            return nullptr;
        }

        return buscarRecursivo(nodo->hijos[i], numero_de_registro);
    }
};

#endif // ARBOLB_H