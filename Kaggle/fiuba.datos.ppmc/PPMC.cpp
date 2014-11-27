#include "PPMC.h"

using namespace std;

PPMC::PPMC(int cantidadDeModelos) {
	this->cantidadDeModelos = cantidadDeModelos;
	this->modelo0 = new Modelo0(100001);
	this->modelo1 = new Modelo1(150001, 1);
	for (int numeroDeModelo = 2; numeroDeModelo <= (cantidadDeModelos);
			numeroDeModelo++) {
		unsigned long tamanio = this->devolverTamanioDeMapa(numeroDeModelo);
		ModelosSuperiores* modelo = new ModelosSuperiores(tamanio,
				numeroDeModelo);
		this->modelosSuperiores[numeroDeModelo - 2] = modelo;
	}
}
Modelo0* PPMC::getModelo0() {
	return this->modelo0;
}

Modelo1* PPMC::getModelo1(){
	return this->modelo1;
}
ModelosSuperiores* PPMC::getModelo2(){
		return this->modelosSuperiores[0];
}
ModelosSuperiores* PPMC::getModelo3(){
	return this->modelosSuperiores[1];
}

ModelosSuperiores* PPMC::getModelo4() {
	return this->modelosSuperiores[2];
}

unsigned long PPMC::devolverTamanioDeMapa(int numeroDeModelo) {

	unsigned long primo;

	if (numeroDeModelo == 2)
		primo = 180001; //NUMERO DE MERSENE 2^15 - 1
	else if (numeroDeModelo == 3)
		primo = 250001; //NUMERO DE MERSENNE 2^16 - 1
	else if (numeroDeModelo == 4)
		primo = 350001; //NUMERO DE MERSENNE 2^18 - 1

	return primo;
}

void PPMC::entrenar(string rutaArchivo) {
	ifstream setDeEntrenamiento;
	setDeEntrenamiento.open(rutaArchivo.c_str(), ios::in); //NOSE QUE HACE C_STR() PERO SINO TIRA ERROR

	string linea;

	if (setDeEntrenamiento.is_open()) {
		while (getline(setDeEntrenamiento, linea)) {
			cout << linea << endl;
		}
		setDeEntrenamiento.close();
	} else {
		//ESCRIBIR EXCEPCION POR SI EL ARCHIVO NO ABRE
	}
}

//CORREGIR ESTO VA A RECIBIR UN VECTOR<STRING>, NOSE CUANTAS PALABRAS VAN A VENIR
//TODO DEPENDE DEL PELA!!!!!!
void PPMC::entrenarPalabras(vector<string>* palabrasLimpias) {
	int tam = palabrasLimpias->size();
	vector<string>* cincoPalabrasTemporales;
	vector<string>* palabrasTemporales;
	int inicio;
	for (inicio = 0; inicio <= (tam - 5); inicio++) {

		cincoPalabrasTemporales = this->devolverPalabras(palabrasLimpias,
				inicio, inicio + 4, 5);
		string ultimaPalabraDeCincoPalabras = (*cincoPalabrasTemporales)[4];
		this->cargarModelosSuperiores(cincoPalabrasTemporales, 3);

		this->cargarModelo1(cincoPalabrasTemporales);
		this->cargarModelo0(cincoPalabrasTemporales);

		//CHEQUEO EL PUNTO PORQUE ME ROMPE LOS CONTEXTOS

		if (ultimaPalabraDeCincoPalabras == ".") {
			this->chequeoCasoParticular(palabrasLimpias, inicio, 4, 3, 3);
			inicio += 4;
		}
		delete cincoPalabrasTemporales;
	}
	this->chequeoCasoParticular(palabrasLimpias, inicio - 1, tam - inicio,
			tam - inicio - 1, tam - inicio - 1);
	vector<string>* ultimaPalabrasTemporales;
	ultimaPalabrasTemporales = this->devolverPalabras(palabrasLimpias, tam - 1,
			tam - 1, 1);
	this->cargarModelo0(ultimaPalabrasTemporales);
}

vector<string>* PPMC::devolverPalabras(vector<string>* palabrasLimpias,
		int inicio, int fin, int cantPalabras) {
	vector<string>* cincoPalabrasTemporales = new vector<string>;
	cincoPalabrasTemporales->resize(cantPalabras);
	int i = inicio;
	while (i <= fin) {
		(*cincoPalabrasTemporales)[i - inicio] = (*palabrasLimpias)[i];
		string s = (*palabrasLimpias)[i];
		i++;
	}
	int tam = cincoPalabrasTemporales->size();
	return cincoPalabrasTemporales;

}

void PPMC::cargarModelosSuperiores(vector<string>* palabrasTemporales,
		int numeroModelo) {

	//VOY BORRANDO LOS STRINGS QUE AGREGO A LOS MODELOS

	//int numeroModelo = 3;
	while (numeroModelo > 0) {
		int i;
		string nombreContexto = (*palabrasTemporales)[0];
		for (i = 1; i <= numeroModelo; i++)
			nombreContexto = nombreContexto + " " + (*palabrasTemporales)[i];

		string nombrePalabra = (*palabrasTemporales)[i];

		this->modelosSuperiores[numeroModelo - 1]->agregarContexto(
				nombreContexto, nombrePalabra);

		palabrasTemporales->pop_back();
		numeroModelo--;
	}
}

