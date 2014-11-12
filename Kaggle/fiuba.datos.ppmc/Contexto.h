/*
 * Contexto.h
 *
 *  Created on: 19/10/2014
 *      Author: ezequiel
 */

#ifndef FIUBA_DATOS_PPMC_CONTEXTO_H_
#define FIUBA_DATOS_PPMC_CONTEXTO_H_

#include <string>
#include <list>
#include "Palabra.h"
#include <iostream>

using namespace std;

class Contexto {
private:
	string nombre;
	list<Palabra>* palabras;

public:
	Contexto();
	void agregarPalabra(string unaPalabra);
	string getNombre();
	virtual ~Contexto();
};

#endif /* FIUBA_DATOS_PPMC_CONTEXTO_H_ */