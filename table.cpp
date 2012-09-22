#include "table.h"

Table::Table(int num, MetaDataTable *meta, Table_Fields cam)
{
    this->num_table=num;
    this->metaData=meta;
    this->listFields=cam;
    this->registerCant=1024/(this->metaData->register_size+sizeof(int));
    this->stackRegisterCant=this->metaData->register_cant;
}

void Table::setBitsMap(QBitArray *bits){
    this->bitsmap.assignBitArray(bits);
}

int Table::addRegisterToFile(QByteArray info){
    int tempV=this->metaData->nextDataFree/this->registerCant;
    int writeDirection=this->getBytesPositionToWriteRegister(tempV);
    if(writeDirection==-1 || writeDirection==-2)
        return writeDirection;
    this->fileOpened->seek(writeDirection+this->metaData->register_size);
    int num;
    this->fileOpened->read(reinterpret_cast<char*>(&num),sizeof(int));
    this->metaData->nextDataFree=num;
    this->fileOpened->seek(writeDirection);
    num=-1;
    info.append(reinterpret_cast<char*>(&num),sizeof(int));
    this->fileOpened->write(info);
    return 0;
}

void Table::setFile(QFile *file){
    this->fileOpened=file;
    this->bitsmap.setFile(file);
}
int Table::getBytesPositionToWriteRegister(int val){
    bool nuevo=true;
    int pointer;
    int iFile;
    if(val==0){
        nuevo=false;
        pointer=this->metaData->pointersData.direct1;
    }else if(val==1){
        if(this->metaData->pointersData.direct2==-1){
            int pointValue=this->bitsmap.getBlockEmpty();
            if(pointValue==-1)
                return -1;
            this->metaData->pointersData.direct2=pointValue;
        }else{
            nuevo=false;
        }
        pointer=this->metaData->pointersData.direct2;
    }else if(val==2){
        if(this->metaData->pointersData.direct3==-1){
            int pointValue=this->bitsmap.getBlockEmpty();
            if(pointValue==-1)
                return -1;
            this->metaData->pointersData.direct3=pointValue;
        }else{
            nuevo=false;
        }
        pointer=this->metaData->pointersData.direct3;
    }else{
        iFile=(val-3)/(1024/sizeof(int));
        if(iFile==0){
            if(this->metaData->pointersData.no_directN10==-1){
                int pointValue=this->bitsmap.getBlockEmpty();
                if(pointValue==-1)
                    return -1;
                this->metaData->pointersData.no_directN10=pointValue;
                this->creatListPointers(this->metaData->pointersData.no_directN10);
            }
            int pos=this->header.all_Header_size+
                    (this->metaData->pointersData.no_directN10*1024)+
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
            if(this->metaData->pointersData.no_directN11==-1){
                int pointValue=this->bitsmap.getBlockEmpty();
                if(pointValue==-1)
                    return -1;
                this->metaData->pointersData.no_directN11=pointValue;
                this->creatListPointers(this->metaData->pointersData.no_directN11);
            }
            int pos=this->header.all_Header_size+
                    (this->metaData->pointersData.no_directN11*1024)+
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
            if((iFile-2)>=(1024/sizeof(int)))
                return -2;
            if(this->metaData->pointersData.no_directN2==-1){
                int pointValue=this->bitsmap.getBlockEmpty();
                if(pointValue==-1)
                    return -1;
                this->metaData->pointersData.no_directN2=pointValue;
                this->creatListPointers(pointValue);
            }
            int pos=this->header.all_Header_size+
                    (this->metaData->pointersData.no_directN2*1024)+
                    ((iFile-2)*sizeof(int));
            this->fileOpened->seek(pos);
            int tempN;
            this->fileOpened->read(reinterpret_cast<char*>(&tempN),sizeof(int));
            if(tempN==-1){
                tempN=this->bitsmap.getBlockEmpty();
                if(tempN==-1)
                    return -1;
                this->fileOpened->seek(pos);
                this->fileOpened->write(reinterpret_cast<char*>(&tempN),sizeof(int));
                this->creatListPointers(tempN);
            }
            pos=this->header.all_Header_size+
                (tempN*1024)+
                ((val-3-(iFile*(1024/sizeof(int))))*sizeof(int));
            this->fileOpened->seek(pos);
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
        }
    }
    if(nuevo){
        this->fullBlockWithRegister(pointer);
    }
    return this->header.all_Header_size+(pointer*1024)+
            ((this->metaData->nextDataFree-(val*this->registerCant))*
            (this->metaData->register_size+sizeof(int)));
}

