#include <fstream>
#include <string>
#include <list>
#include <vector>
#include "../fiuba.datos.archivos/BufferParser.h"
#include "../fiuba.datos.archivos/Parser.h"


class Archivo {
public:
	Archivo(std::string rutaArchivo, int tamanioBuffer);
	void cargarBuffer();
	//metodo de prueba, se puede borrar
	void mostrarBuffer();
	std::vector<std::string>* parsearBuffer(char valorParseo);
	std::vector<std::string>* parsearTest(std::string rutaArchivo, char valorParseo);
	void cargarInicioProximaLectura();
	virtual ~Archivo();
private:
	int tamanioBuffer;
	std::ifstream handle;
    int inicioProximaLectura;
    Parser* parser;
    BufferParser* buffer;
    void cargar();
};
