#include "metadatatable.h"

MetaDataTable::MetaDataTable(char *nombre)
{
    this->primaryKey=-1;
    strcpy(this->nombre,nombre);
    this->free=false;
}

void MetaDataTable::setDescrip(char *d){
    strcpy(this->descrip,d);
}

void MetaDataTable::setFecha(char *d){
    strcpy(this->fecha,d);
}

void MetaDataTable::setName(char *n){
    strcpy(this->nombre,n);
}
