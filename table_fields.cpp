#include "table_fields.h"

Table_Fields::Table_Fields()
{

}
int Table_Fields::addCampo(char *name, char t, int size, bool n){
    if(((this->campos.count()+1)*sizeof(Field))>1024)
        return 1;
    Field temp(name,t,size,(n?'t':'f'));
    this->campos.push_back(temp);
    return 0;
}
