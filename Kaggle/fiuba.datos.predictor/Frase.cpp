/*
 * Frase.cpp
 *
 *  Created on: 30/11/2014
 *      Author: ezequiel
 */

#include "Frase.h"
#include <iostream>

using namespace std;

Frase::Frase(vector< string >* unaFrase){
	this->fraseACompletar = unaFrase;
	this->frecuencias = new vector<FrecuenciaModelo* >;
	this->tamanioFrecuencias = this->fraseACompletar->size();
	if (this->tamanioFrecuencias == 0)
		this->tamanioFrecuencias++;
	this->frecuencias->resize(this->tamanioFrecuencias);
	this->inicializarFrecuencias();

	this->indiceDondeCompletar = 0;
}

void Frase::inicializarFrecuencias(){
	for (unsigned i = 0; i < this->tamanioFrecuencias; i++) {
		FrecuenciaModelo *unFrec = new FrecuenciaModelo(NULL, "");
		(*frecuencias)[i] = unFrec;
	}
}

string Frase::devolverContexto(int numeroModelo, int inicio){

	int indice = inicio-1;

	string contexto = (*this->fraseACompletar)[indice];
	indice--;
	for(indice;indice>=(inicio-numeroModelo);indice--){
		contexto = (*this->fraseACompletar)[indice] + " " + contexto;
	}
	return contexto;
}

string Frase::devolverPalabra(unsigned index){
	string palabra = (*this->fraseACompletar)[index];
	return palabra;
}

unsigned Frase::getTamanioFrase(){
	return this->fraseACompletar->size();
}

void Frase::setFrecuencia(float penalizacion, unsigned long frecuencia, unsigned numeroModelo, unsigned index){
	(*this->frecuencias)[numeroModelo + index]->setFrecuencia(penalizacion*frecuencia);
}
/*
void Frase::setModelo(unsigned numeroModelo, unsigned index){
	(*this->frecuencias)[numeroModelo + index]->setModelo(numeroModelo);
}
*/
Completador* Frase::devolverFrecuenciaMinima(unsigned numeroFrase){
	Completador* unComp;
	if (this->tamanioFrecuencias > 1){
		float menorFrecuencia = (*this->frecuencias)[1]->getFrecuencia();
		float frecTemporal;
		unsigned index;
		unsigned posEnDondeCompletarFrase = 1;
		string palabraConMayorFrecuencia = (*this->frecuencias)[1]->getPalabraConMayorFrecuencia();
		for(index = 1; index < this->tamanioFrecuencias; index++){

			frecTemporal = (*this->frecuencias)[index]->getFrecuencia();
			if (menorFrecuencia > frecTemporal){
				menorFrecuencia = frecTemporal;
				posEnDondeCompletarFrase = index;
				if (index == this->tamanioFrecuencias)
					posEnDondeCompletarFrase--;
				palabraConMayorFrecuencia = (*this->frecuencias)[index]->getPalabraConMayorFrecuencia();
			}
		}
	index--;
	unComp = new Completador(posEnDondeCompletarFrase, numeroFrase, palabraConMayorFrecuencia);
	}
	else if (this->tamanioFrecuencias == 1){
		string palabraConMayorFrecuencia = (*this->frecuencias)[0]->getPalabraConMayorFrecuencia();
		unComp = new Completador(1, numeroFrase, palabraConMayorFrecuencia);
	}
	return unComp;
}

void Frase::setPalabraConMayorFrecuencia(ModelosSuperiores* modelosSuperiores,std::string contexto, unsigned index, unsigned numeroModelo){
	string palabraConMayorFrecuencia;
	bool hayQueAgregarPalabra;
	hayQueAgregarPalabra = ((*this->frecuencias)[numeroModelo+index]->getPalabraConMayorFrecuencia() == "");
	if (hayQueAgregarPalabra){
		// busco la palabr con mayor frec para ese modelo
		palabraConMayorFrecuencia = this->buscarPalabraConMayorFrecuencia(modelosSuperiores, contexto);
		(*this->frecuencias)[numeroModelo+index]->setearPalabraConMayorFrecuencia(palabraConMayorFrecuencia);
	}
}

void Frase::setPalabraConMayorFrecuenciaMejora(ModelosSuperiores* modelosSuperiores,string contexto, unsigned index, unsigned numeroModelo){
	string palabraConMayorFrecuencia = this->buscarPalabraConMayorFrecuencia(modelosSuperiores, contexto);
	(*this->frecuencias)[numeroModelo+index]->setearPalabraConMayorFrecuencia(palabraConMayorFrecuencia);
}

void Frase::setPalabraConMayorFrecuencia(Modelo1* modelo1,std::string contexto, unsigned index, unsigned numeroModelo){
	string palabraConMayorFrecuencia;
	bool hayQueAgregarPalabra;
	hayQueAgregarPalabra = ((*this->frecuencias)[numeroModelo+index]->getPalabraConMayorFrecuencia() == "");
	if (hayQueAgregarPalabra){
		palabraConMayorFrecuencia = this->buscarPalabraConMayorFrecuencia(modelo1, contexto);
		(*this->frecuencias)[numeroModelo+index]->setearPalabraConMayorFrecuencia(palabraConMayorFrecuencia);
	}
}

void Frase::setPalabraConMayorFrecuenciaMejora(Modelo1* modelo1, string contexto, unsigned index, unsigned numeroModelo){
	string palabraConMayorFrecuencia = this->buscarPalabraConMayorFrecuencia(modelo1, contexto);
	(*this->frecuencias)[numeroModelo+index]->setearPalabraConMayorFrecuencia(palabraConMayorFrecuencia);

}

void Frase::setPalabraConMayorFrecuencia(string palabraConMayorFrecuencia, unsigned index, unsigned numeroModelo){
		bool hayQueAgregarPalabra;
		hayQueAgregarPalabra = ((*this->frecuencias)[numeroModelo+index]->getPalabraConMayorFrecuencia() == "");
		if (hayQueAgregarPalabra){
			(*this->frecuencias)[numeroModelo+index]->setearPalabraConMayorFrecuencia(palabraConMayorFrecuencia);
		}
}

string Frase::buscarPalabraConMayorFrecuencia(ModelosSuperiores *modelosSuperiores, std::string contexto){
	string palabraMayorFrec = modelosSuperiores->devolverPalabraConMayorFrecuencia(contexto);
	return palabraMayorFrec;
}

string Frase::buscarPalabraConMayorFrecuencia(Modelo1 *modelo1, std::string contexto){
	string palabraMayorFrec = modelo1->devolverPalabraConMayorFrecuencia(contexto);
	return palabraMayorFrec;
}

vector< FrecuenciaModelo* >* Frase::getFrecuencias(){
	return this->frecuencias;
}

vector<string>* Frase::getFraseACompletar(){
	return this->fraseACompletar;
}

void Frase::insertarPalabraEn(unsigned posEnDondeCompletarFrase, std::string palabraMayorFrecuencia){
	//Chequeo caso particular palabra vacia.
	if(this->fraseACompletar->size() == 0)
		posEnDondeCompletarFrase = 0;

	this->fraseACompletar->insert(this->fraseACompletar->begin() + posEnDondeCompletarFrase, palabraMayorFrecuencia);
}

void Frase::borrarFrecuencias(){
	this->tamanioFrecuencias = this->frecuencias->size();
	for(int i=0; i < this->tamanioFrecuencias;i++)
		delete (*this->frecuencias)[i];

	delete this->frecuencias;
}

vector<string>* Frase::getFrase(){
	return this->fraseACompletar;
}

Frase::~Frase() {
	//No se destruye nada porque las frecuencias las borro antes.
}

