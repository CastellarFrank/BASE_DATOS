#include "field.h"

Field::Field()
{
}

void Field::setName(char *nombre){
    strcpy(this->name,nombre);
}
