#ifndef METADATATABLE_H
#define METADATATABLE_H
#include <stdio.h>
#include <string.h>
#include <table_pointers.h>
class MetaDataTable
{
public:
    MetaDataTable();
    char nombre[24];
    char fecha[12];
    char descrip[40];
    short cant_camp;
    short primaryKey;
    short secondaryIndex;
    char free;
    int pointerToFields;

    Table_Pointers pointersData;
    Table_Pointers pointerArbolB;
    Table_Pointers pointerAVL;
    Table_Pointers pointerInvertedList;

    void setName(char * n);
    void setFecha(char *d);
    void setDescrip(char*d);


};

#endif // METADATATABLE_H
