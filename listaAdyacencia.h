#ifndef LISTA_ADYACENCIA_H
#define LISTA_ADYACENCIA_H

#include <iostream>
#include <cstring>
using namespace std;

struct NodoLA {
    string origen;
    string destino;
    int distancia;
    NodoLA* siguiente;
};

struct ListaAdyacencia {
    int numNodos;
    NodoLA** lista;
    
    ListaAdyacencia(int n) {
        numNodos = n;
        lista = new NodoLA*[n];
        for (int i = 0; i < n; i++)
            lista[i] = nullptr;
    }

    void agregarArista(string origen, string destino, int distancia) {
        NodoLA* nuevoNodo = new NodoLA;
        nuevoNodo->origen = origen;
        nuevoNodo->destino = destino;
        nuevoNodo->distancia = distancia;
        nuevoNodo->siguiente = nullptr;

        int indiceOrigen = buscarIndice(origen);
        if (indiceOrigen == -1) {
            cout << "El nodo origen no existe" << endl;
            return;
        }

        NodoLA* temp = lista[indiceOrigen];
        if (temp == nullptr)
            lista[indiceOrigen] = nuevoNodo;
        else {
            while (temp->siguiente != nullptr)
                temp = temp->siguiente;
            temp->siguiente = nuevoNodo;
        }
    }

    int buscarIndice(string nodo) {
        for (int i = 0; i < numNodos; i++) {
            NodoLA* temp = lista[i];
            while (temp != nullptr) {
                if (temp->origen == nodo)
                    return i;
                temp = temp->siguiente;
            }
        }
        return -1;
    }

    void imprimirLista() {
        for (int i = 0; i < numNodos; i++) {
            NodoLA* temp = lista[i];
            cout << "NodoLA " << i << ": ";
            while (temp != nullptr) {
                cout << "(" << temp->origen << ", " << temp->destino << ", " << temp->distancia << ") ";
                temp = temp->siguiente;
            }
            cout << endl;
        }
    }
};

#endif // LISTA_ADYACENCIA_H