#include <iostream>
#include <string>
#include <fstream>
#include "Grafo.h"
#include "Aeropuerto.h"
#include "Vuelo.h"

#include <list>
#include <set>
#include <map>
#include <queue>
#include <vector>

using namespace std;

int buscar_clave (vector<Aeropuerto> & arreglo, string abuscar)
{
    Aeropuerto aeropuerto;
    string nombre;
    for (vector<Aeropuerto>::const_iterator it = arreglo.begin(); it != arreglo.end(); it++){
        aeropuerto = *it;
        nombre = aeropuerto.devolver_nombre();
        if (nombre == abuscar){
            return aeropuerto.devolver_clave();
        }
    }
}

void Borde()
{
    cout << " " << endl;
    cout << "-----------------------------------------------------------------------------------------------------" << endl;
    cout << " " << endl;
}

void cargar_aeropuertos(string path, Grafo<Vuelo> & grafo, vector<Aeropuerto> & arreglo)
{
    ifstream origen(path.c_str());
    int claves = 0;
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else {
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto = linea.substr(0, primera_pos);
            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_ciudad = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);
            string pais = linea.substr(segunda_pos + 1);

            Aeropuerto nuevo(claves, nombre_aeropuerto, nombre_ciudad, pais);
            arreglo.push_back(nuevo);
            grafo.agregar_vertice(claves);
            claves++;
        }
    }
}

void cargar_aerolineas_rutas(int clave1, int clave2, double distancia, bool cabotaje, string aerolineas, Grafo<Vuelo> & grafo, vector<Aeropuerto> & arreglo)
{
    //Se elimina la llave inicial
    aerolineas = aerolineas.substr(1);
    size_t inicial = 0;
    size_t pos = 0;
    size_t pos_final = aerolineas.find('}');
    while ((pos != std::string::npos) && (pos + 1 < pos_final)) {
        pos = aerolineas.find(',', inicial);
        string dato_aerolinea = aerolineas.substr(inicial, pos - inicial);
        size_t pos_separador = dato_aerolinea.find('-');
        string aerolinea = dato_aerolinea.substr(0, pos_separador);
        string asientos_texto = dato_aerolinea.substr(pos_separador + 1);
        int asientos = atoi(asientos_texto.c_str());

        Vuelo ida (clave1, clave2, asientos, aerolinea, cabotaje, distancia);
        Vuelo vuelta (clave2, clave1, asientos, aerolinea, cabotaje, distancia);
        grafo.agregar_arco(clave1, clave2, ida);
        grafo.agregar_arco(clave2, clave1, vuelta);

        Aeropuerto aeropuerto = arreglo[clave1];
        aeropuerto.agregar_aerolinea (aerolinea);
        arreglo[clave1] = aeropuerto;

        Aeropuerto aeropuerto1 = arreglo[clave2];
        aeropuerto1.agregar_aerolinea (aerolinea);
        arreglo[clave2] = aeropuerto1;

        inicial = pos + 1;
    }
}

void cargar_rutas(string path, Grafo<Vuelo> & grafo, vector<Aeropuerto> & arreglo)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else {
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto_origen = linea.substr(0, primera_pos);

            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_aeropuerto_destino = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);

            size_t tercera_pos = linea.find(',', segunda_pos + 1);
            string distancia_texto = linea.substr(segunda_pos + 1, tercera_pos - segunda_pos - 1);
            double distancia = atof(distancia_texto.c_str());

            size_t cuarta_pos = linea.find(',', tercera_pos + 1);
            string cabotaje_texto = linea.substr(tercera_pos + 1, cuarta_pos - tercera_pos - 1);
            int cabotaje_num = stoi(cabotaje_texto);

            string aerolineas = linea.substr(cuarta_pos + 1);

            bool cabotaje;
            if (cabotaje_num == 0){
                cabotaje = false;
            }else {
                cabotaje = true;
            }

            int clave1 = buscar_clave(arreglo, nombre_aeropuerto_origen);
            int clave2 = buscar_clave(arreglo, nombre_aeropuerto_destino);

            cargar_aerolineas_rutas(clave1, clave2, distancia, cabotaje, aerolineas, grafo, arreglo);
        }
    }
}


