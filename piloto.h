#ifndef PILOTO_H
#define PILOTO_H

#include <string>

class Piloto {
public:
    std::string nombre;
    std::string nacionalidad;
    std::string numero_de_id;
    std::string vuelo;
    int horas_de_vuelo;
    std::string tipo_de_licencia;

    Piloto* siguiente; // Puntero al siguiente piloto en la lista enlazada

    Piloto(const std::string& nombre,
        const std::string& nacionalidad,
        const std::string& numero_de_id,
        const std::string& vuelo,
        int horas_de_vuelo,
        const std::string& tipo_de_licencia)
        : nombre(nombre), nacionalidad(nacionalidad), numero_de_id(numero_de_id),
        vuelo(vuelo), horas_de_vuelo(horas_de_vuelo), tipo_de_licencia(tipo_de_licencia) {}
    
    Piloto() : nombre(""), nacionalidad(""), numero_de_id(""), vuelo(""), horas_de_vuelo(0), tipo_de_licencia(""), siguiente(nullptr) {}
};

#endif // PILOTO_H