void PPMC::cargarModelo1(vector<string>* cincoPalabrasTemporales) {
	//EL VECTOR DE CINCO PALABRAS EN ESTE MOMENTO TIENE DOS ELEMENTOS
	//LOS PRIMEROS DOS

	string nombreContexto = (*cincoPalabrasTemporales)[0];
	string nombrePalabra = (*cincoPalabrasTemporales)[1];

	this->modelo1->agregarContexto(nombreContexto, nombrePalabra);

	cincoPalabrasTemporales->pop_back();
}

void PPMC::cargarModelo0(vector<string>* cincoPalabrasTemporales) {
	//EL VECTOR DE CINCO PALABRAS EN ESTE MOMENTO TIENE UN SOLO ELEMENTO
	//EL ELEMENTO QUE QUEDA ES EL PRIMERO
	this->modelo0->agregarPalabra((*cincoPalabrasTemporales)[0]);
}

void PPMC::chequeoCasoParticular(vector<string>* palabrasLimpias, int inicio,
		unsigned tamanio, unsigned modelo, int iteraciones) {
	unsigned inicioAux = inicio + 1;
	std::vector<string>* palabrasTemporales;
	for (int i = 1; i <= iteraciones; i++) {
		//devolverPalabras devuelve una cantidad de palabras pasadas del vector palabrasLimpias
		palabrasTemporales = this->devolverPalabras(palabrasLimpias, inicioAux,
				inicioAux + tamanio - 1, tamanio);

		if (modelo >= 2)
			this->cargarModelosSuperiores(palabrasTemporales, modelo - 1);
		if (modelo >= 1) {
			this->cargarModelo1(palabrasTemporales);
			this->cargarModelo0(palabrasTemporales);
		}
		inicioAux++;
		tamanio--;
		modelo--;
		delete palabrasTemporales;
	}
}

void PPMC::completarFrases(vector<list<string>*>* frasesACompletar) {

	unsigned numeroModelo = 4;
	for (numeroModelo; numeroModelo >= 1; numeroModelo--) {

		this->cargarModelo(numeroModelo);
		for (int i = 0; i < frasesACompletar->size(); i++) {
			this->predecir((*frasesACompletar)[i], numeroModelo);
		}
//	parser.agregarFraseCompleta(id, frase);
	}
}

void PPMC::predecir(list<string>* fraseACompletar, unsigned numeroModelo) {

	vector< pair<float, bool> >* frecuencias = new vector<pair< float, bool > >;
	int tam = fraseACompletar->size();
	frecuencias->resize(tam);

	this->inicializarFrecuencias(frecuencias);
	int index = 0;
	for (index; index < tam - numeroModelo; index++) {

		string contexto = this->devolverContexto(fraseACompletar, numeroModelo, index+numeroModelo);

		list<string>::iterator iterFrase = fraseACompletar->begin();
		advance(iterFrase, (index+numeroModelo));
		string palabra = *iterFrase;

		unsigned frecuencia = this->modelosSuperiores[numeroModelo-2]->devolverFrecuencia(contexto, palabra);
		bool frecuenciaEsCero = (frecuencia == 0);
		bool noBajaDeNivel = ((*frecuencias)[numeroModelo + index].second == false);
		float penalizacion;
		if ((!frecuenciaEsCero) && (noBajaDeNivel)){
			penalizacion = this->devolverPenalizacion(numeroModelo);
			(*frecuencias)[numeroModelo + index].first = (penalizacion*frecuencia);
			(*frecuencias)[numeroModelo + index].second = true;
		}

	}
}

void PPMC::inicializarFrecuencias(vector< pair< float, bool > >* frecuencias){

	for (unsigned i = 0; i < frecuencias->size(); i++) {
		(*frecuencias)[i].first = 0;
		(*frecuencias)[i].second = false;
	}
}

void PPMC::cargarModelo(int numeroModelo){

	//ACA HAY QUE VER COMO CARGAR EL MODELO EN MEMORIA
}

string PPMC::devolverContexto(list<string>* fraseACompletar, int numeroModelo, int inicio){
	int indice = inicio-1;

	list<string>::iterator iterFrase = fraseACompletar->begin();
	advance(iterFrase, indice);
	string contexto = *iterFrase;

	for(indice;indice>(inicio-numeroModelo);--indice){
		iterFrase--;
		contexto = *iterFrase + " " + contexto;
	}

	return contexto;
}

float PPMC::devolverPenalizacion(unsigned numeroModelo){
	float penalizacion;

	if (numeroModelo >= 2)
		penalizacion = this->modelosSuperiores[numeroModelo-2]->devolverPenalizacion(numeroModelo);
	else if (numeroModelo == 1)
		penalizacion = this->modelo1->devolverPenalizacion();
	else if (numeroModelo == 0)
		penalizacion = this->modelo0->devolverPenalizacion();

	return penalizacion;
}

PPMC::~PPMC() {
	delete this->modelo0;
	delete this->modelo1;
	delete[] this->modelosSuperiores;
}