void cargar_reservas(string path, Grafo<Vuelo> & grafo, vector<Aeropuerto> & arreglo, list<Vuelo> & reservas)
{
    ifstream origen(path.c_str());
    if (!origen.is_open())
        cout << "No se pudo abrir el archivo: " << path << endl;
    else {
        while (!origen.eof()) {
            string linea;
            getline(origen,linea);
            size_t primera_pos = linea.find(',');
            string nombre_aeropuerto_origen = linea.substr(0, primera_pos);

            size_t segunda_pos = linea.find(',', primera_pos + 1);
            string nombre_aeropuerto_destino = linea.substr(primera_pos + 1, segunda_pos - primera_pos - 1);

            size_t tercera_pos = linea.find(',', segunda_pos + 1);
            string aerolinea = linea.substr(segunda_pos + 1, tercera_pos - segunda_pos - 1);

            string asientos_reservados_texto = linea.substr(tercera_pos + 1);
            int asientos_reservados = atoi(asientos_reservados_texto.c_str());


            int clave1 = buscar_clave(arreglo, nombre_aeropuerto_origen);
            int clave2  = buscar_clave(arreglo, nombre_aeropuerto_destino);


            //Reservas Vuelo Ida
            bool encontrado = false;
            list<Vuelo> listaIda = grafo.costo_arco(clave1, clave2);
            list<Vuelo>::iterator it = listaIda.begin();
            while(it != listaIda.end() && encontrado != true)
            {
                if (it->devolver_aerolinea() == aerolinea)
                {
                    Vuelo vuelo = *it;
                    vuelo.modificar_reserva(asientos_reservados);
                    grafo.modificar_costo_arco(clave1, clave2, vuelo);
                    reservas.push_back(vuelo);
                    encontrado = true;
                }
                it++;
            }
        }
    }
}

void Mostrar_Servicio1(int disponibles, double km, ofstream & archivo1)
{
    cout << "Se ha encontrado un vuelo disponible: " << endl;
    archivo1 << "Se ha encontrado un vuelo disponible: " << endl;
    cout << "El vuelo tiene un recorrido de " << km << "km y hay un total de " << disponibles << " asientos disponibles" << endl;
    archivo1 << "El vuelo tiene un recorrido de " << km << "km y hay un total de " << disponibles << " asientos disponibles" << endl;
    archivo1<< " " << endl;
    archivo1 << "------------------------------------------------------------------------------------------------" << endl;
    archivo1 << " " << endl;
}

void Servicio1(Grafo<Vuelo> & grafo, int origen, int destino, string aerolinea, int & disponibles, double & km, ofstream & archivo1)
{
    list <Vuelo> vuelos = grafo.costo_arco(origen, destino);
    list<Vuelo>::iterator it = vuelos.begin();
    bool encontrado = false;
    Vuelo vuelo;
    while (it != vuelos.end() && encontrado == false)
    {
        vuelo = *it;
        if (vuelo.devolver_aerolinea() == aerolinea)
        {
            disponibles = vuelo.devolver_disponibles();
            km = vuelo.devolver_distancia();
            encontrado = true;
        }
        it++;
    }
    if (encontrado == true && disponibles > 0)
    {
        Mostrar_Servicio1(disponibles, km, archivo1);
    } else {
        cout << "No se ha encontrado un vuelo disponible" << endl;
    }
}

bool VerificarVuelo(Grafo<Vuelo> & grafo, int origen, int destino, string aerolinea)
{
    list<Vuelo> vuelos = grafo.costo_arco(origen, destino);
    Vuelo vuelo;
    for(list<Vuelo>::iterator it = vuelos.begin(); it != vuelos.end(); it++){
        vuelo = *it;
        if(vuelo.devolver_aerolinea() == aerolinea && vuelo.devolver_disponibles() > 0){
            return true;
        }
    }
    return false;
}

bool AeropuertoRepetido(list<int> camino, int origen)
{
    int i;
    for(list<int>::iterator it = camino.begin(); it != camino.end(); it++){
        i = *it;
        if(origen == i){
            return true;
        }
    }
    return false;
}

bool CaminoRepetido(list<list<int>> caminos, list<int> nuevocamino)
{
    bool repetido = false;
    list<int> camino;
    for(list<list<int>>::iterator it = caminos.begin(); it != caminos.end(); it++){
        camino = *it;
        if(camino == nuevocamino){
            repetido = true;
        }
    }
    return repetido;
}

