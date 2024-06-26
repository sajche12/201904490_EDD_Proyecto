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
#include "tablaHash.h"
#include "matrizDispersa.h"

using namespace std;

// Prototipo de funciones
void cargarAviones();
void cargarPilotos();
void cargarRutas();
void cargarMovimientos();
void consultarHorasVuelo();
void recomendarRuta();
void visualizarReportes();
PilotoBB convertirPilotoBB(const Piloto &piloto);
void reporteArbolBinarioBusqueda();

// Variables globales
ListaCircularDoble listaAvionesMantenimiento;
ArbolBinarioBusqueda arbolPilotos;
ListaAdyacencia listaRutas;
ArbolB arbolDisponible;
TablaHash tablaPilotos(18);
MatrizDispersa matrizVuelosCiudades;

// Funcion principal
int main()
{
    int opcion;
    do
    {
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
        switch (opcion)
        {
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
void cargarAviones()
{
    // Nombre del archivo JSON avionNuevo leer
    string filename = "aviones.json";

    // Crear un objeto ifstream para leer el archivo
    ifstream ifs(filename, ifstream::binary);

    // Verificar si el archivo se abrió correctamente
    if (!ifs.is_open())
    {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        // return 1;
    }

    // Crear un objeto Json::Value para almacenar los datos JSON
    Json::Value root;

    // Crear un objeto Json::CharReaderBuilder para parsear el JSON
    Json::CharReaderBuilder builder;
    string errs;

    // Parsear el archivo JSON
    bool parsingSuccessful = Json::parseFromStream(builder, ifs, &root, &errs);
    if (!parsingSuccessful)
    {
        cerr << "Error al parsear el archivo JSON: " << errs << endl;
        // return 1;
    }

    // Cerrar el archivo
    ifs.close();

    for (const auto &avion : root)
    {
        Avion avionNuevo(avion["vuelo"].asString(), avion["numero_de_registro"].asString(), avion["modelo"].asString(), avion["capacidad"].asInt(), avion["aerolinea"].asString(), avion["ciudad_destino"].asString(), avion["estado"].asString());

        // Agregar el avión avionNuevo a la lista de aviones verificando su estado
        if (avionNuevo.getEstado() == "Disponible") {
            
            Avion* avionNuevoDisponible = new Avion(avion["vuelo"].asString(), avion["numero_de_registro"].asString(), avion["modelo"].asString(), avion["capacidad"].asInt(), avion["aerolinea"].asString(), avion["ciudad_destino"].asString(), avion["estado"].asString());
            
            arbolDisponible.insertar(avionNuevoDisponible);
            matrizVuelosCiudades.insertarAvion(avionNuevo);
            
        }
        else if (avionNuevo.getEstado() == "Mantenimiento")
        {
            Avion* avionNuevoMantenimiento = new Avion(avion["vuelo"].asString(), avion["numero_de_registro"].asString(), avion["modelo"].asString(), avion["capacidad"].asInt(), avion["aerolinea"].asString(), avion["ciudad_destino"].asString(), avion["estado"].asString());
            listaAvionesMantenimiento.agregarAvion(avionNuevo);
            matrizVuelosCiudades.insertarAvion(avionNuevo);
        }
    }
    cout << "\nAviones cargados exitosamente.\n" << endl;
}

void cargarPilotos()
{
    // Nombre del archivo JSON avionNuevo leer
    string filename = "pilotos.json";

    // Crear un objeto ifstream para leer el archivo
    ifstream ifs(filename, ifstream::binary);

    // Verificar si el archivo se abrió correctamente
    if (!ifs.is_open())
    {
        cerr << "No se pudo abrir el archivo: " << filename << endl;
        // return 1;
    }

    // Crear un objeto Json::Value para almacenar los datos JSON
    Json::Value root;

    // Crear un objeto Json::CharReaderBuilder para parsear el JSON
    Json::CharReaderBuilder builder;
    string errs;

    // Parsear el archivo JSON
    bool parsingSuccessful = Json::parseFromStream(builder, ifs, &root, &errs);
    if (!parsingSuccessful)
    {
        cerr << "Error al parsear el archivo JSON: " << errs << endl;
        // return 1;
    }

    // Cerrar el archivo
    ifs.close();

    // Recorrer el JSON y agregar los pilotos al árbol de búsqueda binaria
    for (const auto &piloto : root)
    {
        string nombre = piloto["nombre"].asString();
        string nacionalidad = piloto["nacionalidad"].asString();
        string numeroId = piloto["numero_de_id"].asString();
        string vuelo = piloto["vuelo"].asString();
        int horasVuelo = piloto["horas_de_vuelo"].asInt();
        string tipoLicencia = piloto["tipo_de_licencia"].asString();

        // Creacion de objeto Piloto y conversion a PilotoBB
        Piloto pilotoNuevo(nombre, nacionalidad, numeroId, vuelo, horasVuelo, tipoLicencia);
        Piloto* pilotoNuevoPtr = new Piloto(nombre, nacionalidad, numeroId, vuelo, horasVuelo, tipoLicencia);
        PilotoBB pilotoNuevoBB = convertirPilotoBB(pilotoNuevo);
        arbolPilotos.insertar(pilotoNuevoBB);
        tablaPilotos.insertar(pilotoNuevoPtr);
        matrizVuelosCiudades.insertarPiloto(pilotoNuevo);
    }

    cout << "\nPilotos cargados exitosamente.\n"
        << endl;
}

PilotoBB convertirPilotoBB(const Piloto &piloto)
{
    PilotoBB pilotoBB;
    pilotoBB.nombre = piloto.nombre;
    pilotoBB.nacionalidad = piloto.nacionalidad;
    pilotoBB.numero_de_id = piloto.numero_de_id;
    pilotoBB.vuelo = piloto.vuelo;
    pilotoBB.horas_de_vuelo = piloto.horas_de_vuelo;
    pilotoBB.tipo_de_licencia = piloto.tipo_de_licencia;
    return pilotoBB;
}

void cargarRutas()
{
    ifstream file("rutas.txt");
    string linea;

    if (file.is_open())
    {
        cout << "\nMovimientos realizados:" << endl;
        while (getline(file, linea))
        {
            // Extraer la palabra que se encuentra antes de /
            string origen = linea.substr(0, linea.find("/"));
            // Extraer la palabra que se encuentra entre / y /
            string destino = linea.substr(linea.find("/") + 1, linea.find_last_of("/") - linea.find("/") - 1);
            // Extraer la palabra que se encuentra despues de / y / y convertirla a entero
            int distancia = stoi(linea.substr(linea.find_last_of("/") + 1));
            cout << "Origen: " << origen << ", Destino: " << destino << ", Distancia: " << distancia << endl;

            // Agregar la arista a la lista de adyacencia
            listaRutas.agregarArista(origen, destino, distancia);
        }
    }
    file.close();
    cout << "\nRutas cargadas exitosamente.\n"
        << endl;
}

void cargarMovimientos() {
    ifstream file("movimientos.txt");
    string linea;

    if (file.is_open()) {
        cout << "\nMovimientos realizados:" << endl;
        while (getline(file, linea)) {
            // Buscar el texto "MantenimientoAviones,Ingreso,"
            if (linea.find("MantenimientoAviones,Ingreso,") != string::npos) {
                // Extraer el número de registro del avión entre la , y el ;
                string ingreso = linea.substr(linea.find(",") + 8 + 1, linea.find(";") - linea.find(",") - 9);
                // Buscar el avion en el arbol de aviones disponibles e ingresarlo a la lista de mantenimiento
                Avion* avion = arbolDisponible.buscar(ingreso);
                if (avion != nullptr) {
                    Avion* avionNuevo = new Avion(avion->getVuelo(), avion->getNumeroDeRegistro(), avion->getModelo(), avion->getCapacidad(), avion->getAerolinea(), avion->getCiudadDestino(), "Mantenimiento");
                    listaAvionesMantenimiento.agregarAvion(*avionNuevo);
                    arbolDisponible.eliminar(ingreso);
                }
                cout << "El avion con numero de registro " << ingreso << " ha ingresado a mantenimiento" << endl;
            } else if (linea.find("MantenimientoAviones,Salida,") != string::npos) {
                // Extraer el número de registro del avión
                string salida = linea.substr(linea.find(",") + 8, linea.find(";") - linea.find(",") - 9 + 1);
                // Buscar el avion en la lista de mantenimiento e ingresarlo al arbol de aviones disponibles
                Avion* avion = listaAvionesMantenimiento.obtenerPorNumeroDeRegistro(salida);
                if (avion != nullptr) {
                    Avion* avionSale = new Avion(avion->getVuelo(), avion->getNumeroDeRegistro(), avion->getModelo(), avion->getCapacidad(), avion->getAerolinea(), avion->getCiudadDestino(), "Disponible");
                    arbolDisponible.insertar(avionSale);
                    listaAvionesMantenimiento.eliminarPorNumeroDeRegistro(salida);
                }
                cout << "El avion con numero de registro " << salida << " ha salido de mantenimiento" << endl;
            } // Extrae el texto dentro de los parentesis de DarDeBaja();
            else if (linea.find("DarDeBaja(") != string::npos) {
                // Extraer el número de id del piloto
                string numeroDeId = linea.substr(linea.find("(") + 1, linea.find(")") - linea.find("(") - 1);
                // Buscar el piloto en la tabla hash, arbol binario y matriz dispersa y eliminarlo
                tablaPilotos.eliminar(numeroDeId);
                arbolPilotos.eliminarPiloto(numeroDeId);
                matrizVuelosCiudades.eliminarPorNumeroDeID(numeroDeId);
                cout << "El pasajero con numero de id " << numeroDeId << " ha sido dado de baja" << endl;
            }
        }
    }
    file.close();
    cout << "\nMovimientos cargados exitosamente.\n" << endl;
}

void consultarHorasVuelo()
{
    int opcion;
    do
    {
        cout << "**********************************" << endl;
        cout << "**    Seleccione el recorrido   **" << endl;
        cout << "**1. Pre-Orden                  **" << endl;
        cout << "**2. In-Orden                   **" << endl;
        cout << "**3. Post-Orden                 **" << endl;
        cout << "**4. Regresar al Menu Principal **" << endl;
        cout << "**********************************" << endl;
        cout << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            cout << "Recorrido Pre-Orden:" << endl;
            arbolPilotos.mostrarPreOrden();
            cout << endl;
            break;
        case 2:
            cout << "Recorrido In-Orden:" << endl;
            arbolPilotos.mostrarEnOrden();
            cout << endl;
            break;
        case 3:
            cout << "Recorrido Post-Orden:" << endl;
            arbolPilotos.mostrarPostOrden();
            cout << endl;
            break;
        case 4:
            break;
        default:
            cout << "Opcion no valida" << endl;
            cout << endl;
            break;
        }
    } while (opcion != 4);
}

void recomendarRuta()
{
    cout << "Ingrese los siguientes datos: " << endl;
    string origen, destino;
    cout << "Origen: ";
    cin >> origen;
    cin.clear(); // Limpia errores
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora el resto de la línea
    cout << "Destino: ";
    cin >> destino;
    cin.clear(); // Limpia errores
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignora el resto de la línea

    auto resultado = listaRutas.rutaCorta(origen, destino);
    // Paso 4: Verificar si la ruta es válida
    if (resultado.second != -1)
    {
        cout << endl;
        // Paso 5: Imprimir la ruta y la distancia
        cout << "Ruta más corta de " << origen << " a " << destino << ":\n";
        for (const auto &nodo : resultado.first)
        {
            // Si el nodo es el último, no imprimir la flecha
            if (nodo == resultado.first.back())
            {
                cout << nodo;
            }
            else
            {
                cout << nodo << " -> ";
            }
        }
        cout << "\nDistancia: " << resultado.second << " km" << endl;
    }
    else
    {
        cout << endl;
        cout << "No se encontró ruta de " << origen << " a " << destino << "." << endl;
    }
    cout << endl;
}

void visualizarReportes()
{
    int opcion;
    do
    {
        cout << "*******************************************************" << endl;
        cout << "**             Seleccione un reporte                 **" << endl;
        cout << "**1. Arbol B - Aviones Disponibles                   **" << endl;
        cout << "**2. Lista Circular Doble - Aviones en Mantenimiento **" << endl;
        cout << "**3. Arbol Binario de Busqueda - Horas de Vuelo      **" << endl;
        cout << "**4. Tabla Hash - Pilotos                            **" << endl;
        cout << "**5. Grafo Dirigido - Rutas                          **" << endl;
        cout << "**6. Matriz Dispersa - Vuelos y Ciudades             **" << endl;
        cout << "**7. Regresar al Menu Principal                      **" << endl;
        cout << "*******************************************************" << endl;
        cout << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion)
        {
        case 1:
            arbolDisponible.generarReporteAvionesDisponibles();
            break;
        case 2:
            listaAvionesMantenimiento.generarDot();
            break;
        case 3:
            reporteArbolBinarioBusqueda();
            break;
        case 4:
            tablaPilotos.visualizarTablaComoString();
            break;
        case 5: 
            listaRutas.generarGrafoGraphviz();
            break;
        case 6:
            matrizVuelosCiudades.generarGraficoGraphviz();
            break;
        case 7:
            break;
        default:
            cout << "Opcion no valida" << endl;
            cout << endl;
            break;
        }
    } while (opcion != 7);
    
}

void reporteArbolBinarioBusqueda()
{
    // Generar el contenido Graphviz del árbol de pilotos
    std::string graphvizContent = arbolPilotos.generarGraphviz();
    
    // Escribir el contenido en un archivo .dot
    std::string filename = "arbolPilotos.dot";
    std::ofstream file(filename);
    if (file.is_open()) {
        file << graphvizContent;
        file.close();
    } else {
        std::cerr << "No se pudo abrir el archivo para escribir el Graphviz." << std::endl;
        return;
    }
    
    // Ejecutar el comando dot para generar la imagen PNG del árbol
    std::string dotCommand = "dot -Tpng " + filename + " -o arbolPilotos.png";
    system(dotCommand.c_str());
    
    // Abrir la imagen generada (en Windows)
    system("arbolPilotos.png");
}