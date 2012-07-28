#ifndef HEADER_H
#define HEADER_H
#include <stdio.h>
#include <string.h>

class Header
{
public:
    Header();
    int tamanio;
    char version[20];
    char authors[25];
    char date[8];
    char modify_date[8];
    int start_metaData;
    int size_metaData;

};

#endif // HEADER_H
