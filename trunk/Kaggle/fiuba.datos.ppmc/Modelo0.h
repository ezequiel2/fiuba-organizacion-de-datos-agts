/*
 * Modelo0.h
 *
 *  Created on: 9/11/2014
 *      Author: ezequiel
 */

#ifndef FIUBA_DATOS_PPMC_MODELO0_H_
#define FIUBA_DATOS_PPMC_MODELO0_H_

#include <vector>
#include <string>
#include "../fiuba.datos.hashing/Jenkins.h"
#include "../fiuba.datos.ppmc/MapaFrecuencia.h"


class Modelo0 {
public:
	Modelo0(unsigned long primo);
	MapaFrecuencia* getMapaFrecuencia();
	void setMapaFrecuencia(unsigned long hash, Palabra* palabra);
	void agregarPalabra(std::string unNombre);
	MapaFrecuencia* devolverPalabra(MapaFrecuencia* unaPalabra);
	unsigned long devolverFrecuencia(std::string palabra);
	unsigned long hashearPalabra(std::string palabra);
	float devolverPenalizacion();
	std::string devolverPalabraConMayorFrecuencia();
	Jenkins* getJenkins();
	virtual ~Modelo0();

private:
	Jenkins* unHash;
	MapaFrecuencia* unMapa;
};

#endif /* FIUBA_DATOS_PPMC_MODELO0_H_ */
