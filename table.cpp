#include "table.h"

Table::Table(int num,int pointer, int key, int second, int cant)
{
    this->pointerToFields=pointer;
    this->key=key;
    this->secondary=second;
    this->cant_camp=cant;
    this->num_table=num;
}

void Table::setFields(Table_Fields fields){
    this->campos=fields;
}
void Table::setPointers(Table_Pointers pointData, Table_Pointers pointArb, Table_Pointers pointIS, Table_Pointers pointInvert){
    this->pointersData=pointData;
    this->pointerArbolB=pointArb;
    this->pointerIS=pointIS;
    this->pointerInvertedList=pointInvert;
}
void Table::setBitsMap(BitsMap &bits){
    this->bitsmap=bits;
}