void DFS(Grafo<Vuelo> & grafo, bool* visitado, int origen, int destino, list< list<int> > & caminos, list<int> & camino, string aerolinea)
{
    visitado[origen] = true;
    camino.push_back(origen);
    if (origen == destino){
        if(!CaminoRepetido(caminos, camino)){
            caminos.push_back(camino);
        }
    } else {
        list<int> adyacentes;
        grafo.devolver_adyacentes(origen, adyacentes);
        for(list<int>::iterator it = adyacentes.begin(); it != adyacentes.end(); it++)
        {
            int v = *it;
            if (!visitado[v] && VerificarVuelo(grafo, origen, v, aerolinea) && !AeropuertoRepetido(camino, v))
            {
                DFS (grafo, visitado, v, destino, caminos, camino, aerolinea);
            }
        }
    }
    camino.pop_back();
    visitado[origen] = false;
}

void recuperar_camino(Grafo<Vuelo> & grafo, list<Vuelo> & vuelos, list<int> camino, string aerolinea)
{
    int origen, destino;
    bool encontrado = false;
    list<Vuelo> candidatos;
    Vuelo candidato;
    list<int>::iterator it = camino.begin();
    while (it != prev(camino.end()))
    {
        origen = *it;
        destino = *next(it);
        candidatos = grafo.costo_arco(origen, destino);
        list<Vuelo>::iterator it2 = candidatos.begin();
        while(it2 != candidatos.end() && encontrado == false)
        {
            candidato = *it2;
            if(candidato.devolver_aerolinea() == aerolinea)
            {
                vuelos.push_back(candidato);
                encontrado = true;
            }
            it2++;
        }
        it++;
        encontrado = false;
    }
}

void Mostrar_Servicio2(list<Vuelo> camino, vector<Aeropuerto> & arreglo, int i, string aerolinea, ofstream & archivo2)
{
    Vuelo vuelo;
    int origen, destino;
    string nombreAeropuerto;
    int escalas = 0;
    double km = 0;
    Aeropuerto aeropuertoOrigen, aeropuertoDestino;
    cout << "Camino: " << i << endl;
    cout << " " << endl;
    archivo2 << "Camino: " << i << endl;
    archivo2 << " " << endl;
    for (list<Vuelo>::iterator it = camino.begin(); it != camino.end(); it++)
    {
        vuelo = *it;
        origen = vuelo.devolver_origen();
        aeropuertoOrigen = arreglo[origen];
        nombreAeropuerto = aeropuertoOrigen.devolver_nombre();
        escalas++;
        km = km + vuelo.devolver_distancia();
        cout << "Aeropuerto: " << origen << " - " << nombreAeropuerto << endl;
        archivo2 << "Aeropuerto: " << origen << " - " << nombreAeropuerto << endl;
    }

    //Agregar el destino de llegada
    destino = vuelo.devolver_destino();
    aeropuertoDestino = arreglo[destino];
    nombreAeropuerto = aeropuertoDestino.devolver_nombre();
    cout << "Aeropuerto: " << destino << " - " << nombreAeropuerto << endl;
    archivo2 << "Aeropuerto: " << destino << " - " << nombreAeropuerto << endl;

    cout << "Se recorrieron: " << km << "km, y se hicieron: " << escalas << " escalas" << endl;
    archivo2 << "Se recorrieron: " << km << "km, y se hicieron: " << escalas << " escalas" << endl;
    archivo2 << " " << endl;
}

void Servicio2(Grafo<Vuelo> & grafo, vector<Aeropuerto> & arreglo, int origen, int destino, ofstream & archivo2)
{
    int N = grafo.devolver_longitud();
    int clave;
    int i,j = 0;
    bool algunCamino = false;
    string aerolinea;
    Aeropuerto aeropuerto = arreglo[origen];
    list<string> aerolineas = aeropuerto.devolver_aerolineas();
    list< list<int>> caminos;
    list<int> camino;
    list<Vuelo> vuelos;
    bool visitado[N];
    for (list<string>::iterator it = aerolineas.begin(); it != aerolineas.end(); it++)
    {
        for (i = 0 ; i < N ; i++)
        {
            visitado[i] = false;
        }
        caminos.clear();
        camino.clear();
        i = 0;
        aerolinea = *it;
        DFS(grafo, visitado, origen, destino, caminos, camino, aerolinea);
        if (!caminos.empty()){
            algunCamino = true;
            cout << " " << endl;
            cout << "Aerolinea: " << aerolinea << endl;
            cout << " " << endl;
            archivo2 << " " << endl;
            archivo2 << "Aerolinea: " << aerolinea << endl;
            archivo2 << " " << endl;
            for(list<list<int>>::iterator it2 = caminos.begin(); it2 != caminos.end(); it2++)
            {
                camino = *it2;
                vuelos.clear();
                recuperar_camino(grafo, vuelos, camino, aerolinea);
                j++;
                Mostrar_Servicio2(vuelos, arreglo, j, aerolinea, archivo2);
                cout << " " << endl;
                archivo2 << " " << endl;
            }
            Borde();
        }
    }
    archivo2 << " " << endl;
    archivo2 << "------------------------------------------------------------------------------------------------" << endl;
    archivo2 << " " << endl;
    if(!algunCamino){
        cout << "No se han encontrado vuelos (directos o con escalas) disponibles" << endl;
    }
}

