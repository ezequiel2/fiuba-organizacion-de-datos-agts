/*
 * MapaContexto.cpp
 *
 *  Created on: 19/11/2014
 *      Author: hernan
 */

#include "MapaContexto.h"

using namespace std;

MapaContexto::MapaContexto() {
	this->hashContexto = new map<unsigned long, Contexto*>;
}

void MapaContexto::agregarContexto(unsigned long clave, string nombrePalabra, int numeroModelo){
    int primo = this->devolverPrimo(numeroModelo);
    Contexto* unContexto = new Contexto(primo);
    unContexto->agregarPalabra(nombrePalabra);

    pair<unsigned, Contexto*> hashContexto = make_pair(clave, unContexto);

	this->hashContexto->insert(hashContexto);
}

void MapaContexto::agregarContextoExistente(unsigned long clave, string nombrePalabra){
	Contexto* contextoExistente = (*this->hashContexto)[clave];
	contextoExistente->agregarPalabra(nombrePalabra);

}
//CAMBIAR PRIMOS??????????? PELA
int MapaContexto::devolverPrimo(int numeroModelo){
	int primo;
	if(numeroModelo == 1)
		primo = 3432;
	else if (numeroModelo == 2)
		primo= 5345;
	else if (numeroModelo == 3)
		primo= 5345;
	else if (numeroModelo == 4)
		primo= 5345;

	return primo;
}
Contexto MapaContexto::getContextos(unsigned long clave){
	return (*(*this->hashContexto)[clave]);
}

bool MapaContexto:: existeClave(unsigned long clave){
    return (this->hashContexto->find(clave) != this->hashContexto->end());

}

MapaContexto::~MapaContexto() {
	delete this->hashContexto;
}
