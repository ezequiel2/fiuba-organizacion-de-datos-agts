/*
 * PPMC.cpp
 *
 *  Created on: 18/10/2014
 *      Author: ezequiel
 */

#include "PPMC.h"

using namespace std;

PPMC::PPMC(int cantidadDeModelos){
	this->cantidadDeModelos = cantidadDeModelos;
	this->modelo0 = new Modelo0(100001);
	this->modelo1 = new Modelo1(150001, 1);
	for(int numeroDeModelo = 2; numeroDeModelo<=(cantidadDeModelos); numeroDeModelo++){
		unsigned long tamanio = this->devolverTamanioDeMapa(numeroDeModelo);
		ModelosSuperiores* modelo = new ModelosSuperiores(tamanio, numeroDeModelo);
		this->modelosSuperiores[numeroDeModelo-2] = modelo;
	}
}

unsigned long PPMC::devolverTamanioDeMapa(int numeroDeModelo){

	unsigned long primo;

	if(numeroDeModelo == 2)
		primo = 180001; //NUMERO DE MERSENE 2^15 - 1
	else if (numeroDeModelo == 3)
		primo = 250001; //NUMERO DE MERSENNE 2^16 - 1
	else if (numeroDeModelo == 4)
		primo = 350001; //NUMERO DE MERSENNE 2^18 - 1

	return primo;
}

void PPMC::entrenar(string rutaArchivo){
	ifstream setDeEntrenamiento;
	setDeEntrenamiento.open(rutaArchivo.c_str(), ios::in );	//NOSE QUE HACE C_STR() PERO SINO TIRA ERROR

	string linea;

	if (setDeEntrenamiento.is_open()){
		while ( getline(setDeEntrenamiento, linea)){
			cout << linea << endl;
		}
	setDeEntrenamiento.close();
	}
	else{
		//ESCRIBIR EXCEPCION POR SI EL ARCHIVO NO ABRE
	}
}

//CORREGIR ESTO VA A RECIBIR UN VECTOR<STRING>, NOSE CUANTAS PALABRAS VAN A VENIR
//TODO DEPENDE DEL PELA!!!!!!
void PPMC::entrenarPalabras(vector<string>* palabrasLimpias){
	int tam = palabrasLimpias->size();
	vector<string>* cincoPalabrasTemporales;
	vector<string>* palabrasTemporales;
	for(int inicio = 0; inicio <= (tam - 5); inicio++){

		cincoPalabrasTemporales = this->devolverPalabras(palabrasLimpias, inicio, inicio+4, 5);
		string ultimaPalabraDeCincoPalabras = (*cincoPalabrasTemporales)[4];
		this->cargarModelosSuperiores(cincoPalabrasTemporales, 3);

		this->cargarModelo1(cincoPalabrasTemporales);
		this->cargarModelo0(cincoPalabrasTemporales);

		//CHEQUEO EL PUNTO PORQUE ME ROMPE LOS CONTEXTOS

		if (ultimaPalabraDeCincoPalabras == "."){
			unsigned inicioAux= inicio+1;
			unsigned tamanio = 4;
			unsigned modelo = 2;

			for(int i=0; i<tamanio;i++){
				//devolverPalabras devuelve una cantidad de palabras pasadas del vector palabrasLimpias
				palabrasTemporales = this->devolverPalabras(palabrasLimpias, inicioAux, inicioAux+tamanio-1, tamanio);

				if (modelo >= 1)
				   this->cargarModelosSuperiores(palabrasTemporales, modelo);
				if (modelo >= 1)
				   this->cargarModelo1(palabrasTemporales);
				this->cargarModelo0(palabrasTemporales);
				inicioAux++;
				tamanio--;
				modelo--;
				delete palabrasTemporales;
			 }
			inicio+=5;

		}
	delete cincoPalabrasTemporales;
	}
}

vector<string>* PPMC::devolverPalabras(vector<string>* palabrasLimpias, int inicio, int fin, int cantPalabras){
	vector<string>* cincoPalabrasTemporales = new vector<string>;
	cincoPalabrasTemporales->resize(cantPalabras);
	int i = inicio;
	while (i <= fin){
		(*cincoPalabrasTemporales)[i-inicio] = (*palabrasLimpias)[i];
		string s = (*palabrasLimpias)[i];
		i++;
	}
	int tam = cincoPalabrasTemporales->size();
	return cincoPalabrasTemporales;

}

void PPMC::cargarModelosSuperiores(vector<string>* palabrasTemporales, int numeroModelo){

	//VOY BORRANDO LOS STRINGS QUE AGREGO A LOS MODELOS

	//int numeroModelo = 3;
	while (numeroModelo > 0){
		int i;
		string nombreContexto = (*palabrasTemporales)[0];
		for(i=1;i<=numeroModelo;i++)
			nombreContexto = nombreContexto + " " + (*palabrasTemporales)[i];

		string nombrePalabra = (*palabrasTemporales)[i];

		this->modelosSuperiores[numeroModelo-1]->agregarContexto(nombreContexto, nombrePalabra);

		palabrasTemporales->pop_back();
		numeroModelo--;
	}
}

void PPMC::cargarModelo1(vector<string>* cincoPalabrasTemporales){
	//EL VECTOR DE CINCO PALABRAS EN ESTE MOMENTO TIENE DOS ELEMENTOS
	//LOS PRIMEROS DOS

	string nombreContexto = (*cincoPalabrasTemporales)[0];
	string nombrePalabra = (*cincoPalabrasTemporales)[1];

	this->modelo1->agregarContexto(nombreContexto, nombrePalabra);

	cincoPalabrasTemporales->pop_back();
}

void PPMC::cargarModelo0(vector<string>* cincoPalabrasTemporales){
	//EL VECTOR DE CINCO PALABRAS EN ESTE MOMENTO TIENE UN SOLO ELEMENTO
	//EL ELEMENTO QUE QUEDA ES EL PRIMERO
	this->modelo0->agregarPalabra((*cincoPalabrasTemporales)[0]);
}

PPMC::~PPMC() {
	delete this->modelo0;
	delete this->modelo1;
	delete[] this->modelosSuperiores;
}