double recuperar_distancia(Grafo<Vuelo> & grafo, int origen, int destino)
{
    list<Vuelo> vuelos = grafo.costo_arco(origen, destino);
    list<Vuelo>::iterator it = vuelos.begin();
    Vuelo vuelo = *it;
    double distancia = vuelo.devolver_distancia();
    return distancia;
}

bool TodoVisitado(bool* visitado, int N)
{
    for(int i = 0; i < N; i++){
        if(!visitado[i]){
            return false;
        }
    }
    return true;
}

void BACK(Grafo<Vuelo> & grafo, bool* visitado, int actual, int origen, double & Dmenor, double Dactual, list<int> & Mejorcircuito, list<int> & circuitoActual)
{
    if (actual == origen && Dactual != 0 && TodoVisitado(visitado, grafo.devolver_longitud())){
        if(Dactual < Dmenor){
            Dmenor = Dactual;
            Mejorcircuito = circuitoActual;
        }
    } else {
        list<int> L;
        grafo.devolver_adyacentes(actual, L);
        list<int>::iterator it = L.begin();
        while(it != L.end()){
            int i = *it;
            double distancia = recuperar_distancia(grafo, actual, i);
            int Dactual2 = Dactual + distancia;
            if(!visitado[i] && Dactual2 < Dmenor){
                visitado[i] = true;
                circuitoActual.push_back(i);
                BACK(grafo, visitado, i, origen, Dmenor, Dactual2, Mejorcircuito, circuitoActual);
                visitado[i] = false;
                circuitoActual.pop_back();
            }
            it++;
        }
    }
}

void Mostrar_Servicio3(list<int> & circuito, double distancia, vector<Aeropuerto> arreglo, ofstream & archivo3)
{
    int vectice;
    int i = 1;
    Aeropuerto aeropuerto;
    cout << "Nro de Aeropuerto - Nombre del Aeropuerto " << endl;
    cout << " " << endl;
    archivo3 << "Nro de Aeropuerto - Nombre del Aeropuerto " << endl;
    archivo3 << " " << endl;
    for(list<int>::iterator it = circuito.begin(); it != circuito.end(); it++){
        vectice = *it;
        aeropuerto = arreglo[vectice];
        cout << "|" << i << "| - " << aeropuerto.devolver_nombre()<< endl;
        archivo3 << "|" << i << "| - " << aeropuerto.devolver_nombre()<< endl;
        i++;
    }
    cout << " " << endl;
    cout << "Nro de Aeropuertos Visitado: " << i - 1 << endl;
    cout << "Distancia(en km): " << distancia << endl;
    archivo3 << " " << endl;
    archivo3 << "Nro de Aeropuertos Visitado: " << i - 1 << endl;
    archivo3 << "Distancia(en km): " << distancia << endl;
    archivo3 << " " << endl;
    archivo3 << "------------------------------------------------------------------------------------------------" << endl;
    archivo3 << " " << endl;
}


void Servicio3(Grafo<Vuelo> & grafo, int origen, vector<Aeropuerto> arreglo, ofstream & archivo3)
{
    int actual = origen;
    double Dactual = 0;
    double Dmenor = 10000000; //VALOR DISCERNIBLE
    int N = grafo.devolver_longitud();
    bool visitado[N];
    list<int> Mejorcircuito, circuitoActual;
    for(int i = 0; i < N; i++){
        visitado[i] = false;
    }
    circuitoActual.push_back(origen);
    BACK(grafo, visitado, actual, origen, Dmenor, Dactual, Mejorcircuito, circuitoActual);
    if(!Mejorcircuito.empty()){
        Mostrar_Servicio3(Mejorcircuito, Dmenor, arreglo, archivo3);
    } else {
        cout << "No se encontro el recorrido solicitado" << endl;
    }
}

