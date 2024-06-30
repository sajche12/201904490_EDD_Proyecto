#ifndef MATRIZ_DISPERSA_H
#define MATRIZ_DISPERSA_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "avion.h"
#include "piloto.h"

using namespace std;

class NodoMD {
public:
    string vuelo;
    string ciudad_destino;
    string numero_de_id;
    NodoMD* derecha;
    NodoMD* abajo;

    NodoMD(string vuelo, string ciudad_destino, string numero_de_id)
        : vuelo(vuelo), ciudad_destino(ciudad_destino), numero_de_id(numero_de_id), derecha(nullptr), abajo(nullptr) {}
};

class MatrizDispersa {
private:
    NodoMD* origen;

public:
    MatrizDispersa() : origen(nullptr) {}

    void insertarAvion(const Avion& avion) {
        NodoMD* fila = insertarFila(avion.getVuelo());
        NodoMD* nodo = new NodoMD(avion.getVuelo(), avion.getCiudadDestino(), "");
        
        // Insertar el nodo en la fila
        if (!fila->derecha) {
            fila->derecha = nodo;
        } else {
            NodoMD* actual = fila;
            while (actual->derecha) {
                actual = actual->derecha;
            }
            actual->derecha = nodo;
        }
    }

    void insertarPiloto(const Piloto& piloto) {
        NodoMD* fila = buscarFila(piloto.vuelo);
        if (fila) {
            NodoMD* actual = fila->derecha;
            while (actual) {
                if (actual->numero_de_id.empty()) {
                    actual->numero_de_id = piloto.numero_de_id;
                    return;
                }
                actual = actual->derecha;
            }
        }
    }

    void imprimirMatriz() {
        NodoMD* filaActual = origen;
        while (filaActual) {
            NodoMD* columnaActual = filaActual->derecha;
            while (columnaActual) {
                if (!columnaActual->numero_de_id.empty()) {
                    cout << "Vuelo: " << columnaActual->vuelo 
                        << ", Destino: " << columnaActual->ciudad_destino
                        << ", Piloto ID: " << columnaActual->numero_de_id << endl;
                }
                columnaActual = columnaActual->derecha;
            }
            filaActual = filaActual->abajo;
        }
    }

    NodoMD* buscarPorNumeroDeID(const string& numero_de_id) {
        NodoMD* filaActual = origen;
        while (filaActual) {
            NodoMD* columnaActual = filaActual;
            while (columnaActual) {
                if (columnaActual->numero_de_id == numero_de_id) {
                    return columnaActual;
                }
                columnaActual = columnaActual->derecha;
            }
            filaActual = filaActual->abajo;
        }
        return nullptr; // Si no se encuentra el ID, retorna nullptr
    }

    void eliminarPorNumeroDeID(const string& numero_de_id) {
        NodoMD* filaActual = origen;
        NodoMD* nodoAEliminar = nullptr;

        while (filaActual) {
            NodoMD* columnaActual = filaActual;
            NodoMD* anterior = nullptr;
            while (columnaActual) {
                if (columnaActual->numero_de_id == numero_de_id) {
                    nodoAEliminar = columnaActual;
                    if (anterior) {
                        anterior->derecha = columnaActual->derecha;
                    } else {
                        filaActual->derecha = columnaActual->derecha;
                    }
                    break;
                }
                anterior = columnaActual;
                columnaActual = columnaActual->derecha;
            }

            if (nodoAEliminar) {
                delete nodoAEliminar;
                break;
            }
            filaActual = filaActual->abajo;
        }
    }

