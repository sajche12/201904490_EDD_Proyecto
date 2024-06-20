#ifndef ARBOLB_H
#define ARBOLB_H

#include <iostream>
using namespace std;

// Definición de la estructura Node para el árbol B de grado 5
template <typename T>
struct Node {
    bool hoja; // Si el nodo es una hoja
    int numLlaves; // Número de llaves actuales en el nodo
    T llaves[4]; // Arreglo de llaves
    Node<T>* hijos[5]; // Arreglo de punteros a hijos

    Node(bool esHoja) {
        hoja = esHoja;
        numLlaves = 0;
        for (int i = 0; i < 5; i++) {
            hijos[i] = nullptr;
        }
    }
};

template <typename T>
class ArbolB {
public:
    ArbolB() : raiz(nullptr) {}

    void insertar(const T& llave);
    void recorrer() { recorrer(raiz); }

    // Destructor
    ~ArbolB() {
        delete raiz;
    }

private:
    Node<T>* raiz;

    void insertarNoLleno(Node<T>* nodo, const T& llave);
    void dividirHijo(Node<T>* nodo, int i, Node<T>* hijo);
    void recorrer(Node<T>* nodo);

    // Funciones auxiliares
    int encontrarLlave(Node<T>* nodo, const T& llave);
};

template <typename T>
void ArbolB<T>::insertar(const T& llave) {
    if (raiz == nullptr) {
        raiz = new Node<T>(true);
        raiz->llaves[0] = llave;
        raiz->numLlaves = 1;
    } else {
        if (raiz->numLlaves == 4) {
            Node<T>* nuevoNode = new Node<T>(false);
            nuevoNode->hijos[0] = raiz;
            dividirHijo(nuevoNode, 0, raiz);
            int i = 0;
            if (nuevoNode->llaves[0] < llave) {
                i++;
            }
            insertarNoLleno(nuevoNode->hijos[i], llave);
            raiz = nuevoNode;
        } else {
            insertarNoLleno(raiz, llave);
        }
    }
}

template <typename T>
void ArbolB<T>::insertarNoLleno(Node<T>* nodo, const T& llave) {
    int i = nodo->numLlaves - 1;

    if (nodo->hoja) {
        while (i >= 0 && nodo->llaves[i] > llave) {
            nodo->llaves[i + 1] = nodo->llaves[i];
            i--;
        }
        nodo->llaves[i + 1] = llave;
        nodo->numLlaves++;
    } else {
        while (i >= 0 && nodo->llaves[i] > llave) {
            i--;
        }
        i++;
        if (nodo->hijos[i]->numLlaves == 4) {
            dividirHijo(nodo, i, nodo->hijos[i]);
            if (nodo->llaves[i] < llave) {
                i++;
            }
        }
        insertarNoLleno(nodo->hijos[i], llave);
    }
}

template <typename T>
void ArbolB<T>::dividirHijo(Node<T>* nodo, int i, Node<T>* hijo) {
    Node<T>* nuevoHijo = new Node<T>(hijo->hoja);
    nuevoHijo->numLlaves = 2;

    for (int j = 0; j < 2; j++) {
        nuevoHijo->llaves[j] = hijo->llaves[j + 2];
    }

    if (!hijo->hoja) {
        for (int j = 0; j < 3; j++) {
            nuevoHijo->hijos[j] = hijo->hijos[j + 2];
        }
    }

    hijo->numLlaves = 2;

    for (int j = nodo->numLlaves; j >= i + 1; j--) {
        nodo->hijos[j + 1] = nodo->hijos[j];
    }

    nodo->hijos[i + 1] = nuevoHijo;

    for (int j = nodo->numLlaves - 1; j >= i; j--) {
        nodo->llaves[j + 1] = nodo->llaves[j];
    }

    nodo->llaves[i] = hijo->llaves[2];
    nodo->numLlaves++;
}

template <typename T>
void ArbolB<T>::recorrer(Node<T>* nodo) {
    if (nodo != nullptr) {
        int i;
        for (i = 0; i < nodo->numLlaves; i++) {
            if (!nodo->hoja) {
                recorrer(nodo->hijos[i]);
            }
            cout << " " << nodo->llaves[i];
        }

        if (!nodo->hoja) {
            recorrer(nodo->hijos[i]);
        }
    }
}

template <typename T>
int ArbolB<T>::encontrarLlave(Node<T>* nodo, const T& llave) {
    int i = 0;
    while (i < nodo->numLlaves && nodo->llaves[i] < llave) {
        i++;
    }
    return i;
}

#endif // ARBOLB_H