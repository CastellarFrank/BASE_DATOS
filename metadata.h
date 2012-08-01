#ifndef METADATA_H
#define METADATA_H
#include <stdio.h>
class MetaData
{
public:
    MetaData(char * nombre,char type, int primaryKey);//Hola a todos esto solo es para probar
    char nombre[40];
    char type;
    char free;
    int primaryKey;
    int firstIndex;
    int secondaryIndex;
    int thirthIndex;
};

#endif // METADATA_H
