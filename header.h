#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <string.h>

class Header
{
public:
    Header(); //Constructor simplemente rellena los datos con un espacio o 0
    char name[52]; //Nombre de la base de datos
    char version[32]; //Versi�n de la base de datos
    char authors[40]; //Autores de la base de datos
    char date[12]; //Fecha de creaci�n de la base de datos
    char modify[12]; //Fecha de modificaci�n de la base de datos
    int tamanio; //Tamanio de la base de datos
    int start_metaData; //Bit donde inicia la metaData
    int size_metaData; //Cantidad de Bits que mide la metaData

};
#endif // HEADER_H
