#include "table.h"

Table::Table(int num, MetaDataTable &meta, Table_Fields cam)
{
    this->num_table=num;
    this->metaData=meta;
    this->listFields=cam;
    this->registerCant=1024/(this->metaData.register_size+sizeof(int));
}

void Table::setBitsMap(QBitArray *bits){
    this->bitsmap.assignBitArray(bits);
}

int Table::addRegisterToFile(QByteArray info){
    qDebug()<<"ADDING REGISTER"<<metaData.nextDataFree<<this->metaData.register_size;
    int tempV=this->metaData.nextDataFree/this->registerCant;
    int writeDirection=this->getBytesPositionToWriteRegister(tempV);
    qDebug()<<"Dirección a escribir"<<writeDirection;
    this->fileOpened->seek(writeDirection+this->metaData.register_size);
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
    qDebug()<<"Archivo #"<<val;
    if(val==0){
        nuevo=false;
        pointer=this->metaData.pointersData.direct1;
        qDebug()<<"Apuntador del bloque"<<pointer;
    }else if(val==1){
        if(this->metaData.pointersData.direct2==-1){
            int pointValue=this->bitsmap.getBlockEmpty();
            if(pointValue==-1)
                return -1;
            qDebug()<<"NUEVO"<<pointValue;
            this->metaData.pointersData.direct2=pointValue;
        }else{
            nuevo=false;
        }
        pointer=this->metaData.pointersData.direct2;
    }else if(val==2){
        if(this->metaData.pointersData.direct3==-1){
            int pointValue=this->bitsmap.getBlockEmpty();
            if(pointValue==-1)
                return -1;
            this->metaData.pointersData.direct3=pointValue;
        }else{
            nuevo=false;
        }
        pointer=this->metaData.pointersData.direct3;
    }else{
        iFile=(val-3)/(1024/sizeof(int));
        if(iFile==0){
            if(this->metaData.pointersData.no_directN10==-1){
                int pointValue=this->bitsmap.getBlockEmpty();
                if(pointValue==-1)
                    return -1;
                this->metaData.pointersData.no_directN10=pointValue;
                this->creatListPointers(this->metaData.pointersData.no_directN10);
            }
            int pos=this->header.all_Header_size+
                    (this->metaData.pointersData.no_directN10*1024)+
                    ((val-3-(iFile*(1024/sizeof(int))))*sizeof(int));
            this->fileOpened->seek(pos);
            int tempN;
            this->fileOpened->read(reinterpret_cast<char*>(&tempN),sizeof(int));
            if(tempN==-1){
                tempN=this->bitsmap.getBlockEmpty();
                if(tempN==-1)
                    return -1;
                this->fileOpened->seek(pos);
                this->fileOpened->write(reinterpret_cast<char*>(&tempN),sizeof(int));
            }else{
                nuevo=false;
            }
            pointer=tempN;
        }else if(iFile==1){
            if(this->metaData.pointersData.no_directN11==-1){
                int pointValue=this->bitsmap.getBlockEmpty();
                if(pointValue==-1)
                    return -1;
                this->metaData.pointersData.no_directN11=pointValue;
                this->creatListPointers(this->metaData.pointersData.no_directN11);
            }
            int pos=this->header.all_Header_size+
                    (this->metaData.pointersData.no_directN11*1024)+
                    ((val-3-(iFile*(1024/sizeof(int))))*sizeof(int));
            this->fileOpened->seek(pos);
            int tempN;
            this->fileOpened->read(reinterpret_cast<char*>(&tempN),sizeof(int));
            if(tempN==-1){
                tempN=this->bitsmap.getBlockEmpty();
                if(tempN==-1)
                    return -1;
                this->fileOpened->seek(pos);
                this->fileOpened->write(reinterpret_cast<char*>(&tempN),sizeof(int));
            }else{
                nuevo=false;
            }
            pointer=tempN;
        }else{
            if((iFile-2)>=(1024/sizeof(int))*(1024/sizeof(int)))
                return -2;
            if(this->metaData.pointersData.no_directN2==-1){
                int pointValue=this->bitsmap.getBlockEmpty();
                if(pointValue==-1)
                    return -1;
                this->metaData.pointersData.no_directN2=pointValue;
                this->creatListPointers(pointValue);
            }
        }
    }
    if(nuevo){
        qDebug()<<"Pointer NUEVO"<<pointer;
        this->fullBlockWithRegister(pointer);
    }

    qDebug()<<"Salida"<<header.all_Header_size<<pointer*1024<<((this->metaData.nextDataFree-(val*this->registerCant)))<<(this->listFields.RegisterSize+sizeof(int))<<((this->metaData.nextDataFree-(val*this->registerCant))*(this->listFields.RegisterSize+sizeof(int)));

    return this->header.all_Header_size+(pointer*1024)+
            ((this->metaData.nextDataFree-(val*this->registerCant))*
            (this->metaData.register_size+sizeof(int)));
}

