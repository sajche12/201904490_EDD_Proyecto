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
    NodoBB* eliminarNodoPorID(NodoBB* nodo, const std::string& numero_de_id) {
        if (nodo == nullptr) {
            return nullptr;
        }

        // Si encontramos el nodo con el ID correcto
        if (nodo->datos.numero_de_id == numero_de_id) {
            // Caso 1: Nodo hoja (sin hijos)
            if (nodo->izquierdo == nullptr && nodo->derecho == nullptr) {
                delete nodo;
                return nullptr;
            }
            // Caso 2: Nodo con un hijo
            else if (nodo->izquierdo == nullptr) {
                NodoBB* temp = nodo->derecho;
                delete nodo;
                return temp;
            }
            else if (nodo->derecho == nullptr) {
                NodoBB* temp = nodo->izquierdo;
                delete nodo;
                return temp;
            }
            // Caso 3: Nodo con dos hijos
            else {
                // Encontrar el sucesor inorden (el nodo más a la izquierda en el subárbol derecho)
                NodoBB* sucesor = nodo->derecho;
                NodoBB* padreSuccessor = nodo;
                while (sucesor->izquierdo != nullptr) {
                    padreSuccessor = sucesor;
                    sucesor = sucesor->izquierdo;
                }

                // Copiar los datos del sucesor al nodo actual
                nodo->datos = sucesor->datos;

                // Eliminar el sucesor
                if (padreSuccessor == nodo) {
                    nodo->derecho = sucesor->derecho;
                } else {
                    padreSuccessor->izquierdo = sucesor->derecho;
                }

                delete sucesor;
                return nodo;
            }
        }
        // Si no es el nodo que buscamos, continuamos la búsqueda en ambos subárboles
        else {
            nodo->izquierdo = eliminarNodoPorID(nodo->izquierdo, numero_de_id);
            nodo->derecho = eliminarNodoPorID(nodo->derecho, numero_de_id);
        }

        return nodo;
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

    // Método auxiliar privado para buscar recursivamente por número de ID
    PilotoBB* buscarPorIDRecursivo(NodoBB* nodo, const std::string& numero_de_id) const {
        if (nodo == nullptr) {
            return nullptr; // No encontrado
        }
        if (nodo->datos.numero_de_id == numero_de_id) {
            return &nodo->datos; // Encontrado
        }
        if (numero_de_id < nodo->datos.numero_de_id) {
            return buscarPorIDRecursivo(nodo->izquierdo, numero_de_id); // Buscar en el subárbol izquierdo
        } else {
            return buscarPorIDRecursivo(nodo->derecho, numero_de_id); // Buscar en el subárbol derecho
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

    // Método público para eliminar un piloto por numero_de_id
    void eliminarPiloto(const std::string& numero_de_id) {
        raiz = eliminarNodoPorID(raiz, numero_de_id);
    }

    // Método público para generar la representación de Graphviz del árbol
    std::string generarGraphviz() const {
        std::string graphviz = "digraph G {\n";
        graphviz += generarGraphvizRecursivo(raiz);
        graphviz += "}\n";
        return graphviz;
    }

    // Método público para buscar un piloto por su número de ID
    PilotoBB* buscarPorID(const std::string& numero_de_id) const {
        return buscarPorIDRecursivo(raiz, numero_de_id);
    }
};

#endif // ARBOL_BINARIO_H