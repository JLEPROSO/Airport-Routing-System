#ifndef VUELO_H
#define VUELO_H
#include <iostream>
#include <string>

using namespace std;

class Vuelo
{
    public:
        Vuelo();
		Vuelo(int origen, int destino, int asientos, string aerolinea, bool cabotaje, double distancia);
		int devolver_origen() const;
		int devolver_destino() const;
		int devolver_asientos() const;
		int devolver_reservas() const;
		string devolver_aerolinea() const;
		double devolver_distancia() const;
		int devolver_disponibles() const;
		bool devolver_cabotaje() const;
		void modificar_reserva(int reservados);
		Vuelo& operator=(const Vuelo& otro);
        virtual ~Vuelo();

    protected:

    private:
        int Origen;
        int Destino;
		int Asientos;
		int Reservados;
		string Aerolinea;
		bool Cabotaje;
		double Distancia;

}; //Class Vuelo

#endif // VUELO_H

