/*
 * Predictor.cpp
 *
 *  Created on: 30/11/2014
 *      Author: ezequiel
 */

#include "Predictor.h"
#include <iostream>

using namespace std;

Predictor::Predictor() {
	this->completadores = new vector<Completador*>;
}

void Predictor::completarFrases(vector<Frase* >* frasesACompletar, Modelo1* modelo1, Modelo0* modelo0){

	this->completarFrecuencias(frasesACompletar, modelo1, modelo0);

	this->cargarCompletadores(frasesACompletar);

	this->borrarFrecuencias(frasesACompletar);

	//Recorrer los completadorModelo.
	//Para el completadorModelo4 tengo el indice, el numeroDeFrase y la palabraConMayorFrec.
	//Accedo a la frase [numeroDeFrase] y agrego palabraConMayorFrecuencia en [indice]. Asi para todos.
	//Esto es lo que faltaria.
	this->completarPalabrasEnFrases(frasesACompletar);

}

void Predictor::mostrarFrases(vector<Frase*>* frasesACompletar){
	for(unsigned i = 0; i < frasesACompletar->size(); i++){
		vector<string>* frase = (*frasesACompletar)[i]->getFraseACompletar();
		cout << "Frase" << " " << i+1 << endl;
		for(vector<string>::iterator iter = frase->begin(); iter != frase->end(); ++iter){
			cout << (*iter) << " ";
		}
		cout << endl;
	}
}

void Predictor::mostrarFrecuencias(vector<Frase*>* frasesACompletar){
	for(unsigned i = 0; i < frasesACompletar->size(); i++){
		vector< FrecuenciaModelo* >* frecuencias = (*frasesACompletar)[i]->getFrecuencias();
		cout << "Vector Frec:" << endl;
		for(unsigned i=0; i<frecuencias->size();i++){
			cout << (*frecuencias)[i]->getFrecuencia() << '\t' << (*frecuencias)[i]->getPalabraConMayorFrecuencia() << endl;
		}
	}
}

void Predictor::completarFrecuencias(vector<Frase*
		>* frasesACompletar, Modelo1* modelo1, Modelo0* modelo0){
		unsigned numeroModelo = 4;
		unsigned tamanioFrases = frasesACompletar->size();
		for (numeroModelo; numeroModelo >= 2; numeroModelo--) {

			SerializadorXml* unSer = new SerializadorXml();
			ModelosSuperiores* modeloSuperior = unSer->DeserializarModelosSuperiores(numeroModelo, "/home/ezequiel/workspace/Kaggle/fiuba.datos.modelos/");

			for (unsigned i = 0; i < tamanioFrases; i++)
				this->predecirUnaFrase((*frasesACompletar)[i], modeloSuperior);
		//DESCOMENTAR ESTO DESPUES
			delete modeloSuperior;
			delete unSer;
		}

		for (unsigned i = 0; i < tamanioFrases; i++)
			this->predecirUnaFrase((*frasesACompletar)[i], modelo1);

		string palabraConMayorFrecuencia = modelo0->devolverPalabraConMayorFrecuencia();
		for (unsigned i = 0; i < tamanioFrases; i++)
				this->predecirUnaFrase((*frasesACompletar)[i], modelo0, palabraConMayorFrecuencia);

}


void Predictor::predecirUnaFrase(Frase* fraseACompletar, ModelosSuperiores* modelosSuperiores){

	unsigned tam = fraseACompletar->getTamanioFrase();
	unsigned index = 0;
	unsigned numeroModelo = modelosSuperiores->getNumeroModelo();
	if (tam >= numeroModelo + 1){
		for (index; index < tam - numeroModelo; index++) {

			string contexto = this->devolverContexto(fraseACompletar, numeroModelo, index+numeroModelo);
			string palabra = this->devolverPalabra(fraseACompletar, index, numeroModelo);
			pair<unsigned long, bool>* frecExisteContexto;
			frecExisteContexto = modelosSuperiores->devolverFrecuencia(contexto, palabra);
			bool frecuenciaEsCero = (frecExisteContexto->first == 0);
			float penalizacion;

			if(frecExisteContexto->second){
				fraseACompletar->setPalabraConMayorFrecuencia(modelosSuperiores, contexto, index, numeroModelo);

				if ((!frecuenciaEsCero)){
					penalizacion = modelosSuperiores->devolverPenalizacion(numeroModelo);
					fraseACompletar->setFrecuencia(penalizacion, frecExisteContexto->first, numeroModelo, index);
					fraseACompletar->setPalabraConMayorFrecuenciaMejora(modelosSuperiores, contexto, index, numeroModelo);
				}
			}
			delete frecExisteContexto;
		}
	}
}

