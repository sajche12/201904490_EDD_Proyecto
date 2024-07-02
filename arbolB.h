#ifndef ARBOLB_H
#define ARBOLB_H

#include <iostream>
#include <cstring>
#include <string>
#include "avion.h"
#include <fstream>
#include <cstdlib>

using namespace std;

const int T = 3;  // Grado del árbol B
const int MAX_CLAVES = 2*T - 1;
const int MAX_HIJOS = 2*T;

struct NodoArbolB {
    Avion* claves[MAX_CLAVES];
    NodoArbolB* hijos[MAX_HIJOS];
    int n; 
    bool hoja;

    NodoArbolB(bool _hoja = true) : n(0), hoja(_hoja) {
        memset(claves, 0, sizeof(claves));
        memset(hijos, 0, sizeof(hijos));
    }
};

class ArbolB {
private:
    NodoArbolB* raiz;

    void dividirHijo(NodoArbolB* padre, int i, NodoArbolB* hijo);
    void insertarNoLleno(NodoArbolB* nodo, Avion* k);
    Avion* buscar(NodoArbolB* nodo, const string& numero_de_registro);
    void eliminar(NodoArbolB* nodo, const string& numero_de_registro);
    void eliminarDeHoja(NodoArbolB* nodo, int idx);
    void eliminarDeNoHoja(NodoArbolB* nodo, int idx);
    void fusionar(NodoArbolB* nodo, int idx);
    void prestarAnterior(NodoArbolB* nodo, int idx);
    void prestarSiguiente(NodoArbolB* nodo, int idx);
    void llenarNodo(NodoArbolB* nodo, int idx);
    Avion* obtenerAntecesor(NodoArbolB* nodo, int idx);
    Avion* obtenerSucesor(NodoArbolB* nodo, int idx);
    void mostrarNodo(NodoArbolB* nodo);
    void generarDOTRecursivo(NodoArbolB* nodo, string& contenido, int& nodoId);
    

public:
    NodoArbolB* getRaiz() const {
        return raiz;
    }

    ArbolB() : raiz(nullptr) {}

    void insertar(Avion* k);
    Avion* buscar(const string& numero_de_registro);
    void eliminar(const string& numero_de_registro);
    void mostrarAviones();
    void generarReporteAvionesDisponibles();
    
    
};

void ArbolB::insertar(Avion* k) {
    if (raiz == nullptr) {
        raiz = new NodoArbolB(true);
        raiz->claves[0] = k;
        raiz->n = 1;
    } else {
        if (raiz->n == MAX_CLAVES) {
            NodoArbolB* s = new NodoArbolB(false);
            s->hijos[0] = raiz;
            dividirHijo(s, 0, raiz);
            int i = 0;
            if (s->claves[0]->getNumeroDeRegistro() < k->getNumeroDeRegistro())
                i++;
            insertarNoLleno(s->hijos[i], k);
            raiz = s;
        } else {
            insertarNoLleno(raiz, k);
        }
    }
}

void ArbolB::insertarNoLleno(NodoArbolB* nodo, Avion* k) {
    int i = nodo->n - 1;
    if (nodo->hoja) {
        while (i >= 0 && nodo->claves[i]->getNumeroDeRegistro() > k->getNumeroDeRegistro()) {
            nodo->claves[i+1] = nodo->claves[i];
            i--;
        }
        nodo->claves[i+1] = k;
        nodo->n = nodo->n + 1;
    } else {
        while (i >= 0 && nodo->claves[i]->getNumeroDeRegistro() > k->getNumeroDeRegistro())
            i--;
        i++;
        if (nodo->hijos[i]->n == MAX_CLAVES) {
            dividirHijo(nodo, i, nodo->hijos[i]);
            if (nodo->claves[i]->getNumeroDeRegistro() < k->getNumeroDeRegistro())
                i++;
        }
        insertarNoLleno(nodo->hijos[i], k);
    }
}

void ArbolB::dividirHijo(NodoArbolB* padre, int i, NodoArbolB* hijo) {
    NodoArbolB* z = new NodoArbolB(hijo->hoja);
    z->n = T - 1;

    for (int j = 0; j < T-1; j++)
        z->claves[j] = hijo->claves[j+T];

    if (!hijo->hoja) {
        for (int j = 0; j < T; j++)
            z->hijos[j] = hijo->hijos[j+T];
    }

    hijo->n = T - 1;

    for (int j = padre->n; j >= i+1; j--)
        padre->hijos[j+1] = padre->hijos[j];

    padre->hijos[i+1] = z;

    for (int j = padre->n-1; j >= i; j--)
        padre->claves[j+1] = padre->claves[j];

    padre->claves[i] = hijo->claves[T-1];
    padre->n = padre->n + 1;
}

