/*
 * PPMC.h
 *
 *  Created on: 18/10/2014
 *      Author: ezequiel
 */
#ifndef FIUBA_DATOS_PPMC_PPMC_H_
#define FIUBA_DATOS_PPMC_PPMC_H_

#include <list>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include "../fiuba.datos.ppmc/Modelo0.h"
#include "../fiuba.datos.ppmc/Modelo1.h"
#include "../fiuba.datos.ppmc/ModelosSuperiores.h"

class PPMC {
private:
	    int cantidadDeModelos;
		Modelo0* modelo0; //ESTOS MODELOS HAY QUE GUARDARLOS EN
		Modelo1* modelo1; //MEMORIA, NO SE PERSISTEN
		std::list<ModelosSuperiores*>* modelosSuperiores;
		std::vector<std::string>* devolverCincoPalabras(std::vector<std::string>* palabrasLimpias, int inicio, int fin);
		void cargarModelosSuperiores(std::vector<std::string>* cincoPalabrasTemporales);
		void cargarModelo1(std::vector<std::string>* cincoPalabrasTemporales);
		void cargarModelo0(std::vector<std::string>* cincoPalabrasTemporales);
		void agregarContextoSuperiorEn(Contexto* unContexto, int numeroDeModelo);
		unsigned long devolverTamanioDeTabla();

public:
	PPMC(int cantidadDeModelos);
	void guardarContextoEnModeloSuperior(int numeroDeModelo, Contexto *unContexto);
	void entrenar(std::string rutaArchivo);
	void entrenarPalabras(std::vector<std::string>* palabrasLimpias);
	~PPMC();
};

#endif
