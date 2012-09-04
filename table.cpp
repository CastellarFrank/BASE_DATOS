#include "table.h"

Table::Table(int num, MetaDataTable meta, Table_Fields cam)
{
    this->num_table=num;
    this->metaData=meta;
    this->campos=cam;
    this->registerCant=1024/campos.RegisterSize;
}

void Table::setBitsMap(BitsMap &bits){
    this->bitsmap=bits;
}

