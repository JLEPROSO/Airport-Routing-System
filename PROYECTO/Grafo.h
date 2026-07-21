#ifndef GRAFO_H_
#define GRAFO_H_
#include <cassert>


#include <list>
#include <set>
#include <map>
#include <queue>
#include <vector>

using namespace std;

template <typename C> class Grafo
{
public:
	class Arco
	{
	public:
		Arco();
		Arco(int adyacente, const C & costo);
		int devolver_adyacente() const;
		void modificar_costo(const C & costo);
		const C & devolver_costo() const;
	private:
		int vDestino;
		C aCosto;
	}; // class Arco

public:
	// NOTA: Dependiendo de la implementación puede ser necesario incluir otras funciones constructuras
	Grafo();
	Grafo(const Grafo & otroGrafo);

	~Grafo();

	Grafo & operator = (const Grafo & otroGrafo);

	// Devuelve true si la cantidad de vértices es cero
	bool esta_vacio() const;

	// Indica la cantidad de vértices del grafo
	int devolver_longitud() const;

	bool existe_vertice(int vertice) const;

	bool existe_arco(int origen, int destino) const;

	// PRE CONDICION: existe_arco(origen, destino)
	list <C> costo_arco(int origen, int destino) const;

	void devolver_vertices(list<int> & vertices) const;

	void devolver_adyacentes(int origen, list<int> & adyacentes) const;

	void agregar_vertice(int vertice);

	// POST CONDICION: Para todo vértice v != vertice: !existeArco(v, vertice) && !existeArco(vertice, v)
	void eliminar_vertice(int vertice);

	// PRE CONDICION: existeArco(origen, destino)
	void modificar_costo_arco(int origen, int destino, const C & costo);

	// PRE CONDICION: existeVertice(origen) && existeVertice(destino)
	// POST CONDICION: existeArco(origen, destino)
	void agregar_arco(int origen, int destino, const C & costo);

	// POST CONDICION: !existeArco(origen, destino)
	void eliminar_arco(int origen, int destino);

private:
	map <int ,list<Arco> > miMapa;

	//Métodos auxiliares

	void vaciar();

	//Miembro de la clase

	int cantidad = 0;

}; // class Grafo


/*
 * Arco
 */

template <typename C> Grafo<C>::Arco::Arco()
{
    //Constructor del arco.
}

template <typename C> Grafo<C>::Arco::Arco(int adyacente, const C & costo)
{
    vDestino = adyacente;
    aCosto = costo;
}

template <typename C> int Grafo<C>::Arco::devolver_adyacente() const
{
    return vDestino;
}

template <typename C> void Grafo<C>::Arco::modificar_costo(const C & costo)
{
    aCosto = costo;
}

template <typename C> const C & Grafo<C>::Arco::devolver_costo() const
{
    return aCosto;
}


/*
 * Grafo
 */

template <typename C> Grafo<C>::Grafo()
{
    //Constructor del Grafo
}

/*
template <typename C> Grafo<C>::Grafo(const Grafo & otroGrafo)
{
    list<int> listaV;
    otroGrafo.devolver_vertices(listaV);
    for(list<int>::iterator it = listaV.begin(); it != listaV.end(); it++){
        list<Arco> listaAdy;
        otroGrafo.devolver_adyacentes(*it, listaAdy);
        agregar_nodo(*it, listaAdy);
    }
}
*/

template <typename C> Grafo<C>::~Grafo()
{
    vaciar();
}

/*
template <typename C> Grafo<C> & Grafo<C>::operator = (const Grafo & otroGrafo)
{
    vaciar();
    list<int> listaV;
    otroGrafo.devolver_vertices(listaV);
    for(list<int>::iterator it = listaV.begin(); it != listaV.end(); it++){
        list<Arco> listaAdy;
        otroGrafo.devolver_adyacentes(*it, listaAdy);
        agregar_nodo(*it, listaAdy);
    }
    return *this;
}
*/

template <typename C> bool Grafo<C>::esta_vacio() const
{
    if (miMapa.empty() == true){
        return true;
    }else
        return false;
}

template <typename C> int Grafo<C>::devolver_longitud() const
{
    return cantidad; //en lugar de "size_t" para mejorar complejidad O(log n) a O(1)//
}