    void generarGraficoGraphviz() {
        ofstream archivo("matriz_dispersa.dot");
        archivo << "digraph G {\n";
        archivo << "node [ shape = box ];\n";
        archivo << "matriz [label=\"\", group = filas ];\n";

        // Recopilar nodos válidos y crear mapeos
        vector<NodoMD*> nodosValidos;
        map<string, int> vueloIndice;
        map<string, int> destinoIndice;
        
        NodoMD* filaActual = origen;
        int contadorFila = 1;
        int contadorColumna = 1;
        
        while (filaActual) {
            NodoMD* columnaActual = filaActual->derecha;
            while (columnaActual) {
                if (!columnaActual->vuelo.empty() && !columnaActual->ciudad_destino.empty() && !columnaActual->numero_de_id.empty()) {
                    nodosValidos.push_back(columnaActual);
                    if (vueloIndice.find(columnaActual->vuelo) == vueloIndice.end()) {
                        vueloIndice[columnaActual->vuelo] = contadorFila++;
                    }
                    if (destinoIndice.find(columnaActual->ciudad_destino) == destinoIndice.end()) {
                        destinoIndice[columnaActual->ciudad_destino] = contadorColumna++;
                    }
                }
                columnaActual = columnaActual->derecha;
            }
            filaActual = filaActual->abajo;
        }

        // Nodos de filas (vuelos)
        for (const auto& vuelo : vueloIndice) {
            archivo << "U" << vuelo.second << "[label = \"" << vuelo.first << "\" , group = filas ];\n";
        }

        // Conexiones entre filas
        archivo << "matriz -> U1 [dir=both] ;\n";
        for (int i = 1; i < contadorFila - 1; i++) {
            archivo << "U" << i << " -> U" << (i + 1) << " [dir=both]  ;  \n";
        }

        // Nodos de columnas (ciudades destino)
        for (const auto& destino : destinoIndice) {
            archivo << "A" << destino.second << "[label = \"" << destino.first << "\" , group = " << destino.second << " ];\n";
        }

        // Conexiones entre columnas
        archivo << "matriz -> A1 [dir=both] ;\n";
        for (int i = 1; i < contadorColumna - 1; i++) {
            archivo << "A" << i << " -> A" << (i + 1) << " [dir=both] ;\n";
        }

        // Rank same para matriz y columnas
        archivo << "{ rank = same; matriz;\n";
        for (int i = 1; i < contadorColumna; i++) {
            archivo << "A" << i << ";\n";
        }
        archivo << "}\n";

        // Nodos de datos (número de ID)
        for (const auto& nodo : nodosValidos) {
            int filaIndex = vueloIndice[nodo->vuelo];
            int columnaIndex = destinoIndice[nodo->ciudad_destino];
            archivo << "X" << columnaIndex << "Y" << filaIndex << " [ label = \"" << nodo->numero_de_id 
                    << "\" style=filled group = " << columnaIndex << " ] ;\n";
        }

        // Conexiones entre nodos de datos
        for (int i = 0; i < nodosValidos.size(); i++) {
            for (int j = i + 1; j < nodosValidos.size(); j++) {
                if (nodosValidos[i]->vuelo == nodosValidos[j]->vuelo) {
                    int filaIndex = vueloIndice[nodosValidos[i]->vuelo];
                    int columnaIndex1 = destinoIndice[nodosValidos[i]->ciudad_destino];
                    int columnaIndex2 = destinoIndice[nodosValidos[j]->ciudad_destino];
                    archivo << "X" << columnaIndex1 << "Y" << filaIndex << " -> X" << columnaIndex2 << "Y" << filaIndex << " [dir=both] ;\n";
                }
                if (nodosValidos[i]->ciudad_destino == nodosValidos[j]->ciudad_destino) {
                    int columnaIndex = destinoIndice[nodosValidos[i]->ciudad_destino];
                    int filaIndex1 = vueloIndice[nodosValidos[i]->vuelo];
                    int filaIndex2 = vueloIndice[nodosValidos[j]->vuelo];
                    archivo << "X" << columnaIndex << "Y" << filaIndex1 << " -> X" << columnaIndex << "Y" << filaIndex2 << " [dir=both] ;\n";
                }
            }
        }

        // Conexiones entre filas y nodos de datos
        for (const auto& nodo : nodosValidos) {
            int filaIndex = vueloIndice[nodo->vuelo];
            int columnaIndex = destinoIndice[nodo->ciudad_destino];
            // Opción 1: Arista bidireccional
            //archivo << "U" << filaIndex << " -> X" << columnaIndex << "Y" << filaIndex << " [dir=both] ;\n";
            // Opción 2: Dos aristas separadas
            archivo << "U" << filaIndex << " -> X" << columnaIndex << "Y" << filaIndex << " ;\n";
            archivo << "X" << columnaIndex << "Y" << filaIndex << " -> U" << filaIndex << " ;\n";
        }

        // Conexiones entre columnas y nodos de datos
        for (const auto& nodo : nodosValidos) {
            int filaIndex = vueloIndice[nodo->vuelo];
            int columnaIndex = destinoIndice[nodo->ciudad_destino];
            // Opción 1: Arista bidireccional
            //archivo << "A" << columnaIndex << " -> X" << columnaIndex << "Y" << filaIndex << " [dir=both] ;\n";
            // Opción 2: Dos aristas separadas
            archivo << "A" << columnaIndex << " -> X" << columnaIndex << "Y" << filaIndex << " ;\n";
            archivo << "X" << columnaIndex << "Y" << filaIndex << " -> A" << columnaIndex << " ;\n";
        }

        // Rank same para filas y nodos de datos
        for (const auto& vuelo : vueloIndice) {
            archivo << "{rank = same ; U" << vuelo.second << " ;\n";
            for (const auto& nodo : nodosValidos) {
                if (nodo->vuelo == vuelo.first) {
                    int columnaIndex = destinoIndice[nodo->ciudad_destino];
                    archivo << "X" << columnaIndex << "Y" << vuelo.second << " ;";
                }
            }
            archivo << "}\n";
        }

        archivo << "}\n";
        archivo.close();
        
        // Generar la imagen
        system("dot -Tpng matriz_dispersa.dot -o matriz_dispersa.png");
        cout << endl;
    }

private:
    NodoMD* insertarFila(const string& vuelo) {
        if (!origen || vuelo < origen->vuelo) {
            NodoMD* nuevo = new NodoMD(vuelo, "", "");
            nuevo->abajo = origen;
            origen = nuevo;
            return nuevo;
        } else {
            NodoMD* actual = origen;
            while (actual->abajo && actual->abajo->vuelo <= vuelo) {
                actual = actual->abajo;
            }
            if (actual->vuelo != vuelo) {
                NodoMD* nuevo = new NodoMD(vuelo, "", "");
                nuevo->abajo = actual->abajo;
                actual->abajo = nuevo;
                return nuevo;
            }
            return actual;
        }
    }

