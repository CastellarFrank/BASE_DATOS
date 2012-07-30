#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <string.h>

class Header
{
public:
    Header(); //Constructor simplemente rellena los datos con un espacio o 0
    int tamanio; //Tamanio de la base de datos
    char name[28]; //Nombre de la base de datos
    char version[20]; //Versión de la base de datos
    char authors[28]; //Autores de la base de datos
    char date[8]; //Fecha de creación de la base de datos
    char modify_date[8]; //Fecha de modificación de la base de datos
    int start_metaData; //Bit donde inicia la metaData
    int size_metaData; //Cantidad de Bits que mide la metaData

};

#endif // HEADER_H
