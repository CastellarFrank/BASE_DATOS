#include "field.h"

Field::Field()
{
    memset(this->name,0,24);
    this->type='n';
    this->nulo='f';
    this->size=0;
}

Field::Field(char *name, char t, int size, char n){
    this->setName(name);
    this->type=t;
    this->nulo=n;
    this->size=size;
}

void Field::setName(char *nombre){
    strcpy(this->name,nombre);
}
