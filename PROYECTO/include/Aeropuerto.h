#ifndef AEROPUERTO_H
#define AEROPUERTO_H
#include <iostream>
#include <string>
#include <list>

using namespace std;

class Aeropuerto
{
    public:
        Aeropuerto();
        Aeropuerto(int clave, string nombre, string ciudad, string pais);
        int devolver_clave() const;
        string devolver_nombre() const;
        string devolver_ciudad() const;
        string devolver_pais() const;
        list<string> devolver_aerolineas() const;
        void agregar_aerolinea(string aerolinea);
        Aeropuerto& operator=(const Aeropuerto& otro);
        virtual ~Aeropuerto();

    protected:

    private:
        int Clave;
        list<string> Aerolineas;
        string Nombre;
        string Ciudad;
        string Pais;
};

#endif // AEROPUERTO_H
