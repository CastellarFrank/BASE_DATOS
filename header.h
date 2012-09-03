#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <string.h>

class Header
{
public:
    Header(); //Constructor simplemente rellena los datos con un espacio o 0
    char name[208]; //Nombre de la base de datos
    char version[104]; //Versión de la base de datos
    char authors[208]; //Autores de la base de datos
    char date[12]; //Fecha de creación de la base de datos
    char modify[12]; //Fecha de modificación de la base de datos
    int tamanio; //Tamanio de la base de datos en Megas
    int start_metaData; //Byte donde inicia la metaData
    int size_metaData;
    int all_Header_size;
    int countBlocksBitsMap;
    int sizeBitsMap;
    int rellenoMetadata;
    void setName(char *n);
    void setVersion(char *v);
    void setAuthors(char *a);
    void setDate(char *d);
    void setModify(char *m);
private:
    char padding[452];
};
#endif // HEADER_H