void Predictor::predecirUnaFrase(Frase* fraseACompletar, Modelo1* modelo1){

	unsigned tam = fraseACompletar->getTamanioFrase();
	unsigned index = 0;
	unsigned numeroModelo = 1;
	if (tam >= numeroModelo + 1){
		for (index; index < tam - numeroModelo; index++) {

			string contexto = this->devolverContexto(fraseACompletar, numeroModelo, index+numeroModelo);
			string palabra = this->devolverPalabra(fraseACompletar, index, numeroModelo);
			pair<unsigned long, bool>* frecExisteContexto;
			frecExisteContexto = modelo1->devolverFrecuencia(contexto, palabra);
			bool frecuenciaEsCero = (frecExisteContexto->second == 0);
			float penalizacion;
			if(frecExisteContexto->second){
				fraseACompletar->setPalabraConMayorFrecuencia(modelo1, contexto, index, numeroModelo);
				if ((!frecuenciaEsCero)){
					penalizacion = modelo1->devolverPenalizacion();
					fraseACompletar->setFrecuencia(penalizacion, frecExisteContexto->first, numeroModelo, index);
					fraseACompletar->setPalabraConMayorFrecuenciaMejora(modelo1, contexto, index, numeroModelo);
				}
			}
		delete frecExisteContexto;
		}
	}
}

void Predictor::predecirUnaFrase(Frase* fraseACompletar, Modelo0* modelo0, string palabraConMayorFrecuencia){

	unsigned tam = fraseACompletar->getTamanioFrase();
	unsigned index = 1;
	unsigned numeroModelo = 0;
	if (tam >= numeroModelo + 1){
		for (index; index < tam - numeroModelo; index++) {

			string palabra = this->devolverPalabra(fraseACompletar, index, numeroModelo);

			unsigned long frecuencia = modelo0->devolverFrecuencia(palabra);
			bool frecuenciaEsCero = (frecuencia == 0);
			float penalizacion;
			if (!frecuenciaEsCero){
				if (tam == 1)
					index = 0;
				penalizacion = modelo0->devolverPenalizacion();
				fraseACompletar->setFrecuencia(penalizacion, frecuencia, numeroModelo, index);
				fraseACompletar->setPalabraConMayorFrecuencia(palabraConMayorFrecuencia, index, numeroModelo);
			}
		}
	}
	//Caso especial: Frase vacia.
	if (tam == 0){
		fraseACompletar->setFrecuencia(0,0, 0, 0);
		fraseACompletar->setPalabraConMayorFrecuencia(palabraConMayorFrecuencia, 0, 0);
	}
}

void Predictor::cargarCompletadores(vector<Frase*>* frasesACompletar){
	unsigned tam = frasesACompletar->size();
	for (unsigned i=0; i < tam; i++){

		unsigned numeroFrase = i + 1;
		Completador *unCompletador = this->hallarLaFrecuenciaMinima((*frasesACompletar)[i], numeroFrase);
		this->setearCompletadorModelo(unCompletador);
	}
}

string Predictor::devolverContexto(Frase* fraseACompletar, unsigned numeroModelo, unsigned inicio){

	return fraseACompletar->devolverContexto(numeroModelo, inicio);
}

string Predictor::devolverPalabra(Frase* fraseACompletar, unsigned numeroModelo, unsigned index){
	return fraseACompletar->devolverPalabra(index+numeroModelo);
}

Completador* Predictor::hallarLaFrecuenciaMinima(Frase *fraseACompletar, unsigned numeroFrase){

	Completador* unComp = fraseACompletar->devolverFrecuenciaMinima(numeroFrase);
	return unComp;
}

void Predictor::setearCompletadorModelo(Completador* unCompletador){

	this->completadores->push_back(unCompletador);
}

void Predictor::completarPalabrasEnFrases(vector<Frase* >* frasesACompletar){

	this->completarPalabrasDeLosCompletadores(frasesACompletar);
}

void Predictor::completarPalabrasDeLosCompletadores(vector<Frase* >* frasesACompletar){

	unsigned numeroFrase, posEnDondeCompletarFrase;
	string palabraMayorFrecuencia;
	// pido una sola ves la palabra con mayor frecuencia del modelo 0

	unsigned tamCompletadores = this->completadores->size();
	for(unsigned indice = 0; indice < tamCompletadores; indice++){
		numeroFrase = (*this->completadores)[indice]->getNumeroFrase();
		posEnDondeCompletarFrase = (*this->completadores)[indice]->getPosEnDondeCompletarFrase();
		palabraMayorFrecuencia = (*this->completadores)[indice]->getPalabraConMayorFrecuencia();
		if (palabraMayorFrecuencia[0] == '"')
			palabraMayorFrecuencia = """";
		(*frasesACompletar)[numeroFrase-1]->insertarPalabraEn(posEnDondeCompletarFrase, palabraMayorFrecuencia);
	}
}

void Predictor::borrarFrecuencias(vector<Frase*>* frasesACompletar){
	unsigned tamanioFrases = frasesACompletar->size();
	for (unsigned i=0; i<tamanioFrases;i++){
		(*frasesACompletar)[i]->borrarFrecuencias();
	}
}

Predictor::~Predictor() {
	unsigned tam = this->completadores->size();
	for(unsigned i=0; i < tam; i++){
		delete (*this->completadores)[i];
	}
	delete this->completadores;
}
