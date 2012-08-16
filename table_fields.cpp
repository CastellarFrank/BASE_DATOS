#include "table_fields.h"

Table_Fields::Table_Fields()
{

}
void Table_Fields::addCampo(char *name, char t, int size, char n){
    Field temp(name,t,size,n);
    this->campos.push_back(temp);
}
