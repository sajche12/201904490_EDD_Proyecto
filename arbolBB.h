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

    // Método auxiliar para encontrar el sucesor inorden (el nodo más pequeño en el subárbol derecho)
    NodoBB* minimoNodo(NodoBB* nodo) {
        NodoBB* actual = nodo;
        while (actual && actual->izquierdo != nullptr) {
            actual = actual->izquierdo;
        }
        return actual;
    }

    // Método auxiliar para eliminar un nodo
    NodoBB* eliminarNodo(NodoBB* raiz, const std::string& numero_de_id) {
        if (raiz == nullptr) return raiz;

        // Si el numero_de_id es menor que el del raiz, está en el subárbol izquierdo
        if (numero_de_id < raiz->datos.numero_de_id) {
            raiz->izquierdo = eliminarNodo(raiz->izquierdo, numero_de_id);
        }
        // Si el numero_de_id es mayor que el del raiz, está en el subárbol derecho
        else if (numero_de_id > raiz->datos.numero_de_id) {
            raiz->derecho = eliminarNodo(raiz->derecho, numero_de_id);
        }
        // Si el numero_de_id es igual al del nodo actual, este es el nodo a eliminar
        else {
            // Nodo con solo un hijo o sin hijos
            if (raiz->izquierdo == nullptr) {
                NodoBB* temp = raiz->derecho;
                delete raiz;
                return temp;
            }
            else if (raiz->derecho == nullptr) {
                NodoBB* temp = raiz->izquierdo;
                delete raiz;
                return temp;
            }

            // Nodo con dos hijos: Obtener el sucesor inorden (el menor en el subárbol derecho)
            NodoBB* temp = minimoNodo(raiz->derecho);

            // Copiar los datos del sucesor inorden al nodo actual
            raiz->datos = temp->datos;

            // Eliminar el sucesor inorden
            raiz->derecho = eliminarNodo(raiz->derecho, temp->datos.numero_de_id);
        }
        return raiz;
    }

    // Método auxiliar recursivo para generar la representación de Graphviz
    std::string generarGraphvizRecursivo(NodoBB* nodo) const {
        std::string cadena = "";
        if (nodo != nullptr) {
            // Modificar aquí para incluir nombre y horas_de_vuelo en la etiqueta del nodo
            cadena += "\"" + nodo->datos.numero_de_id + "\" [label=\"" + nodo->datos.nombre + "\\nHoras de vuelo: " + std::to_string(nodo->datos.horas_de_vuelo) + "\"];\n";
            // Si tiene hijo izquierdo, agregar arista y llamar recursivamente
            if (nodo->izquierdo != nullptr) {
                cadena += "\"" + nodo->datos.numero_de_id + "\" -> \"" + nodo->izquierdo->datos.numero_de_id + "\";\n";
                cadena += generarGraphvizRecursivo(nodo->izquierdo);
            }
            // Si tiene hijo derecho, agregar arista y llamar recursivamente
            if (nodo->derecho != nullptr) {
                cadena += "\"" + nodo->datos.numero_de_id + "\" -> \"" + nodo->derecho->datos.numero_de_id + "\";\n";
                cadena += generarGraphvizRecursivo(nodo->derecho);
            }
        }
        return cadena;
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

    // Método público para eliminar un piloto por numero_de_id
    void eliminarPiloto(const std::string& numero_de_id) {
        raiz = eliminarNodo(raiz, numero_de_id);
    }

    // Método público para generar la representación de Graphviz del árbol
    std::string generarGraphviz() const {
        std::string graphviz = "digraph G {\n";
        graphviz += generarGraphvizRecursivo(raiz);
        graphviz += "}\n";
        return graphviz;
    }
};

#endif // ARBOL_BINARIO_H