void pedir_opciones(int & opciones)
{
    Borde();
    cout << "Ingrese la opcion de desea: " << endl;
    cout << "1.Lista de aeropuertos " << endl;
    cout << "2.Lista de reservas" << endl;
    cout << "3.Servicio 1: Verificar vuelo directo" << endl;
    cout << "4.Servicio 2: Obtener vuelos de la misma aerolinea" << endl;
    cout << "5.Servicio 3: Circuito de aeropuertos" << endl;
    cout << "6.Salir" << endl;
    cout << " " << endl;
    cout << "Su eleccion: " << ends;
    cin >> opciones;
    Borde();
}

void pedir_od(int & origen, int & destino)
{
    cout << "ingrese numero de lista del aeropuerto origen: " << ends;
    cin >> origen;
    cout << " " << endl;
    cout << "ingrese numero de lista del aeropuerto destino: " << ends;
    cin >> destino;
    cout << " " << endl;
}

void pedir_aerolinea(vector<Aeropuerto> arreglo, int origen, string & aerolinea)
{
    Aeropuerto aeropuerto = arreglo[origen];
    list<string> aerolineas = aeropuerto.devolver_aerolineas();
    int i = 0;
    for (list<string>::iterator it = aerolineas.begin(); it != aerolineas.end(); it++)
    {
        cout << "|" << i << "| " << *it << endl;
        i++;
    }
    cout << " " << endl;
    int opcion;
    cout << "Ingrese la aerolinea deseada: " << ends;
    cin >> opcion;
    list<string>::iterator it2 = aerolineas.begin();
    for (int j = 1; j <= opcion; j++)
    {
        it2++;
    }
    aerolinea = *it2;
}

void ListarAeropuertos(vector<Aeropuerto> arreglo, ofstream & archivo_aeropuertos)
{
    int N = arreglo.size();
    int j;
    Aeropuerto aeropuerto;
    list<string> aerolineas;
    string aerolinea;
    cout << "Numero de Aeropuerto - Nombre del Aeropuerto" << endl;
    cout << " " << endl;
    archivo_aeropuertos << "Numero de Aeropuerto - Nombre del Aeropuerto" << endl;
    archivo_aeropuertos << " " << endl;
    for(int i = 0; i < N; i++){
        aeropuerto = arreglo[i];
        aerolineas = aeropuerto.devolver_aerolineas();
        cout << "|" << i << "|- " << aeropuerto.devolver_nombre() << " Pais: " << aeropuerto.devolver_pais() <<  " Ciudad: " << aeropuerto.devolver_ciudad() << endl;
        cout << "Aerolineas: " << endl;
        archivo_aeropuertos << "|" << i << "|- " << aeropuerto.devolver_nombre() << " Pais: " << aeropuerto.devolver_pais() <<  " Ciudad: " << aeropuerto.devolver_ciudad() << endl;
        archivo_aeropuertos << "Aerolineas: " << endl;
        j = 1;
        for(list<string>::iterator it = aerolineas.begin(); it != aerolineas.end(); it++){
            aerolinea = *it;
            cout << "|" << j << "|- " << aerolinea << endl;
            archivo_aeropuertos << "|" << j << "|- " << aerolinea << endl;
            j++;
        }
        cout << " " << endl;
        archivo_aeropuertos << " " << endl;
    }
    archivo_aeropuertos << " " << endl;
    archivo_aeropuertos << "------------------------------------------------------------------------------------------------" << endl;
    archivo_aeropuertos << " " << endl;
}

