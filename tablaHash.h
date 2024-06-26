#ifndef TABLA_HASH_H
#define TABLA_HASH_H

#include <iostream>
#include "piloto.h"

using namespace std;

class TablaHash {
private:
    int M; // Tamaño de la tabla
    Piloto** tabla; // Arreglo de punteros a la cabeza de las listas enlazadas

    int h(string llave) {
        // Extraer la parte numérica de numero_de_id ignorando el primer carácter
        int id_num = stoi(llave.substr(1));
        return id_num % M;
    }

public:
    TablaHash(int tam) : M(tam) {
        tabla = new Piloto*[M];
        for (int i = 0; i < M; ++i) {
            tabla[i] = nullptr;
        }
    }

    ~TablaHash() {
        for (int i = 0; i < M; ++i) {
            Piloto* actual = tabla[i];
            while (actual != nullptr) {
                Piloto* temp = actual;
                actual = actual->siguiente;
                delete temp;
            }
        }
        delete[] tabla;
    }

    void insertar(Piloto* nuevo_piloto) {
        int indice = h(nuevo_piloto->numero_de_id);
        nuevo_piloto->siguiente = tabla[indice];
        tabla[indice] = nuevo_piloto;
    }

    Piloto* buscar(string numero_de_id) {
        int indice = h(numero_de_id);
        Piloto* actual = tabla[indice];
        while (actual != nullptr) {
            if (actual->numero_de_id == numero_de_id) {
                return actual;
            }
            actual = actual->siguiente;
        }
        return nullptr;
    }

    void eliminar(string numero_de_id) {
        int indice = h(numero_de_id);
        Piloto* actual = tabla[indice];
        Piloto* anterior = nullptr;
        
        while (actual != nullptr && actual->numero_de_id != numero_de_id) {
            anterior = actual;
            actual = actual->siguiente;
        }
        
        if (actual == nullptr) {
            return; // El piloto no se encontró
        }
        
        if (anterior == nullptr) {
            tabla[indice] = actual->siguiente; // El piloto a eliminar es el primero de la lista
        } else {
            anterior->siguiente = actual->siguiente;
        }
        
        delete actual;
    }

    void imprimirTabla() {
        for (int i = 0; i < M; ++i) {
            cout << "Indice " << i << ": ";
            Piloto* actual = tabla[i];
            while (actual != nullptr) {
                cout << "(" << actual->numero_de_id << ", " << actual->nombre << ") ";
                actual = actual->siguiente;
            }
            cout << endl;
        }
    }
};

#endif // TABLA_HASH_H