void Table::setHeader(Header &head){
    this->header=head;
}
void Table::fullBlockWithRegister(int blockNum){
    this->fileOpened->seek(this->header.all_Header_size+(blockNum*1024));
    for(int i=0;i<this->registerCant;i++){
        QByteArray block;
        QString bytes;
        bytes.fill('\0',this->metaData->register_size);
        block.append(bytes);
        int num=i+1+this->metaData->nextDataFree;
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
    this->metaData->register_cant=this->stackRegisterCant;
}
int Table::addRegister(QList<QString> Regis){
        int temp=this->canAddRegister();
        if(temp==0){
            QByteArray main=this->joinToMe(Regis);
            this->StackRegisters.push_back(main);
            this->stackRegisterCant++;
        }
        return temp;
}
int Table::canAddRegister(){
    int cantFiles=3+(2*(1024/sizeof(int)))+((1024/sizeof(int))*(1024/sizeof(int)));
    int abableRegistersCant=cantFiles*this->registerCant;
    if(abableRegistersCant>this->stackRegisterCant)
        return 0;
    else
        return -2;
}

QByteArray Table::joinToMe(QList<QString> list){
    QByteArray Main;
    for(int i=0;i<this->metaData->cant_camp;i++){
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
            temp.append(reinterpret_cast<char*>(&val),sizeof(int));
        }else if(this->listFields.campos.at(i).type=='f'){
            float val=list[i].toFloat();
            temp.append(reinterpret_cast<char*>(&val),sizeof(float));
        }else if(this->listFields.campos.at(i).type=='c'){
            char val=list[i].at(0).toAscii();
            temp.append(reinterpret_cast<char*>(&val),sizeof(char));
        }else if(this->listFields.campos.at(i).type=='d'){
            temp.append(list[i]);
        }
        Main.append(temp);
    }
    return Main;
}
QStringList Table::getRegister(int num){
    QStringList temp;
    int position=this->getPositionRegister(num);
    if(position==-1 || position==-2)
        return temp;
    this->fileOpened->seek(position+this->metaData->register_size);
    int numTemp;
    this->fileOpened->read(reinterpret_cast<char*>(&numTemp),sizeof(int));
    if(numTemp!=-1)
        return temp;
    this->fileOpened->seek(position);
    QByteArray registros=this->fileOpened->read(this->metaData->register_size);
    return convertQByteToStringList(registros);
}
int Table::getPositionRegister(int num){
    int file=num/this->registerCant;
    int pointerToFile;
    int nfile;
    if(file==0){
        pointerToFile=this->metaData->pointersData.direct1;
    }else if(file==1){
        pointerToFile=this->metaData->pointersData.direct2;
    }else if(file==2){
        pointerToFile=this->metaData->pointersData.direct3;
    }else{
        nfile=(file-3)/(1024/sizeof(int));
        if(nfile==0){
            if(this->metaData->pointersData.no_directN10==-1)
                return -1;
            int pos=this->header.all_Header_size+
                    (this->metaData->pointersData.no_directN10*1024)+
                    ((file-3-(nfile*(1024/sizeof(int))))*sizeof(int));
            this->fileOpened->seek(pos);
            int numTemp;
            this->fileOpened->read(reinterpret_cast<char*>(&numTemp),sizeof(int));
            pointerToFile=numTemp;
        }else if(nfile==1){
            if(this->metaData->pointersData.no_directN11==-1)
                return -1;
            int pos=this->header.all_Header_size+
                    (this->metaData->pointersData.no_directN11*1024)+
                    ((file-3-(nfile*(1024/sizeof(int))))*sizeof(int));
            this->fileOpened->seek(pos);
            int numTemp;
            this->fileOpened->read(reinterpret_cast<char*>(&numTemp),sizeof(int));
            pointerToFile=numTemp;
        }else{
            if((nfile-2)>=(1024/sizeof(int)))
                return -2;
            if(this->metaData->pointersData.no_directN2==-1)
                return -1;
            int pos=this->header.all_Header_size+
                    (this->metaData->pointersData.no_directN2*1024)+
                    ((nfile-2)*sizeof(int));
            this->fileOpened->seek(pos);
            int numTemp;
            this->fileOpened->read(reinterpret_cast<char*>(&numTemp),sizeof(int));
            if(numTemp==-1)
                return -1;
            pos=this->header.all_Header_size+
                (numTemp*1024)+
                ((file-3-(nfile*(1024/sizeof(int))))*sizeof(int));
            this->fileOpened->seek(pos);
            this->fileOpened->read(reinterpret_cast<char*>(&numTemp),sizeof(int));
            pointerToFile=numTemp;
        }
    }
    return this->header.all_Header_size+(pointerToFile*1024)+
            ((num-(file*this->registerCant))*
            (this->metaData->register_size+sizeof(int)));
}

QStringList Table::convertQByteToStringList(QByteArray array){
    QStringList temp;
    QString fieldConverted;
    for(int i=0;i<this->metaData->cant_camp;i++){
        if(this->listFields.campos.at(i).type=='c'){
            char c;
            memcpy(&c,array,1);
            array.remove(0,1);
            QChar cV(c);
            QString valS(cV);
            fieldConverted=valS;
        }else if(this->listFields.campos.at(i).type=='i'){
            int val;
            memcpy(&val,array,sizeof(int));
            array.remove(0,sizeof(int));
            QString valS;
            fieldConverted=valS.setNum(val);
        }else if(this->listFields.campos.at(i).type=='f'){
            float val;
            memcpy(&val,array,sizeof(float));
            array.remove(0,sizeof(float));
            QString valS;
            fieldConverted=valS.setNum(val);
        }else if(this->listFields.campos.at(i).type=='d'){
            QString valS(array.left(this->listFields.campos.at(i).size));
            array.remove(0,this->listFields.campos.at(i).size);
            fieldConverted=valS;
        }else if(this->listFields.campos.at(i).type=='s'){
            QString valS(array.left(this->listFields.campos.at(i).size));
            array.remove(0,this->listFields.campos.at(i).size);
            fieldConverted=valS;
        }
        temp.push_back(fieldConverted);
    }
    return temp;
}