void ListarReservas(list<Vuelo> reservas, vector<Aeropuerto> arreglo, ofstream & archivo_reservas)
{
    Vuelo vuelo;
    Aeropuerto aeropuertoO, aeropuertoD;
    int origen, destino;
    cout << "Origen - Destino - Aerolinea" << endl;
    cout << " " << endl;
    archivo_reservas << "Origen - Destino - Aerolinea" << endl;
    archivo_reservas << " " << endl;
    for(list<Vuelo>::iterator it = reservas.begin(); it != reservas.end(); it++){
        vuelo = *it;
        origen = vuelo.devolver_origen();
        destino = vuelo.devolver_destino();
        aeropuertoO = arreglo[origen];
        aeropuertoD = arreglo[destino];
        cout << aeropuertoO.devolver_nombre() << " - " << aeropuertoD.devolver_nombre() << " - " << vuelo.devolver_aerolinea() << endl;
        cout << " " << endl;
        cout << "Asientos Totales: " << vuelo.devolver_asientos() << endl;
        cout << "Asientos Reservados: " << vuelo.devolver_reservas() << endl;
        cout << "Asientos Disponibles: " << vuelo.devolver_disponibles() << endl;
        cout << " " << endl;
        archivo_reservas << aeropuertoO.devolver_nombre() << " - " << aeropuertoD.devolver_nombre() << " - " << vuelo.devolver_aerolinea() << endl;
        archivo_reservas << " " << endl;
        archivo_reservas << "Asientos Totales: " << vuelo.devolver_asientos() << endl;
        archivo_reservas << "Asientos Reservados: " << vuelo.devolver_reservas() << endl;
        archivo_reservas << "Asientos Disponibles: " << vuelo.devolver_disponibles() << endl;
        archivo_reservas << " " << endl;
    }
    archivo_reservas << " " << endl;
    archivo_reservas << "------------------------------------------------------------------------------------------------" << endl;
    archivo_reservas << " " << endl;
}

void Menu(Grafo<Vuelo> & grafo, vector<Aeropuerto> arreglo, list<Vuelo> reservas, ofstream & archivo_aeropuertos, ofstream & archivo_reservas, ofstream & archivo1, ofstream & archivo2, ofstream & archivo3)
{
    unsigned int aux = 1;
    int opciones;
    while (aux == 1){
        pedir_opciones(opciones);
        while(opciones < 1 || opciones > 7){
            pedir_opciones(opciones);
        }
        if (opciones == 1){
            ListarAeropuertos(arreglo, archivo_aeropuertos);
        }
        if (opciones == 2){
            ListarReservas(reservas, arreglo, archivo_reservas);
        }
        if (opciones == 3){
            int origen, destino, disponibles;
            double km;
            string aerolinea;
            pedir_od(origen, destino);
            Borde();
            if(grafo.existe_arco(origen, destino)){
                pedir_aerolinea(arreglo, origen, aerolinea);
                Borde();
                Servicio1(grafo, origen, destino, aerolinea, disponibles, km, archivo1);
            } else {
                cout << "No existen vuelos directos " << endl;
            }
            Borde();
        }
        if (opciones == 4){
            int origen, destino;
            pedir_od(origen, destino);
            Borde();
            Servicio2(grafo, arreglo, origen, destino, archivo2);
            Borde();
        }
        if (opciones == 5){
            int origen;
            cout << "Ingrese el origen: " << ends;
            cin >> origen;
            Servicio3(grafo, origen, arreglo, archivo3);
            Borde();
        }
        if(opciones != 6){
            cout << "¿Desea volver al menu? (0 - No(salir) / 1 - Si) " << endl;
            cout << " " << endl;
            cout << "Su eleccion: " << ends;
            cin >> aux;
            system("cls");
        } else
            aux = 0;
    }
}

int main()
{
    Grafo<Vuelo> grafo;
    vector<Aeropuerto> arreglo;
    list<Vuelo> reservas;
    cargar_aeropuertos("datasets/Aeropuertos.txt", grafo, arreglo);
    cargar_rutas("datasets/Rutas.txt", grafo, arreglo);
    cargar_reservas("datasets/Reservas.txt", grafo, arreglo, reservas);
    string nombreARCHar = "mostrarAeropuertos.txt";
    string nombreARCHre = "mostrarReservas.txt";
    string nombreARCH1 = "mostrarServicio1.txt";
    string nombreARCH2 = "mostrarServicio2.txt";
    string nombreARCH3 = "mostrarServicio3.txt";
    ofstream archivo_aeropuertos(nombreARCHar, ios::out);
    ofstream archivo_reservas(nombreARCHre, ios::out);
    ofstream archivo1(nombreARCH1, ios::out);
    ofstream archivo2(nombreARCH2, ios::out);
    ofstream archivo3(nombreARCH3, ios::out);
    Menu(grafo, arreglo, reservas, archivo_aeropuertos, archivo_reservas, archivo1, archivo2, archivo3);
    archivo_aeropuertos.close();
    archivo_reservas.close();
    archivo1.close();
    archivo2.close();
    archivo3.close();
    return 0;
}
