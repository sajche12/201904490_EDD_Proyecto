#ifndef ARBOLB_H
#define ARBOLB_H

#include <iostream>
#include <cstring>

const int GRADO = 5; // Grado del árbol B

struct Vuelo {
    char vuelo[10];
    char numero_de_registro[10];
    char modelo[20];
    int capacidad;
    char aerolinea[20];
    char ciudad_destino[20];
    char estado[15];

    Vuelo() {
        strcpy(estado, "Mantenimiento");
    }
};

struct NodoB {
    Vuelo llaves[GRADO - 1];
    NodoB* hijos[GRADO];
    int num_llaves;
    bool hoja;

    NodoB() {
        num_llaves = 0;
        hoja = true;
        for (int i = 0; i < GRADO; i++) {
            hijos[i] = nullptr;
        }
    }
};

class ArbolB {
public:
    ArbolB() {
        raiz = new NodoB();
    }

    void insertar(const Vuelo& vuelo) {
        NodoB* r = raiz;
        if (r->num_llaves == GRADO - 1) {
            NodoB* s = new NodoB();
            raiz = s;
            s->hoja = false;
            s->hijos[0] = r;
            dividirHijo(s, 0, r);
            insertarNoLleno(s, vuelo);
        } else {
            insertarNoLleno(r, vuelo);
        }
    }

    void imprimir() {
        imprimir(raiz, 0);
    }

private:
    NodoB* raiz;

    void dividirHijo(NodoB* x, int i, NodoB* y) {
        NodoB* z = new NodoB();
        z->hoja = y->hoja;
        z->num_llaves = (GRADO / 2) - 1;

        for (int j = 0; j < (GRADO / 2) - 1; j++) {
            z->llaves[j] = y->llaves[j + (GRADO / 2)];
        }

        if (!y->hoja) {
            for (int j = 0; j < GRADO / 2; j++) {
                z->hijos[j] = y->hijos[j + (GRADO / 2)];
            }
        }

        y->num_llaves = (GRADO / 2) - 1;

        for (int j = x->num_llaves; j >= i + 1; j--) {
            x->hijos[j + 1] = x->hijos[j];
        }

        x->hijos[i + 1] = z;

        for (int j = x->num_llaves - 1; j >= i; j--) {
            x->llaves[j + 1] = x->llaves[j];
        }

        x->llaves[i] = y->llaves[(GRADO / 2) - 1];
        x->num_llaves++;
    }

    void insertarNoLleno(NodoB* x, const Vuelo& vuelo) {
        int i = x->num_llaves - 1;
        if (x->hoja) {
            while (i >= 0 && strcmp(vuelo.numero_de_registro, x->llaves[i].numero_de_registro) < 0) {
                x->llaves[i + 1] = x->llaves[i];
                i--;
            }
            x->llaves[i + 1] = vuelo;
            x->num_llaves++;
        } else {
            while (i >= 0 && strcmp(vuelo.numero_de_registro, x->llaves[i].numero_de_registro) < 0) {
                i--;
            }
            i++;
            if (x->hijos[i]->num_llaves == GRADO - 1) {
                dividirHijo(x, i, x->hijos[i]);
                if (strcmp(vuelo.numero_de_registro, x->llaves[i].numero_de_registro) > 0) {
                    i++;
                }
            }
            insertarNoLleno(x->hijos[i], vuelo);
        }
    }

    void imprimir(NodoB* nodo, int nivel) {
        if (nodo != nullptr) {
            for (int i = 0; i < nodo->num_llaves; i++) {
                for (int j = 0; j < nivel; j++) {
                    std::cout << "    ";
                }
                std::cout << nodo->llaves[i].numero_de_registro << " " << nodo->llaves[i].vuelo << " " << nodo->llaves[i].modelo << " " << nodo->llaves[i].capacidad << " " << nodo->llaves[i].aerolinea << " " << nodo->llaves[i].ciudad_destino << " " << nodo->llaves[i].estado << std::endl;
            }
            if (!nodo->hoja) {
                for (int i = 0; i <= nodo->num_llaves; i++) {
                    imprimir(nodo->hijos[i], nivel + 1);
                }
            }
        }
    }
};

#endif // ARBOLB_H