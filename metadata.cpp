#include "metadata.h"
#include <string.h>

MetaData::MetaData(char *nombre, char type, int primaryKey)
{
    this->primaryKey=primaryKey;
    strcpy(this->nombre,nombre);
    this->type=type;
    this->free=false;

}
