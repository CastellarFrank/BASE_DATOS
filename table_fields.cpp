#include "table_fields.h"

Table_Fields::Table_Fields()
{

}
void Table_Fields::addCampo(char *name, char t, int size, bool n){
    Field temp(name,t,size,(n?'t':'f'));
    this->campos.push_back(temp);
}
