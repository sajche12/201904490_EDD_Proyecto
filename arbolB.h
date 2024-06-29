#ifndef ARBOLB_H
#define ARBOLB_H

#include "avion.h"
#include <iostream>
#include <string>

using namespace std;

const int ORDEN = 5;

int obtenerNumero(string numero_de_registro) {
    return stoi(numero_de_registro.substr(1));
}

class NodoB {
public:
    bool hoja;
    int n; // Número de claves
    Avion* aviones[ORDEN - 1];
    NodoB* hijos[ORDEN];

    NodoB(bool _hoja) {
        hoja = _hoja;
        n = 0;
        for (int i = 0; i < ORDEN; i++)
            hijos[i] = nullptr;
    }

    void insertarNoLleno(Avion* avion) {
        int num_registro = obtenerNumero(avion->getNumeroDeRegistro());
        int i = n - 1;

        if (hoja) {
            while (i >= 0 && obtenerNumero(aviones[i]->getNumeroDeRegistro()) > num_registro) {
                aviones[i + 1] = aviones[i];
                i--;
            }

            aviones[i + 1] = avion;
            n = n + 1;
        } else {
            while (i >= 0 && obtenerNumero(aviones[i]->getNumeroDeRegistro()) > num_registro)
                i--;

            if (hijos[i + 1]->n == ORDEN - 1) {
                dividirHijo(i + 1, hijos[i + 1]);

                if (obtenerNumero(aviones[i + 1]->getNumeroDeRegistro()) < num_registro)
                    i++;
            }
            hijos[i + 1]->insertarNoLleno(avion);
        }
    }

    void dividirHijo(int i, NodoB* y) {
        NodoB* z = new NodoB(y->hoja);
        z->n = (ORDEN / 2) - 1;

        for (int j = 0; j < (ORDEN / 2) - 1; j++)
            z->aviones[j] = y->aviones[j + (ORDEN / 2)];

        if (!y->hoja) {
            for (int j = 0; j < ORDEN / 2; j++)
                z->hijos[j] = y->hijos[j + (ORDEN / 2)];
        }

        y->n = (ORDEN / 2) - 1;

        for (int j = n; j >= i + 1; j--)
            hijos[j + 1] = hijos[j];

        hijos[i + 1] = z;

        for (int j = n - 1; j >= i; j--)
            aviones[j + 1] = aviones[j];

        aviones[i] = y->aviones[(ORDEN / 2) - 1];

        n = n + 1;
    }

    void recorrer() {
        int i;
        for (i = 0; i < n; i++) {
            if (!hoja)
                hijos[i]->recorrer();
            cout << " " << aviones[i]->getNumeroDeRegistro();
        }

        if (!hoja)
            hijos[i]->recorrer();
    }

    NodoB* buscar(string numero_de_registro) {
        int num_registro = obtenerNumero(numero_de_registro);
        int i = 0;
        while (i < n && obtenerNumero(aviones[i]->getNumeroDeRegistro()) < num_registro)
            i++;

        if (i < n && obtenerNumero(aviones[i]->getNumeroDeRegistro()) == num_registro)
            return this;

        if (hoja)
            return nullptr;

        return hijos[i]->buscar(numero_de_registro);
    }

    void generarDot(ofstream& archivo, int& nodoId) {
        int miId = nodoId++;
        archivo << "    nodo" << miId << " [label=\"";
        for (int i = 0; i < n; i++) {
            if (i > 0) archivo << " | ";
            archivo << "<f" << i << "> " << aviones[i]->getNumeroDeRegistro();
        }
        archivo << "\"];\n";

        for (int i = 0; i <= n; i++) {
            if (hijos[i] != nullptr) {
                int hijoId = nodoId;
                hijos[i]->generarDot(archivo, nodoId);
                archivo << "    nodo" << miId << ":f" << i << " -> nodo" << hijoId << ";\n";
            }
        }
    }

    void eliminarRecursivo(string numero_de_registro) {
        int i = 0;
        while (i < n && obtenerNumero(aviones[i]->getNumeroDeRegistro()) < obtenerNumero(numero_de_registro))
            i++;
        if (i < n && aviones[i]->getNumeroDeRegistro() == numero_de_registro) { // Caso 1: El avión está en este nodo
            if (hoja) {
                // Eliminar el avión directamente si es una hoja
            } else {
                // Encontrar y eliminar el predecesor/sucesor si no es una hoja
            }
        } else {
            if (hoja) {
                // El avión no está en el árbol
                return;
            }
            // Descender al hijo adecuado, posiblemente después de ajustar el árbol
            bool flag = ((i == n) ? true : false);
            if (hijos[i]->n < (ORDEN / 2)) {
                // Ajustar el árbol antes de descender
            }
            if (flag && i > n)
                hijos[i - 1]->eliminarRecursivo(numero_de_registro);
            else
                hijos[i]->eliminarRecursivo(numero_de_registro);
        }
        // Ajustar este nodo después de la eliminación si es necesario
    }

    Avion* buscarRecursivo(string numeroRegistro) {
        int num_registro = obtenerNumero(numeroRegistro);
        int i = 0;
        while (i < n && obtenerNumero(aviones[i]->getNumeroDeRegistro()) < num_registro)
            i++;

        if (i < n && obtenerNumero(aviones[i]->getNumeroDeRegistro()) == num_registro)
            return aviones[i];

        if (hoja)
            return nullptr;

        return hijos[i]->buscarRecursivo(numeroRegistro);
    }
};

class ArbolB {
private:
    NodoB* raiz;

public:
    ArbolB() { raiz = nullptr; }

    void recorrer() {
        if (raiz != nullptr) raiz->recorrer();
    }

    NodoB* buscar(string numero_de_registro) {
        return (raiz == nullptr) ? nullptr : raiz->buscar(numero_de_registro);
    }

    void insertar(Avion* avion) {
        if (raiz == nullptr) {
            raiz = new NodoB(true);
            raiz->aviones[0] = avion;
            raiz->n = 1;
        } else {
            if (raiz->n == ORDEN - 1) {
                NodoB* s = new NodoB(false);
                s->hijos[0] = raiz;

                s->dividirHijo(0, raiz);

                int i = 0;
                if (obtenerNumero(s->aviones[0]->getNumeroDeRegistro()) < obtenerNumero(avion->getNumeroDeRegistro()))
                    i++;
                s->hijos[i]->insertarNoLleno(avion);

                raiz = s;
            } else {
                raiz->insertarNoLleno(avion);
            }
        }
    }

    void generarDot() {
        ofstream archivo("arbolB.dot");
        archivo << "digraph G {\n";
        archivo << "    node [shape=record];\n";

        int nodoId = 0;
        if (raiz != nullptr) {
            raiz->generarDot(archivo, nodoId);
        }

        archivo << "}\n";
        archivo.close();

        system("dot -Tpng arbolB.dot -o arbolB.png");
    }

    void eliminar(string numero_de_registro) {
        if (raiz != nullptr) {
            raiz->eliminarRecursivo(numero_de_registro);
            if (raiz->n == 0) { // Si la raíz está vacía
                NodoB* tmp = raiz;
                if (raiz->hoja)
                    raiz = nullptr;
                else
                    raiz = raiz->hijos[0];
                delete tmp;
            }
        }
    }

    Avion* buscarAvion(string numeroRegistro) {
        if (raiz == nullptr)
            return nullptr;
        else
            return raiz->buscarRecursivo(numeroRegistro);
    }
};

#endif // ARBOLB_H
