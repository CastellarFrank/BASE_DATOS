#include "table.h"

Table::Table(int num, MetaDataTable meta, Table_Fields cam)
{
    this->num_table=num;
    this->metaData=meta;
    this->listFields=cam;
    this->registerCant=1024/this->listFields.RegisterSize;
}

void Table::setBitsMap(BitsMap &bits){
    this->bitsmap=bits;
}

int Table::addRegister(QByteArray info){
    int tempV=this->metaData.nextDataFree/this->registerCant;
    if(tempV!=0)
        this->validateAndCreateDataSpace(tempV);


}

void Table::setFile(QFile *file){
    this->fileOpened=file;
}
int Table::validateAndCreateDataSpace(int val){
    if(val==1 && this->metaData.pointersData.direct2==-1){

    }
}

void Table::setHeader(Header &head){
    this->header=head;
}