template <typename C> bool Grafo<C>::existe_vertice(int vertice) const
{
    typename map <int ,list<Arco>>::const_iterator it = miMapa.find(vertice);
    if (it != miMapa.end()) {
        return true;
    }else
        return false;
}

template <typename C> bool Grafo<C>::existe_arco(int origen, int destino) const
{
    bool existe = false;
    for (typename map <int ,list<Arco> >::const_iterator it = miMapa.begin(); it != miMapa.end(); it++){
        if (it->first == origen){
            const list<Arco>& listaAdy = it->second;
            for (typename list<Arco>::const_iterator it2 = listaAdy.begin(); it2 != listaAdy.end(); it2++){
                if (it2->devolver_adyacente() == destino){
                    existe = true;
                }
            }
        }
    }
    return existe;
}

template <typename C> list <C> Grafo<C>::costo_arco(int origen, int destino) const
{
    typename map <int ,list<Arco> >::const_iterator it = miMapa.find(origen);
    if (it != miMapa.end()){
        const list<Arco>& listaAdy = it->second;
        typename list<Arco>::const_iterator it2 = listaAdy.begin();
        list<C> costos;
        while (it2 != listaAdy.end()){
            if (it2->devolver_adyacente() == destino){
                costos.push_back(it2->devolver_costo());
            }
            it2++;
        }
        return costos;
    }
}

template <typename C> void Grafo<C>::devolver_vertices(list<int> & vertices) const
{
    for (typename map <int ,list<Arco> >::const_iterator it = miMapa.begin(); it != miMapa.end(); it++){
        vertices.push_back(it->first);
    }
}

template <typename C> void Grafo<C>::devolver_adyacentes(int origen, list <int> & adyacentes) const
{
    typename map <int ,list<Arco> >::const_iterator it = miMapa.find(origen);
    const list<Arco>& listaAdy = it->second;
    int longitud = devolver_longitud();
    int destino;
    vector<bool> guardados;
    guardados.resize(longitud);
    for (int i = 0; i < longitud; i++)
    {
        guardados[i] = false;
    }
    for (typename list<Arco>::const_iterator it2 = listaAdy.begin(); it2 != listaAdy.end(); it2++){
        int destino = it2->devolver_adyacente();
        if (guardados[destino] == false)
        {
            adyacentes.push_back(destino);
            guardados[destino] = true;
        }
    }
}

template <typename C> void Grafo<C>::agregar_vertice(int vertice)
{
    if (existe_vertice(vertice) == false){
        list<Arco> listaAdy;
        miMapa[vertice] = listaAdy;
        cantidad = cantidad + 1;
    }
}

template <typename C> void Grafo<C>::eliminar_vertice(int vertice)
{
    if (existe_vertice(vertice) == true){
        miMapa.erase(vertice);
    }
}

template <typename C> void Grafo<C>::modificar_costo_arco(int origen, int destino, const C & costo)
{
        typename map <int ,list<Arco> >::iterator it = miMapa.find(origen);
        bool encontrado = false;
        if (it != miMapa.end()){
            typename list<Arco>::iterator it2 = it->second.begin();
            while (it2 != it->second.end() && encontrado == false){
                if (it2->devolver_adyacente() == destino){
                    it2->modificar_costo(costo);
                    encontrado = true;
                }else {
                    it2++;
                }
            }
        }
}

template <typename C> void Grafo<C>::agregar_arco(int origen, int destino, const C & costo)
{
    Arco nuevo(destino, costo);
    miMapa[origen].push_back(nuevo);
}

template <typename C> void Grafo<C>::eliminar_arco(int origen, int destino)
{
    typename map <int ,list<Arco> >::iterator it = miMapa.find(origen);
    if (it != miMapa.end()){
        list<Arco>& listaAdy = it->second;
        bool eliminado = false;
        typename list<Arco>::iterator it2 = listaAdy.begin();
        while(it2 != listaAdy.end() && eliminado != true){
            if(it2->devolver_adyacente() == destino){
                eliminado = true;
                listaAdy.erase(it2);
            }
        it2++;
        }
    }
}

template <typename C> void Grafo<C>::vaciar()
{
    miMapa.clear();
}

#endif /* GRAFO_H_ */
