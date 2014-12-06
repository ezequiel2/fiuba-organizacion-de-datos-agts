#ifndef FIUBA_DATOS_PPMC_PPMC_H_
#define FIUBA_DATOS_PPMC_PPMC_H_

#include <iostream>
#include <vector>
#include <list>
#include <fstream>
#include <string>
#include "../fiuba.datos.ppmc/Modelo0.h"
#include "../fiuba.datos.ppmc/Modelo1.h"
#include "../fiuba.datos.ppmc/ModelosSuperiores.h"
#include "../fiuba.datos.ppmc/MapaFrecuencia.h"
#include "../fiuba.datos.ppmc/MapaContexto.h"


class PPMC {
private:
	int cantidadDeModelos;
	Modelo0* modelo0; //ESTOS MODELOS HAY QUE GUARDARLOS EN
	Modelo1* modelo1; //MEMORIA, NO SE PERSISTEN
	ModelosSuperiores* modelosSuperiores[3];

	std::string devolverPalabras(std::vector<std::string>* palabrasLimpias, unsigned long inicio, unsigned long fin);
	void cargarModelosSuperiores(std::string contexto, std::string palabra, int numeroModelo);
	void cargarModelo4(std::string contexto, std::string palabra);
	void cargarModelo3(std::string contexto, std::string palabra);
	void cargarModelo2(std::string contexto, std::string palabra);
	void cargarModelo1(std::string contexto, std::string palabra);
	void cargarModelo0(std::string palabra);
	void agregarContextoSuperiorEn(Contexto* unContexto, int numeroDeModelo);
	unsigned long devolverTamanioDeMapa(int numeroDeModelo);
	const std::string currentDateTime();

public:
	PPMC(int cantidadDeModelos);
	void entrenarPalabras(std::vector<std::string>* palabrasLimpias);
	void completarFrases(std::vector<std::string>* frasesACompletar);

	Modelo0* getModelo0();
	Modelo1* getModelo1();
	ModelosSuperiores *getModelo2();
	ModelosSuperiores *getModelo3();
	ModelosSuperiores *getModelo4();
	~PPMC();
};

#endif
