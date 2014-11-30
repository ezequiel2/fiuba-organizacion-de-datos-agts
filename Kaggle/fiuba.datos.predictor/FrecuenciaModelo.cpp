/*
 * FrecuenciaModelo.cpp
 *
 *  Created on: 29/11/2014
 *      Author: hernan
 */

#include "FrecuenciaModelo.h"

FrecuenciaModelo::FrecuenciaModelo(){

}

FrecuenciaModelo::FrecuenciaModelo(unsigned long unaFrecuencia, unsigned unModelo){
	this->frecuencia = unaFrecuencia;
	this->modelo = unModelo;
}
void FrecuenciaModelo::setFrecuencia(unsigned long unaFrecuencia){
	this->frecuencia = unaFrecuencia;
}
void FrecuenciaModelo::setModelo(unsigned unModelo){
		this->modelo = unModelo;
	}
unsigned long FrecuenciaModelo::getFrecuencia(){
	return this->frecuencia;
}
unsigned FrecuenciaModelo::getModelo(){
	return this->modelo;
}

FrecuenciaModelo::~FrecuenciaModelo() {
	// TODO Auto-generated destructor stub
}
