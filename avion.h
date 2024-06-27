// Avion.h
#ifndef AVION_H // Directivas de preprocesador
#define AVION_H // Directivas de preprocesador

#include <string>

class Avion {
private:
    std::string vuelo;
    std::string numero_de_registro;
    std::string modelo;
    int capacidad;
    std::string aerolinea;
    std::string ciudad_destino;
    std::string estado;

public:
    // Constructor
    Avion(const std::string& vuelo, const std::string& numero_de_registro,
            const std::string& modelo, int capacidad,
            const std::string& aerolinea, const std::string& ciudad_destino,
            const std::string& estado) : vuelo(vuelo), numero_de_registro(numero_de_registro), modelo(modelo), capacidad(capacidad), aerolinea(aerolinea), ciudad_destino(ciudad_destino), estado(estado) {}

    std::string getVuelo() const {
        return vuelo;}
    std::string getNumeroDeRegistro() const {
        return numero_de_registro;}
    std::string getModelo() const {
        return modelo;}
    int getCapacidad() const {
        return capacidad;}
    std::string getAerolinea() const {
        return aerolinea;}
    std::string getCiudadDestino() const {
        return ciudad_destino;}
    std::string getEstado() const {
        return estado;}
    
    // Operador de comparación
    bool operator<(const Avion& otro) const {
        return numero_de_registro < otro.numero_de_registro;
    }

};    
#endif // AVION_H 