#ifndef RUTA_H
#define RUTA_H

#include <string>

class Ruta {
public:
    Ruta(const std::string &origen, const std::string &destino, int distancia)
        : origen(origen), destino(destino), distancia(distancia) {}

    std::string getOrigen() const { return origen; }
    std::string getDestino() const { return destino; }
    int getDistancia() const { return distancia; }

    // Destructor
    ~Ruta() {}

private:
    std::string origen;
    std::string destino;
    int distancia;
};

#endif // RUTA_H