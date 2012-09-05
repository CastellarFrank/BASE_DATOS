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
    int writeDirection=this->getBytesPositionToWriteRegister(tempV);
    this->fileOpened->seek(writeDirection+this->listFields.RegisterSize);
    int num;
    this->fileOpened->read(reinterpret_cast<char*>(&num),sizeof(int));
    this->metaData.nextDataFree=num;
    this->fileOpened->seek(writeDirection);
    num=-1;
    info.append(reinterpret_cast<char*>(&num),sizeof(int));
    this->fileOpened->write(info);
    return 0;
}

void Table::setFile(QFile *file){
    this->fileOpened=file;
}
int Table::getBytesPositionToWriteRegister(int val){
    bool nuevo=true;
    int pointer;
    int iFile;
    if(val==0){
        nuevo=false;
        pointer=this->metaData.pointersData.direct1;
    }else if(val==1){
        if(this->metaData.pointersData.direct2==-1){
            this->metaData.pointersData.direct2=this->bitsmap.getBlockEmpty();
        }else{
            nuevo=false;
        }
        pointer=this->metaData.pointersData.direct2;
    }else if(val==2){
        if(this->metaData.pointersData.direct3==-1){
            this->metaData.pointersData.direct3=this->bitsmap.getBlockEmpty();
        }else{
            nuevo=false;
        }
        pointer=this->metaData.pointersData.direct3;
    }else{
        iFile=(val-3)/(1024/sizeof(int));
        if(iFile==0){
            if(this->metaData.pointersData.no_directN10==-1){
                //crearListApuntadores pidiendo un nuevo bloque al bitsmap writePointerList(getBlockEmpty);
            }
            int pos=this->header.all_Header_size+
                    (this->metaData.pointersData.no_directN10*1024)+
                    ((val-1)*sizeof(int));
            this->fileOpened->seek(pos);
            int tempN;
            this->fileOpened->read(reinterpret_cast<char*>(&tempN),sizeof(int));
            if(tempN==-1){
                tempN=this->bitsmap.getBlockEmpty();
                this->fileOpened->seek(pos);
                this->fileOpened->write(reinterpret_cast<char*>(&tempN),sizeof(int));
            }else{
                nuevo=false;
            }
            pointer=tempN;
        }
    }
    if(nuevo)
        this->fullBlockWithRegister(pointer);

    return this->header.all_Header_size+(pointer*1024)+
            (this->metaData.nextDataFree-(val*this->registerCant))*
            (this->listFields.RegisterSize+sizeof(int));
}

void Table::setHeader(Header &head){
    this->header=head;
}
void Table::fullBlockWithRegister(int blockNum){
    this->fileOpened->seek(this->header.all_Header_size+(blockNum*1024));
    for(int i=0;i<this->registerCant;i++){
        QByteArray block;
        QString bytes;
        bytes.fill('\0',this->listFields.RegisterSize);
        block.append(bytes);
        int num=i+1+this->metaData.nextDataFree;
        block.append(reinterpret_cast<char*>(&num),sizeof(int));
        this->fileOpened->write(block);
    }
}