Avion* ArbolB::buscar(const string& numero_de_registro) {
    return (raiz == nullptr) ? nullptr : buscar(raiz, numero_de_registro);
}

Avion* ArbolB::buscar(NodoArbolB* nodo, const string& numero_de_registro) {
    int i = 0;
    while (i < nodo->n && numero_de_registro > nodo->claves[i]->getNumeroDeRegistro())
        i++;

    if (i < nodo->n && numero_de_registro == nodo->claves[i]->getNumeroDeRegistro())
        return nodo->claves[i];

    if (nodo->hoja)
        return nullptr;

    return buscar(nodo->hijos[i], numero_de_registro);
}

void ArbolB::mostrarAviones() {
    if (raiz != nullptr)
        mostrarNodo(raiz);
}

void ArbolB::mostrarNodo(NodoArbolB* nodo) {
    for (int i = 0; i < nodo->n; i++) {
        cout << "Vuelo: " << nodo->claves[i]->getVuelo() 
                << ", Registro: " << nodo->claves[i]->getNumeroDeRegistro() 
                << ", Modelo: " << nodo->claves[i]->getModelo() 
                << ", Capacidad: " << nodo->claves[i]->getCapacidad()
                << ", Aerolinea: " << nodo->claves[i]->getAerolinea() 
                << ", Ciudad Destino: " << nodo->claves[i]->getCiudadDestino() 
                << ", Estado: " << nodo->claves[i]->getEstado() << "\n";
    }
    if (!nodo->hoja) {
        for (int i = 0; i <= nodo->n; i++)
            mostrarNodo(nodo->hijos[i]);
    }
}

void ArbolB::eliminar(const string& numero_de_registro) {
    if (!raiz) {
        return;
    }
    eliminar(raiz, numero_de_registro);
    if (raiz->n == 0) {
        NodoArbolB* tmp = raiz;
        if (raiz->hoja)
            raiz = nullptr;
        else
            raiz = raiz->hijos[0];
        delete tmp;
    }
}

void ArbolB::eliminar(NodoArbolB* nodo, const string& numero_de_registro) {
    int idx = 0;
    while (idx < nodo->n && numero_de_registro > nodo->claves[idx]->getNumeroDeRegistro())
        ++idx;

    if (idx < nodo->n && numero_de_registro == nodo->claves[idx]->getNumeroDeRegistro()) {
        if (nodo->hoja)
            eliminarDeHoja(nodo, idx);
        else
            eliminarDeNoHoja(nodo, idx);
    }
    else {
        if (nodo->hoja) {
            cout << "La clave " << numero_de_registro << " no existe en el árbol\n";
            return;
        }

        bool flag = ((idx==nodo->n) ? true : false);

        if (nodo->hijos[idx]->n < T)
            llenarNodo(nodo, idx);

        if (flag && idx > nodo->n)
            eliminar(nodo->hijos[idx-1], numero_de_registro);
        else
            eliminar(nodo->hijos[idx], numero_de_registro);
    }
}

void ArbolB::eliminarDeHoja(NodoArbolB* nodo, int idx) {
    for (int i = idx+1; i < nodo->n; ++i)
        nodo->claves[i-1] = nodo->claves[i];
    nodo->n--;
}

void ArbolB::eliminarDeNoHoja(NodoArbolB* nodo, int idx) {
    Avion* k = nodo->claves[idx];

    if (nodo->hijos[idx]->n >= T) {
        Avion* pred = obtenerAntecesor(nodo, idx);
        nodo->claves[idx] = pred;
        eliminar(nodo->hijos[idx], pred->getNumeroDeRegistro());
    }
    else if (nodo->hijos[idx+1]->n >= T) {
        Avion* succ = obtenerSucesor(nodo, idx);
        nodo->claves[idx] = succ;
        eliminar(nodo->hijos[idx+1], succ->getNumeroDeRegistro());
    }
    else {
        fusionar(nodo, idx);
        eliminar(nodo->hijos[idx], k->getNumeroDeRegistro());
    }
}

void ArbolB::llenarNodo(NodoArbolB* nodo, int idx) {
    if (idx != 0 && nodo->hijos[idx-1]->n >= T)
        prestarAnterior(nodo, idx);
    else if (idx != nodo->n && nodo->hijos[idx+1]->n >= T)
        prestarSiguiente(nodo, idx);
    else {
        if (idx != nodo->n)
            fusionar(nodo, idx);
        else
            fusionar(nodo, idx-1);
    }
}

