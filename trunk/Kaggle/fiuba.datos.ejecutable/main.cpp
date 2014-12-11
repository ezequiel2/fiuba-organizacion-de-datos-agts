#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "../fiuba.datos.ppmc/PPMC.h"
#include "../fiuba.datos.archivos/Archivo.h"
#include "../fiuba.datos.archivos/SerializadorXml.h"
#include "../fiuba.datos.predictor/Frase.h"
#include "../fiuba.datos.predictor/Predictor.h"
#include <time.h>

using namespace std;

// Get current date/time, format is YYYY-MM-DD.HH:mm:ss
const std::string currentDateTime() {
	time_t now = time(0);
	struct tm tstruct;
	char buf[80];
	tstruct = *localtime(&now);
	// Visit http://en.cppreference.com/w/cpp/chrono/c/strftime
	// for more information about date/time format
	strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

	return buf;
}

void entrenar(Archivo* unArchivo) {
	std::cout << "currentDateTime()=" << currentDateTime() << std::endl;

	cout << "Cargando Buffer..." << endl;
	unArchivo->cargarBuffer();
	cout << "Buffer cargado." << endl;
	cout << endl;

	cout << "Parseando Buffer..." << endl;
	vector<string>* palabrasLimpias = unArchivo->parsearBuffer(' ');
	cout << "Buffer PARSEADO" << endl;
	cout << endl;

	cout << "Entrenando..." << endl;
	PPMC* unPPMC = new PPMC(4);
	unPPMC->entrenarPalabras(palabrasLimpias);

	cout << "FIN Entrenado..." << endl;

	std::cout << "currentDateTime()=" << currentDateTime() << std::endl;

	cout << "Inicio Serializador" << endl;
	SerializadorXml* unSer = new SerializadorXml();
	unSer->Serializar(unPPMC, "/home/ezequiel/workspace/Kaggle/fiuba.datos.modelos/");
	cout << "Fin Serializar" << endl;

	std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
}

void predecir() {

	SerializadorXml* unSer = new SerializadorXml();
	Predictor* unPred = new Predictor();

	Archivo* archivo = new Archivo();
	vector<Frase*>* frases = archivo->parsearTest(
			"/home/ezequiel/Documentos/Archivos/test_v2.txt", '\n');

	Modelo0 *unModelo0 = unSer->DeserializarModelo0(
			"/home/ezequiel/workspace/Kaggle/fiuba.datos.modelos/");
	Modelo1 *unModelo1 = unSer->DeserializarModelo1(
			"/home/ezequiel/workspace/Kaggle/fiuba.datos.modelos/");
	unPred->completarFrases(frases, unModelo1, unModelo0);

	unSer->SerializarTestFile(frases,
			"/home/ezequiel/Documentos/ArchivoCompletado/test_v2.txt");

	delete unPred;
	delete archivo;

	unsigned tamanioFrases = frases->size();
	for (unsigned i = 0; i < tamanioFrases; i++) {
		delete (*frases)[i];
	}
	delete frases;
}

/*             PROGRAMA PRINCIPAL                        */

int main() {
	//Cambiar los path por los que tengan en sus compus.
	//Hay uno en PREDICTOR en el metodo completarFrecuencias y despues todos aca en el main.
	unsigned opcionElegida;
	bool seguirEntrenando = true;
	bool salir = false;
	while (!salir) {
		cout << "Bienvinido a KaggleAGTS" << endl;
		cout << "Seleccione la opcion deseada:" << endl;
		cout << "1-Entrenar" << endl;
		cout << "2-Predecir" << endl;
		cout << "3-Salir" << endl;

		cin >> opcionElegida;

		switch (opcionElegida) {
		case 1: {
			if(!seguirEntrenando){
				cout << "Ya no puede entrenar mas." << endl;
				break;
			}
			unsigned cantidadAEntrenar;
			int inicioLectura;
			char seguir;
			cout << "Ingresar en bytes cuanta cantidad desea entrenar:" << endl;
			cin >> cantidadAEntrenar;
			cout << "Creando archivo" << endl;
			Archivo* unArchivo = new Archivo(
					"/home/ezequiel/Documentos/Archivos/train_v2.txt",
					cantidadAEntrenar);
			cout << "Archivo creado" << endl;
			cout << endl;
			while (seguirEntrenando) {
				cout << "Desde donde desea cargar(bytes):" << endl;
				cin >> inicioLectura;
				unArchivo->setInicioProximaLectura(inicioLectura);

				entrenar(unArchivo);
				cout << "¿Desea seguir entrenando?: s/n" << endl;
				cin >> seguir;
				seguirEntrenando = (seguir == 's');
			}
			delete unArchivo;
			break;
		}
		case 2: {
			cout << "Prediccion en curso..." << endl;
			cout<< "Aguarde un momento por favor.." << endl;

			std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
			cout << endl;

			predecir();

			cout << "Fin prediccion! :)" << endl;

			std::cout << "currentDateTime()=" << currentDateTime() << std::endl;
			cout << endl;
			salir = true;
			break;
			}
		case 3:{
			salir = true;
		}
		}
	}
		return 0;
}
