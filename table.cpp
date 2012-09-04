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
    int pointer;
    if(val==1 && this->metaData.pointersData.direct2==-1){
        this->metaData.pointersData.direct2=this->bitsmap.getBlockEmpty();
        pointer=this->metaData.pointersData.direct2;
    }else if(val==2 && this->metaData.pointersData.direct3==-1){
        this->metaData.pointersData.direct3=this->bitsmap.getBlockEmpty();
        pointer=this->metaData.pointersData.direct3;
    }
    this->fileOpened->seek(this->header.all_Header_size+(pointer*1024));
    for(int i=0;i<this->registerCant;i++){
        QByteArray block;
        QDataStream val(&block, QIODevice::WriteOnly);
        val.setVersion(QDataStream::Qt_4_6);
        QString bytes;
        bytes.fill(0,this->listFields.RegisterSize);
        val<<bytes;
        val<<quint32(i+1+this->metaData.nextDataFree);
        this->fileOpened->write(block);
    }
    return 0;
}

void Table::setHeader(Header &head){
    this->header=head;
}

