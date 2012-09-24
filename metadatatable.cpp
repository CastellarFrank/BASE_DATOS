#include "metadatatable.h"

MetaDataTable::MetaDataTable()
{
    this->primaryKey=-1;
    this->cant_camp=0;
    this->free='f';
    this->pointerToFields=-1;
    this->nextDataFree=-1;
    this->register_size=0;
    this->register_cant=0;
    this->treeStart=-1;
    memset(this->nombre,0,24);
    memset(this->descrip,0,40);
    memset(this->fecha,0,12);


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
