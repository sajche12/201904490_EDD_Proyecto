#ifndef PILOTO_H
#define PILOTO_H

#include <string>

class Piloto
{
public:
    Piloto(const std::string &nombre, const std::string &nacionalidad, const std::string &numeroId, const std::string &vuelo, int horasVuelo, const std::string &tipoLicencia)
        : nombre(nombre), nacionalidad(nacionalidad), numeroId(numeroId), vuelo(vuelo), horasVuelo(horasVuelo), tipoLicencia(tipoLicencia) {}

    std::string getNombre() const { return nombre; }
    std::string getNacionalidad() const { return nacionalidad; }
    std::string getNumeroId() const { return numeroId; }
    std::string getVuelo() const { return vuelo; }
    int getHorasVuelo() const { return horasVuelo; }
    std::string getTipoLicencia() const { return tipoLicencia; }

    // Sobrecarga del operador 
    bool operator<(const Piloto& otro) const {
        return horasVuelo < otro.horasVuelo;
    }

    bool operator>(const Piloto& otro) const {
        return horasVuelo > otro.horasVuelo;
    }

    // Destructor
    ~Piloto() {}

private:
    std::string nombre;
    std::string nacionalidad;
    std::string numeroId;
    std::string vuelo;
    int horasVuelo;
    std::string tipoLicencia;
};

#endif // PILOTO_H