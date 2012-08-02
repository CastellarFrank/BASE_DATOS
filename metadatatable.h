#ifndef METADATATABLE_H
#define METADATATABLE_H
#include <stdio.h>
#include <string.h>
class MetaDataTable
{
public:
    MetaDataTable(char * nombre);
    char nombre[25];
    char fecha[12];
    char descrip[40];
    short cant_camp;
    bool free;
    short primaryKey;
    int firstIndex1;
    int firstIndex2;
    int firstIndex3;
    int secondaryIndex1;
    int secondaryIndex2;
    int thirthIndex;

    void setName(char * n);
    void setFecha(char *d);
    void setDescrip(char*d);
private:


};

#endif // METADATATABLE_H
