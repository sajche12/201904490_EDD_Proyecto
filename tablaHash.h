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
        // Paso 1: Convertir el primer carácter a su valor ASCII
        int ascii_value = static_cast<int>(llave[0]);
        
        // Paso 2 y 3: Separar los dígitos del resto de la llave y sumarlos
        int sum = 0;
        for (size_t i = 1; i < llave.length(); ++i) {
            sum += llave[i] - '0'; // Convertir cada carácter a entero y sumar
        }
        
        // Paso 4: Sumar el valor ASCII del primer carácter
        sum += ascii_value;
        
        // Paso 5: Aplicar módulo por M para obtener el índice
        return sum % M;
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

    void visualizarTablaComoString() {
    string dotGraph = "digraph TablaHash {\n";
    dotGraph += "rankdir=LR;\n"; // Cambio aquí para orientación de izquierda a derecha
    dotGraph += "node [shape=box];\n"; // Estilo global para los nodos

    for (int i = 0; i < M; ++i) {
        dotGraph += to_string(i) + " [label=\"" + to_string(i) + "\"];\n";

        Piloto* actual = tabla[i];
        string nodoAnterior = to_string(i);
        while (actual != nullptr) {
            string idPiloto = actual->numero_de_id;
            dotGraph += idPiloto + " [label=\"" + idPiloto + "\"];\n";
            dotGraph += nodoAnterior + " -> " + idPiloto + ";\n";
            nodoAnterior = idPiloto;
            actual = actual->siguiente;
        }
    }

    dotGraph += "}\n";

    // Opcional: Guardar dotGraph en un archivo
    std::ofstream outFile("tabla_hash_visualizacion.dot");
    if (outFile.is_open()) {
        outFile << dotGraph;
        outFile.close();
    }

    system("dot -Tpng -o tabla_hash_visualizacion.png tabla_hash_visualizacion.dot");

    system("start tabla_hash_visualizacion.png");
    }
};

#endif // TABLA_HASH_H