    NodoMD* insertarColumna(const string& ciudad_destino) {
        if (!origen || ciudad_destino < origen->ciudad_destino) {
            NodoMD* nuevo = new NodoMD("", ciudad_destino, "");
            nuevo->derecha = origen;
            origen = nuevo;
            return nuevo;
        } else {
            NodoMD* actual = origen;
            while (actual->derecha && actual->derecha->ciudad_destino <= ciudad_destino) {
                actual = actual->derecha;
            }
            if (actual->ciudad_destino != ciudad_destino) {
                NodoMD* nuevo = new NodoMD("", ciudad_destino, "");
                nuevo->derecha = actual->derecha;
                actual->derecha = nuevo;
                return nuevo;
            }
            return actual;
        }
    }

    void insertarNodoEnFila(const string& vuelo, const string& ciudad_destino) {
        NodoMD* fila = buscarFila(vuelo);
        if (!fila) return;

        NodoMD* actual = fila;
        while (actual->derecha && actual->derecha->ciudad_destino < ciudad_destino) {
            actual = actual->derecha;
        }

        if (actual->ciudad_destino != ciudad_destino) {
            NodoMD* nuevo = new NodoMD(vuelo, ciudad_destino, "");
            nuevo->derecha = actual->derecha;
            actual->derecha = nuevo;
        }
    }

    NodoMD* buscarFila(const string& vuelo) {
        NodoMD* actual = origen;
        while (actual && actual->vuelo < vuelo) {
            actual = actual->abajo;
        }
        return (actual && actual->vuelo == vuelo) ? actual : nullptr;
    }

    void insertarNodo(const string& vuelo, const string& ciudad_destino, const string& numero_de_id) {
        NodoMD* fila = insertarFila(vuelo);
        NodoMD* columna = insertarColumna(ciudad_destino);
        
        NodoMD* nuevo = new NodoMD(vuelo, ciudad_destino, numero_de_id);
        
        // Insertar en la fila
        NodoMD* actual = fila;
        while (actual->derecha && actual->derecha->ciudad_destino < ciudad_destino) {
            actual = actual->derecha;
        }
        nuevo->derecha = actual->derecha;
        actual->derecha = nuevo;
        
        // Insertar en la columna
        actual = columna;
        while (actual->abajo && actual->abajo->vuelo < vuelo) {
            actual = actual->abajo;
        }
        nuevo->abajo = actual->abajo;
        actual->abajo = nuevo;
    }

    NodoMD* buscarNodo(const string& vuelo, const string& ciudad_destino) {
        NodoMD* fila = buscarFila(vuelo);
        if (!fila) return nullptr;
        
        NodoMD* actual = fila;
        while (actual && actual->ciudad_destino != ciudad_destino) {
            actual = actual->derecha;
        }
        return actual;
    }

    NodoMD* buscarNodoVacioEnFila(NodoMD* fila) {
        NodoMD* actual = fila;
        while (actual) {
            if (actual->numero_de_id.empty()) {
                return actual;
            }
            actual = actual->derecha;
        }
        return nullptr;
    }
};

#endif // MATRIZ_DISPERSA_H