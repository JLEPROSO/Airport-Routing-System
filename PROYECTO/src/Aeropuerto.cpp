#include "Aeropuerto.h"
#include <iostream>
#include <string>
#include <list>

using namespace std;

Aeropuerto::Aeropuerto()
{
    //Constructor sin argumentos
}

Aeropuerto::Aeropuerto(int clave, string nombre, string ciudad, string pais)
{
    Clave = clave;
    Nombre = nombre;
    Ciudad = ciudad;
    Pais = pais;
}

Aeropuerto::~Aeropuerto()
{
    //dtor
}

int Aeropuerto::devolver_clave() const
{
    return Clave;
}

string Aeropuerto::devolver_nombre() const
{
    return Nombre;
}

string Aeropuerto::devolver_ciudad() const
{
    return Ciudad;
}

string Aeropuerto::devolver_pais() const
{
    return Pais;
}

list<string> Aeropuerto::devolver_aerolineas() const
{
    return Aerolineas;
}

void Aeropuerto::agregar_aerolinea(string aerolinea)
{
    int cantidad = 0;
    for(list<string>::iterator it = Aerolineas.begin(); it != Aerolineas.end(); it++)
    {
        if (*it == aerolinea)
        {
            cantidad++;
        }
    }
    if(cantidad == 0){
        Aerolineas.push_back(aerolinea);
    }
}

Aeropuerto& Aeropuerto::operator=(const Aeropuerto& otro)
{
    if(this == &otro){
        return *this;
    }

    Clave = otro.devolver_clave();
    Aerolineas = otro.devolver_aerolineas();
    Nombre = otro.devolver_nombre();
    Ciudad = otro.devolver_ciudad();
    Pais = otro.devolver_pais();

    return *this;
}
