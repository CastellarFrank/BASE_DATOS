#include "table_control.h"

Table_Control::Table_Control()
{
}
void Table_Control::clearAll(){
    this->campos.clear();
    this->metaData.clear();
}
int Table_Control::crearTable(QString name, QString descrip, QString fecha, Table_Fields Field, int key, int second,int blockEmpty){
    int pos=-1;
    for(int i=0;i<this->metaData.size();i++){
        if(this->metaData.at(i).free)
            pos=i;
    }
    if(pos==-1)
        return 1;

    if(blockEmpty==-1)
        return 2;
    this->metaData.value(pos).setName(const_cast<char*>(name.toStdString().c_str()));
    this->metaData.value(pos).setDescrip(const_cast<char*>(descrip.toStdString().c_str()));
    this->metaData.value(pos).setFecha(const_cast<char*>(fecha.toStdString().c_str()));
    this->metaData.value(pos).free=false;
    this->metaData.value(pos).pointerToFields=blockEmpty;
    this->metaData.value(pos).cant_camp=Field.campos.count();
    this->metaData.value(pos).primaryKey=key;
    this->metaData.value(pos).secondaryIndex=second;
    this->campos.insert(pos,Field);
    return 0;
}