void Table::setHeader(Header &head){
    this->header=head;
}
void Table::fullBlockWithRegister(int blockNum){
    this->fileOpened->seek(this->header.all_Header_size+(blockNum*1024));
    for(int i=0;i<this->registerCant;i++){
        QByteArray block;
        QString bytes;
        bytes.fill('\0',this->metaData.register_size);
        block.append(bytes);
        int num=i+1+this->metaData.nextDataFree;
        qDebug()<<"NUMERO NEXT"<<num;
        block.append(reinterpret_cast<char*>(&num),sizeof(int));
        this->fileOpened->write(block);
    }
}
void Table::creatListPointers(int blockNum){
    this->fileOpened->seek(this->header.all_Header_size+(blockNum*1024));
    int tempCiclos=1024/sizeof(int);
    int val[tempCiclos];
    for(int i=0;i<tempCiclos;i++){
        val[i]=-1;
    }
    this->fileOpened->write(reinterpret_cast<char*>(&val),(sizeof(int)*tempCiclos));
}
int Table::addAllRegistersToFile(){
    for(int i=0;i<this->StackRegisters.count();i++){
        this->addRegisterToFile(this->StackRegisters[i]);
    }
}
int Table::addRegister(QList<QString> Regis){
        int temp=this->canAddRegister();
        if(temp==0){
            QByteArray main=this->joinToMe(Regis);
            this->StackRegisters.push_back(main);
            this->metaData.register_cant++;
        }
        return temp;
}
int Table::canAddRegister(){
    int cantFiles=3+(2*(1024/sizeof(int)))+((1024/sizeof(int))*(1024/sizeof(int)));
    int abableRegistersCant=cantFiles*this->registerCant;
    if(abableRegistersCant>this->metaData.register_cant)
        return 0;
    else
        return -2;
}

QByteArray Table::joinToMe(QList<QString> list){
    QByteArray Main;
    for(int i=0;i<this->metaData.cant_camp;i++){
        QByteArray temp;
        if(this->listFields.campos.at(i).type=='s'){
            temp.append(list[i]);
            int rest=this->listFields.campos.at(i).size-list[i].size();
            if(rest>0){
                QString val;
                val.fill(QChar('\0'),rest);
                temp.append(val);
            }
        }else if(this->listFields.campos.at(i).type=='i'){
            int val=list[i].toInt();
            qDebug()<<"Numero"<<val;
            temp.append(reinterpret_cast<char*>(&val),sizeof(int));
        }else if(this->listFields.campos.at(i).type=='f'){
            float val=list[i].toFloat();
            temp.append(reinterpret_cast<char*>(&val),sizeof(float));
        }else if(this->listFields.campos.at(i).type=='c'){
            char val=list[i].at(0).toAscii();
            qDebug()<<"CARACTER"<<val;
            temp.append(reinterpret_cast<char*>(&val),sizeof(char));
        }else if(this->listFields.campos.at(i).type=='d'){
            temp.append(list[i]);
        }
        Main.append(temp);
    }
    return Main;
}
