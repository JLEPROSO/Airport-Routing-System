#include <string>
#include "Vuelo.h"

using namespace std;

Vuelo::Vuelo()
{
    //CONSTRUCTOR sin argumentos.
}

Vuelo::Vuelo(int origen, int destino, int asientos, string aerolinea, bool cabotaje, double distancia)
{
    Origen = origen;
    Destino = destino;
    Asientos = asientos;
    Reservados = 0;
    Aerolinea = aerolinea;
    Cabotaje = cabotaje;
    Distancia = distancia;
}

Vuelo::~Vuelo()
{
    //destructor
}

int Vuelo::devolver_origen() const
{
    return Origen;
}

int Vuelo::devolver_destino() const
{
    return Destino;
}

int Vuelo::devolver_asientos() const
{
    return Asientos;
}

int Vuelo::devolver_reservas() const
{
    return Reservados;
}

string Vuelo::devolver_aerolinea() const
{
    return Aerolinea;
}

double Vuelo::devolver_distancia() const
{
    return Distancia;
}

int Vuelo::devolver_disponibles() const
{
    if (Asientos > Reservados){
        int disponibles = Asientos - Reservados;
        return disponibles;
    }else
        return 0;
}

void Vuelo::modificar_reserva(int reservados)
{
    Reservados = reservados;
}

bool Vuelo::devolver_cabotaje() const
{
    return Cabotaje;
}

Vuelo& Vuelo::operator=(const Vuelo& otro)
{
    if(this == &otro){
        return *this;
    }

    Origen = otro.devolver_origen();
    Destino = otro.devolver_destino();
    Asientos = otro.devolver_asientos();
    Reservados = otro.devolver_reservas();
    Aerolinea = otro.devolver_aerolinea();
    Cabotaje = otro.devolver_cabotaje();
    Distancia = otro.devolver_distancia();

    return *this;
}
