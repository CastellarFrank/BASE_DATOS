#include "field.h"

Field::Field()
{
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
