#ifndef LISTA_ADYACENCIA_H
#define LISTA_ADYACENCIA_H

#include <iostream>
#include "fstream"
#include <sstream>
#include <vector>
#include <string>

const int INFINITO = 1e9; // Representa un valor "infinito" grande

struct Arista {
    std::string destino;
    int distancia;

    Arista(std::string dest, int dist) : destino(dest), distancia(dist) {}
};

struct NodoLA {
    std::string origen;
    std::vector<Arista> aristas;

    NodoLA(std::string orig) : origen(orig) {}
};

class ListaAdyacencia {
private:
    std::vector<NodoLA> nodos;

    NodoLA* encontrarNodoLA(const std::string& origen) {
        for (auto& nodo : nodos) {
            if (nodo.origen == origen) {
                return &nodo;
            }
        }
        return nullptr;
    }

    int encontrarIndiceNodoLA(const std::string& nombre) const {
        for (int i = 0; i < nodos.size(); ++i) {
            if (nodos[i].origen == nombre) {
                return i;
            }
        }
        return -1; // No encontrado
    }

    std::pair<std::vector<std::string>, int> buscarRutaMinima(int indiceActual, int indiceDestino, int distanciaAcumulada, std::vector<int>& distanciasMinimas, std::vector<std::string>& rutaActual) const {
        if (indiceActual == indiceDestino) {
            rutaActual.push_back(nodos[indiceActual].origen);
            return {rutaActual, distanciaAcumulada};
        }
        if (distanciaAcumulada >= distanciasMinimas[indiceActual]) {
            return {{}, INFINITO};
        }
        distanciasMinimas[indiceActual] = distanciaAcumulada;
        std::pair<std::vector<std::string>, int> mejorRuta = {{}, INFINITO};

        for (const auto& arista : nodos[indiceActual].aristas) {
            int indiceDestinoArista = encontrarIndiceNodoLA(arista.destino);
            if (indiceDestinoArista != -1) {
                std::vector<std::string> rutaSiguiente = rutaActual;
                rutaSiguiente.push_back(nodos[indiceActual].origen);
                auto resultado = buscarRutaMinima(indiceDestinoArista, indiceDestino, distanciaAcumulada + arista.distancia, distanciasMinimas, rutaSiguiente);
                if (resultado.second < mejorRuta.second) {
                    mejorRuta = resultado;
                }
            }
        }

        return mejorRuta;
    }

public:
    void agregarArista(const std::string& origen, const std::string& destino, int distancia) {
        NodoLA* nodoOrigen = encontrarNodoLA(origen);
        if (nodoOrigen == nullptr) {
            nodos.push_back(NodoLA(origen));
            nodoOrigen = &nodos.back();
        }
        nodoOrigen->aristas.emplace_back(destino, distancia);
    }

    void mostrarLista() const {
        for (const auto& nodo : nodos) {
            std::cout << "Origen: " << nodo.origen << " -> ";
            for (const auto& arista : nodo.aristas) {
                std::cout << "(Destino: " << arista.destino << ", Distancia: " << arista.distancia << ") ";
            }
            std::cout << std::endl;
        }
    }

    std::pair<std::vector<std::string>, int> rutaCorta(const std::string& origen, const std::string& destino) const {
        int indiceOrigen = encontrarIndiceNodoLA(origen);
        int indiceDestino = encontrarIndiceNodoLA(destino);
        if (indiceOrigen == -1 || indiceDestino == -1) return {{}, -1};

        std::vector<int> distanciasMinimas(nodos.size(), INFINITO);
        std::vector<std::string> ruta;
        auto resultado = buscarRutaMinima(indiceOrigen, indiceDestino, 0, distanciasMinimas, ruta);
        if (resultado.second == INFINITO) return {{}, -1};
        return resultado;
    }

    void generarGrafoGraphviz() const {
        std::ofstream archivo("grafoDirigido.dot");
        
        // Inicio del grafo dirigido en Graphviz
        archivo << "digraph G {" << std::endl;

        // Recorrer todos los nodos y sus aristas para generar las instrucciones de Graphviz
        for (const auto& nodo : nodos) {
            for (const auto& arista : nodo.aristas) {
                archivo << "    \"" << nodo.origen << "\" -> \"" << arista.destino << "\" [label=\"" << arista.distancia << "\"];" << std::endl;
            }
        }

        // Fin del grafo dirigido
        archivo << "}" << std::endl;

        archivo.close();

        // Generar el archivo de imagen con Graphviz
        system("dot -Tpng grafoDirigido.dot -o grafoDirigido.png");

        // Abrir la imagen generada
        system("grafoDirigido.png");
    }
};

#endif // LISTA_ADYACENCIA_H