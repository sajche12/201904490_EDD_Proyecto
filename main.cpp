// Definicion de librerias
#include <iostream>
#include <string>
#include <fstream>
#include "jsoncpp.cpp"
#include "json/json.h"
#include "avion.h"
#include "piloto.h"
#include "listaCircularDoble.h"
#include "arbolB.h"
#include "arbolBB.h"
#include "listaAdyacencia.h"
#include "ruta.h"

using namespace std;

// Prototipo de funciones
void cargarAviones();
void cargarPilotos();
void cargarRutas();
void cargarMovimientos();
int contarLineas();
void consultarHorasVuelo();
void recomendarRuta();
void visualizarReportes();

// Variables globales
//NodoArbolB* raiz = nullptr;
ListaCircularDoble<Avion> listaAvionesMantenimiento;
ArbolBinarioBusqueda<Piloto> arbolPilotos;

// Funcion principal
int main() {
    int opcion;
    do {
        cout << "--------------------------------------------" << endl;
        cout << "|           **MENU PRINCIPAL**              |" << endl;
        cout << "| 1. Carga de Aviones                       |" << endl;
        cout << "| 2. Carga de Pilotos                       |" << endl;
        cout << "| 3. Carga de Rutas                         |" << endl;
        cout << "| 4. Carga de Movimientos                   |" << endl;
        cout << "| 5. Consulta de horas de vuelo (pilotos)   |" << endl;
        cout << "| 6. Recomendar ruta                        |" << endl;
        cout << "| 7. Visualizar Reportes                    |" << endl;
        cout << "| 8. Salir                                  |" << endl;
        cout << "--------------------------------------------" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        switch (opcion) {
            case 1:
                cargarAviones();
                break;
            case 2:
                cargarPilotos();
                break;
            case 3:
                cargarRutas();
                break;
            case 4:
                cargarMovimientos();
                break;
            case 5:
                consultarHorasVuelo();
                break;
            case 6:
                recomendarRuta();
                break;
            case 7:
                visualizarReportes();
                break;
            case 8:
                cout << "Saliendo..." << endl;
                break;
            default:
                cout << "Opcion no valida" << endl;
                break;
        }
    } while (opcion != 8);
    return 0;
}

// Definicion de funciones
void cargarAviones() {
    // Nombre del archivo JSON avionNuevo leer
    string filename = "aviones.json";

    // Crear un objeto ifstream para leer el archivo
    ifstream ifs(filename, ifstream::binary);

    // Verificar si el archivo se abrió correctamente
    if (!ifs.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        //return 1;
    }

    // Crear un objeto Json::Value para almacenar los datos JSON
    Json::Value root;

    // Crear un objeto Json::CharReaderBuilder para parsear el JSON
    Json::CharReaderBuilder builder;
    string errs;

    // Parsear el archivo JSON
    bool parsingSuccessful = Json::parseFromStream(builder, ifs, &root, &errs);
    if (!parsingSuccessful) {
        cerr << "Error al parsear el archivo JSON: " << errs << endl;
        //return 1;
    }

    // Cerrar el archivo
    ifs.close();

    for (const auto& avion : root) {
        Avion avionNuevo(avion["vuelo"].asString(), avion["numero_de_registro"].asString(), avion["modelo"].asString(), avion["capacidad"].asInt(), avion["aerolinea"].asString(), avion["ciudad_destino"].asString(), avion["estado"].asString());

        // Agregar el avión avionNuevo a la lista de aviones verificando su estado
        if (avionNuevo.getEstado() == "Disponible") {
            //arbolDisponible()
        } else if (avionNuevo.getEstado() == "Mantenimiento") {
            listaAvionesMantenimiento.insertar(avionNuevo);
        }
    }
    cout << "\nAviones cargados exitosamente.\n" << endl;
}

void cargarPilotos() {
    // Nombre del archivo JSON avionNuevo leer
    string filename = "pilotos.json";

    // Crear un objeto ifstream para leer el archivo
    ifstream ifs(filename, ifstream::binary);

    // Verificar si el archivo se abrió correctamente
    if (!ifs.is_open()) {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        //return 1;
    }

    // Crear un objeto Json::Value para almacenar los datos JSON
    Json::Value root;

    // Crear un objeto Json::CharReaderBuilder para parsear el JSON
    Json::CharReaderBuilder builder;
    string errs;

    // Parsear el archivo JSON
    bool parsingSuccessful = Json::parseFromStream(builder, ifs, &root, &errs);
    if (!parsingSuccessful) {
        cerr << "Error al parsear el archivo JSON: " << errs << endl;
        //return 1;
    }

    // Cerrar el archivo
    ifs.close();

    for (const auto& piloto : root) {
        Piloto pilotoNuevo(piloto["nombre"].asString(), piloto["nacionalidad"].asString(), piloto["numero_id"].asString(), piloto["vuelo"].asString(), piloto["horas_vuelo"].asInt(), piloto["tipo_licencia"].asString());
        arbolPilotos.insertar(pilotoNuevo);
    }

    cout << "\nPilotos cargados exitosamente.\n" << endl;
}

void cargarRutas() {
    ifstream file("rutas.txt");
    string linea;
    int contador_lineas = contarLineas();

    if (file.is_open()) {
        ListaAdyacencia listaRutas(contador_lineas);
        cout << "\nMovimientos realizados:" << endl;
        while (getline(file, linea)) {
            // Extraer la palabra que se encuentra antes de /
            string origen = linea.substr(0, linea.find("/"));
            // Extraer la palabra que se encuentra entre / y /
            string destino = linea.substr(linea.find("/") + 1, linea.find_last_of("/") - linea.find("/") - 1);
            // Extraer la palabra que se encuentra despues de / y / y convertirla a entero
            int distancia = stoi(linea.substr(linea.find_last_of("/") + 1));
            
            // Agregar la arista a la lista de adyacencia
            listaRutas.agregarArista(origen, destino, distancia);
        }
    }
    file.close();
    cout << "\nRutas cargadas exitosamente.\n" << endl;
}

int contarLineas() {
    ifstream file("rutas.txt");
    string linea;
    int contador = 0;

    if (file.is_open()) {
        while (getline(file, linea)) {
            contador++;
        }
    }
    file.close();
    return contador;
}

void cargarMovimientos() {
    cout << "Cargar movimientos" << endl;
}

void consultarHorasVuelo() {
    cout << "Consultar horas de vuelo" << endl;
}

void recomendarRuta() {
    cout << "Ingrese los siguientes datos: " << endl;
    string origen, destino;
    cout << "Origen: ";
    cin >> origen;
    cout << "Destino: ";
    cin >> destino;

    
    
}

void visualizarReportes() {
    cout << "Visualizar reportes" << endl;
}