void ArbolB::prestarAnterior(NodoArbolB* nodo, int idx) {
    NodoArbolB *hijo = nodo->hijos[idx];
    NodoArbolB *hermano = nodo->hijos[idx-1];

    for (int i = hijo->n-1; i >= 0; --i)
        hijo->claves[i+1] = hijo->claves[i];

    if (!hijo->hoja) {
        for(int i = hijo->n; i >= 0; --i)
            hijo->hijos[i+1] = hijo->hijos[i];
    }

    hijo->claves[0] = nodo->claves[idx-1];

    if(!hijo->hoja)
        hijo->hijos[0] = hermano->hijos[hermano->n];

    nodo->claves[idx-1] = hermano->claves[hermano->n-1];

    hijo->n += 1;
    hermano->n -= 1;
}

void ArbolB::prestarSiguiente(NodoArbolB* nodo, int idx) {
    NodoArbolB *hijo = nodo->hijos[idx];
    NodoArbolB *hermano = nodo->hijos[idx+1];

    hijo->claves[hijo->n] = nodo->claves[idx];

    if (!hijo->hoja)
        hijo->hijos[hijo->n+1] = hermano->hijos[0];

    nodo->claves[idx] = hermano->claves[0];

    for (int i=1; i<hermano->n; ++i)
        hermano->claves[i-1] = hermano->claves[i];

    if (!hermano->hoja) {
        for(int i=1; i<=hermano->n; ++i)
            hermano->hijos[i-1] = hermano->hijos[i];
    }

    hijo->n += 1;
    hermano->n -= 1;
}

void ArbolB::fusionar(NodoArbolB* nodo, int idx) {
    NodoArbolB *hijo = nodo->hijos[idx];
    NodoArbolB *hermano = nodo->hijos[idx+1];

    hijo->claves[T-1] = nodo->claves[idx];

    for (int i=0; i<hermano->n; ++i)
        hijo->claves[i+T] = hermano->claves[i];

    if (!hijo->hoja) {
        for(int i=0; i<=hermano->n; ++i)
            hijo->hijos[i+T] = hermano->hijos[i];
    }

    for (int i=idx+1; i<nodo->n; ++i)
        nodo->claves[i-1] = nodo->claves[i];

    for (int i=idx+2; i<=nodo->n; ++i)
        nodo->hijos[i-1] = nodo->hijos[i];

    hijo->n += hermano->n+1;
    nodo->n--;

    delete hermano;
}

Avion* ArbolB::obtenerAntecesor(NodoArbolB* nodo, int idx) {
    NodoArbolB *cur = nodo->hijos[idx];
    while (!cur->hoja)
        cur = cur->hijos[cur->n];
    return cur->claves[cur->n-1];
}

Avion* ArbolB::obtenerSucesor(NodoArbolB* nodo, int idx) {
    NodoArbolB *cur = nodo->hijos[idx+1];
    while (!cur->hoja)
        cur = cur->hijos[0];
    return cur->claves[0];
}

void ArbolB::generarReporteAvionesDisponibles() {
        string contenido = "digraph G {\n";
        contenido += "node [shape=record];\n";
        int nodoId = 0;
        generarDOTRecursivo(raiz, contenido, nodoId);
        contenido += "}\n";

        // Generar el archivo .dot
        ofstream dotFile("arbolb.dot");
        if (dotFile.is_open()) {
            dotFile << contenido;
            dotFile.close();

            // Generar la imagen PNG usando Graphviz
            string cmd = "dot -Tpng arbolb.dot -o arbolb.png";
            system(cmd.c_str());
        }
}

void ArbolB::generarDOTRecursivo(NodoArbolB* nodo, string& contenido, int& nodoId) {
        if (nodo == nullptr) return;
        string nodoNombre = "nodo" + to_string(nodoId++);
        
        // Crear la etiqueta del nodo
        contenido += nodoNombre + " [label=\"";
        for (int i = 0; i < nodo->n; i++) {
            if (i > 0) contenido += "|";
            contenido += "<p" + to_string(i) + "> |";
            contenido += " " + nodo->claves[i]->getNumeroDeRegistro() + " ";
        }
        contenido += "|<p" + to_string(nodo->n) + ">\"];\n";

        // Crear las conexiones a los hijos
        for (int i = 0; i <= nodo->n; i++) {
            if (nodo->hijos[i] != nullptr) {
                string hijoNombre = "nodo" + to_string(nodoId);
                contenido += nodoNombre + ":p" + to_string(i) + " -> " + hijoNombre + ";\n";
                generarDOTRecursivo(nodo->hijos[i], contenido, nodoId);
            }
        }
    }

#endif // ARBOLB_H