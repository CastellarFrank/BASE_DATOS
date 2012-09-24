#include "table.h"

Table::Table(int num, MetaDataTable *meta, Table_Fields cam)
{
    this->num_table=num;
    this->metaData=meta;
    this->listFields=cam;
    this->registerCant=1024/(this->metaData->register_size+sizeof(int));
    this->stackRegisterCant=this->metaData->register_cant;
    this->duplicates=0;
    this->MaxKey=84;
    this->minKey=this->MaxKey/2;
}

void Table::setBitsMap(QBitArray *bits){
    this->bitsmap.assignBitArray(bits);
}

int Table::addRegisterToFile(QByteArray info){
    int tempV=this->metaData->nextDataFree/this->registerCant;
    int writeDirection=this->getBytesPositionToWriteRegister(tempV);
    if(writeDirection==-1 || writeDirection==-2)
        return writeDirection;
    int pointer=writeDirection;
    int key=this->getPrimaryKeyValueFromQByteRegister(info);
    if(this->insertKey(key,pointer)==1){
        return -3;
    }
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
    int val;
    bool t=false;
    int i=0;
    for(;i<this->StackRegisters.count();i++){
        if(!this->bitsmap.avaibleBlockCant(2)){
            t=true;
            break;
        }
        val=this->addRegisterToFile(this->StackRegisters[i]);
        if(val==-1 || val==-2){
            t=true;
            break;
        }
    }
    int cant;
    if(t){
        this->metaData->register_cant=this->stackRegisterCant-(this->StackRegisters.count()-(i+1));
        cant=this->StackRegisters.count()-(i+1);
    }else{
        this->metaData->register_cant=this->stackRegisterCant;
        cant=this->StackRegisters.count();
    }
    this->StackRegisters.clear();

    return cant;
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
//int Table::createNewNodoTree(){
//    int pointer;
//    if(this->metaData->createNodoTreePos==0){
//       int tempP=this->bitsmap.getBlockEmpty();
//       if(tempP==-1)
//           return -1;
//       this->metaData->pointerArbolB.direct1=tempP;
//       pointer=tempP;
//   }else if(this->metaData->createNodoTreePos==1){
//       int tempP=this->bitsmap.getBlockEmpty();
//       if(tempP==-1)
//           return -1;
//       this->metaData->pointerArbolB.direct2=tempP;
//       pointer=tempP;
//   }else if(this->metaData->createNodoTreePos==2){
//       int tempP=this->bitsmap.getBlockEmpty();
//       if(tempP==-1)
//           return -1;
//       this->metaData->pointerArbolB.direct3=tempP;
//       pointer=tempP;
//   }else{
//       int iFile=(this->metaData->createNodoTreePos-3)/(1024/sizeof(int));
//       if(iFile==0){
//           if(this->metaData->pointerArbolB.no_directN10==-1){
//               int tempP=this->bitsmap.getBlockEmpty();
//               if(tempP==-1)
//                   return -1;
//               this->metaData->pointerArbolB.no_directN10=tempP;
//               this->creatListPointers(tempP);
//           }
//           int pos=this->header.all_Header_size+
//                   (this->metaData->pointerArbolB.no_directN10*1024)+
//                   ((this->metaData->createNodoTreePos-3-(iFile*(1024/sizeof(int))))*sizeof(int));
//           this->fileOpened->seek(pos);
//           int valTemp=this->bitsmap.getBlockEmpty();
//           if(valTemp==-1)
//               return -1;
//           this->fileOpened->write(reinterpret_cast<char*>(&valTemp),sizeof(int));
//           pointer=valTemp;
//       }else if(iFile==1){
//           if(this->metaData->pointerArbolB.no_directN11==-1){
//               int tempP=this->bitsmap.getBlockEmpty();
//               if(tempP==-1)
//                   return -1;
//               this->metaData->pointerArbolB.no_directN11=tempP;
//               this->creatListPointers(tempP);
//           }
//           int pos=this->header.all_Header_size+
//                   (this->metaData->pointerArbolB.no_directN11*1024)+
//                   ((this->metaData->createNodoTreePos-3-(iFile*(1024/sizeof(int))))*sizeof(int));
//           this->fileOpened->seek(pos);
//           int valTemp=this->bitsmap.getBlockEmpty();
//           if(valTemp==-1)
//               return -1;
//           this->fileOpened->write(reinterpret_cast<char*>(&valTemp),sizeof(int));
//           pointer=valTemp;
//       }else{
//           if((iFile-2)>=(1024/sizeof(int)))
//               return -2;
//           if(this->metaData->pointerArbolB.no_directN2==-1){
//               int tempP=this->bitsmap.getBlockEmpty();
//               if(tempP==-1)
//                   return -1;
//               this->metaData->pointerArbolB.no_directN2=tempP;
//               this->creatListPointers(tempP);
//           }
//           int pos=this->header.all_Header_size+
//                   (this->metaData->pointerArbolB.no_directN2*1024)+
//                   ((iFile-2)*sizeof(int));
//           this->fileOpened->seek(pos);
//           int tempN;
//           this->fileOpened->read(reinterpret_cast<char*>(&tempN),sizeof(int));
//           if(tempN==-1){
//               tempN=this->bitsmap.getBlockEmpty();
//               if(tempN==-1)
//                   return -1;
//               this->fileOpened->seek(pos);
//               this->fileOpened->write(reinterpret_cast<char*>(&tempN),sizeof(int));
//               this->creatListPointers(tempN);
//           }
//           pos=this->header.all_Header_size+
//               (tempN*1024)+
//               ((this->metaData->createNodoTreePos-3-(iFile*(1024/sizeof(int))))*sizeof(int));
//           tempN=this->bitsmap.getBlockEmpty();
//           if(tempN==-1)
//               return -1;
//           this->fileOpened->seek(pos);
//           this->fileOpened->write(reinterpret_cast<char*>(&tempN),sizeof(int));
//           pointer=tempN;
//       }
//   }
//   nodoTree newBlock;
//   this->writeNodoTree(pointer,newBlock);
//   this->metaData->createNodoTreePos++;
//   return pointer;
//}
nodoTree Table::getNodoTree(int num){
    nodoTree temp;

    int pos=getPositionNodoTree(num);
    this->fileOpened->seek(pos);
    this->fileOpened->read(reinterpret_cast<char*>(&temp),sizeof(nodoTree));
    return temp;
}

int Table::getPositionNodoTree(int num){
    return this->header.all_Header_size+(num*1024);
}
void Table::writeNodoTree(int num, nodoTree nodo){
    int pos=this->getPositionNodoTree(num);
    this->fileOpened->seek(pos);
    this->fileOpened->write(reinterpret_cast<char*>(&nodo),sizeof(nodoTree));
}

//int Table::getPositionNodoTreeByPosNodo(int num){
//    int pointer;
//    if(num==0){
//        if(this->metaData->pointerArbolB.direct1==-1)
//            return -1;
//        pointer=this->metaData->pointerArbolB.direct1;
//    }else if(num==1){
//        if(this->metaData->pointerArbolB.direct1==-1)
//            return -1;
//        pointer=this->metaData->pointerArbolB.direct2;
//    }else if(num==2){
//        if(this->metaData->pointerArbolB.direct2==-1)
//            return -1;
//        pointer=this->metaData->pointerArbolB.direct3;
//    }else{
//        int iFile=(num-3)/(1024/sizeof(int));
//        if(iFile==0){
//            if(this->metaData->pointerArbolB.no_directN10==-1){
//                return -1;
//            }
//            int pos=this->header.all_Header_size+
//                    (this->metaData->pointerArbolB.no_directN10*1024)+
//                    ((num-3-(iFile*(1024/sizeof(int))))*sizeof(int));
//            this->fileOpened->seek(pos);
//            int tempValue;
//            this->fileOpened->read(reinterpret_cast<char*>(&tempValue),sizeof(int));
//            if(tempValue==-1){
//                return -1;
//            }
//            pointer=tempValue;
//        }else if(iFile==1){
//            if(this->metaData->pointerArbolB.no_directN11==-1){
//                return -1;
//            }
//            int pos=this->header.all_Header_size+
//                    (this->metaData->pointerArbolB.no_directN11*1024)+
//                    ((num-3-(iFile*(1024/sizeof(int))))*sizeof(int));
//            this->fileOpened->seek(pos);
//            int tempValue;
//            this->fileOpened->read(reinterpret_cast<char*>(&tempValue),sizeof(int));
//            if(tempValue==-1){
//                return -1;
//            }
//            pointer=tempValue;
//        }else{
//            if((iFile-2)>=(1024/sizeof(int)))
//                return -1;
//            if(this->metaData->pointerArbolB.no_directN2==-1){
//                return -1;
//            }
//            int pos=this->header.all_Header_size+
//                    (this->metaData->pointerArbolB.no_directN2*1024)+
//                    ((iFile-2)*sizeof(int));
//            this->fileOpened->seek(pos);
//            int tempN;
//            this->fileOpened->read(reinterpret_cast<char*>(&tempN),sizeof(int));
//            if(tempN==-1)
//                return -1;
//            pos=this->header.all_Header_size+
//                (tempN*1024)+
//                ((this->metaData->createNodoTreePos-3-(iFile*(1024/sizeof(int))))*sizeof(int));
//            this->fileOpened->seek(pos);
//            this->fileOpened->read(reinterpret_cast<char*>(&tempN),sizeof(int));
//            pointer=tempN;
//        }
//    }
//    return pointer;
//}
int Table::inserValueAtTheTree(int rrn, keyTree key, int *promo_r_child, keyTree *promo_key){
    qDebug()<<"\t#Main Calling#"<<rrn;
    nodoTree page,newPage;
    int promoted,pos,p_b_rrn,founded;
    keyTree p_b_key;
    if(rrn==-1){
        *promo_key=key;
        *promo_r_child=-1;
        qDebug()<<"\t#Current NULO#"<<"#PROMOTED#";
        return 1;
    }
    qDebug()<<"\t#Read NODOTREE put into page#"<<rrn;
    page=this->getNodoTree(rrn);
    founded=this->searchValueIntoAnArray(page.keys,key.value,page.keyCount,pos);
    //founded=this->search_node(key,&page,&pos);
    qDebug()<<"\t#Searching key into page#"<<key.value<<key.pointer<<"Pos:"<<pos;
    if(founded==0){
        qDebug()<<"\t#Key duplicated#"<<"#Error#";
        return 2;
    }
    qDebug()<<"\t#Recursiva#";
    promoted=this->inserValueAtTheTree(page.hijos[pos],key,&p_b_rrn,&p_b_key);
    qDebug()<<"\t#Second LVL Retorning#";
    if(promoted==2 || promoted==0){
        qDebug()<<"\t#Error or No Promoted#";
        return promoted;
    }
    if(page.keyCount<this->MaxKey){
        qDebug()<<"\t#Insert In page (no fill out)#"<<"Page"<<p_b_rrn<<p_b_key.value<<p_b_key.pointer<<"Child:"<<p_b_rrn;
        this->insertInPage(p_b_key,p_b_rrn,&page);
        qDebug()<<"\t#Write page into File#"<<rrn;
        this->writeNodoTree(rrn,page);
        return 0;
    }else{
        int block=this->bitsmap.getBlockEmpty();
        qDebug()<<"\t#Split new page#"<<block;
        if(block==-1)
            return 2;
        this->splitArray(p_b_key,p_b_rrn,&page,promo_key,promo_r_child,&newPage,block);
        qDebug()<<"\t#Write Page#"<<rrn;
        this->writeNodoTree(rrn,page);
        qDebug()<<"\t#Write NewPage#"<<*promo_r_child;
        this->writeNodoTree(*promo_r_child,newPage);
        return 1;
    }
}

int Table::searchValueIntoAnArray(keyTree Arreglo[], int elemento, int size,int &pos){
    int centro=0,inferior=0,superior=size-1;
    while(inferior <= superior){
        centro=(superior + inferior)/2;
        if(Arreglo[centro].value==elemento){
            pos=centro;
            return 0;
        }else if (Arreglo[centro].value > elemento)
            superior = centro-1;
        else
            inferior = centro+1;
    }
    pos=inferior;
    return -1;
}
int Table::insertKey(int key,int pointer){
    qDebug()<<"#InsertKey#"<<key<<pointer;
    keyTree Key,promo_key;
    Key.pointer=pointer;
    Key.value=key;

    int promoted,promo_rrn;
    promoted=this->inserValueAtTheTree(this->metaData->treeStart,Key,&promo_rrn,&promo_key);
    if(promoted==1){
        this->metaData->treeStart=this->createNodoTree(promo_key,this->metaData->treeStart,promo_rrn);
        qDebug()<<"\t#Created New LVL#"<<this->metaData->treeStart;
    }
    if(promoted==2){
        this->duplicates++;
        return 1;
    }else{
        return 0;
    }
}
int Table::createNodoTree(keyTree key, int left, int right){
    nodoTree temp;
    int block=this->bitsmap.getBlockEmpty();
    temp.keys[0]=key;
    temp.hijos[0]=left;
    temp.hijos[1]=right;
    temp.keyCount++;
    if(block!=-1){
        this->writeNodoTree(block,temp);
    }
    return block;
}
void Table::insertInPage(keyTree key, int r_chid, nodoTree *p_page){
    int i;
    for(i=p_page->keyCount;key.value<p_page->keys[i-1].value&&i>0;i--){
        p_page->keys[i]=p_page->keys[i-1];
        p_page->hijos[i+1]=p_page->hijos[i];
    }
    p_page->keyCount++;
    p_page->keys[i]=key;
    p_page->hijos[i+1]=r_chid;
}

void Table::splitArray(keyTree key, int r_child, nodoTree *p_oldpage, keyTree *promo_key, int *promo_r_child,
                       nodoTree *p_newpage,int block){
    int i;
    keyTree workkeys[this->MaxKey+1];
    int workch[this->MaxKey+2];

    for(i=0;i<this->MaxKey;i++){
        workkeys[i]=p_oldpage->keys[i];
        workch[i]=p_oldpage->hijos[i];
    }
    workch[i]=p_oldpage->hijos[i];

    for(i=this->MaxKey;key.value<workkeys[i-1].value&&i>0;i--){
        workkeys[i]=workkeys[i-1];
        workch[i+1]=workch[i];
    }
    workkeys[i]=key;
    workch[i+1]=r_child;
    *promo_r_child=block;
    for(i=0;i<this->minKey;i++){
        p_oldpage->keys[i]=workkeys[i];
        p_oldpage->hijos[i]=workch[i];
        p_newpage->keys[i]=workkeys[i+1+this->minKey];
        p_newpage->hijos[i]=workch[i+1+this->minKey];
        p_oldpage->keys[i+this->minKey].value=-1;
        p_oldpage->hijos[i+1+this->minKey]=-1;
    }
    p_oldpage->hijos[this->minKey]=workch[this->minKey];
    p_newpage->hijos[this->minKey]=workch[i+1+this->minKey];
    p_newpage->keyCount=this->MaxKey-this->minKey;
    p_oldpage->keyCount=this->minKey;
    *promo_key=workkeys[this->minKey];

}
int Table::search_node(keyTree key, nodoTree *p_page, int *pos){
    int i;
    for(i=0;i<p_page->keyCount&&key.value>p_page->keys[i].value;i++);
    *pos=i;
    if(*pos<p_page->keyCount&&key.value==p_page->keys[*pos].value)
        return 1;
    else
        return 0;
}
int Table::getPrimaryKeyValueFromQByteRegister(QByteArray array){
    int primaryField=this->metaData->primaryKey;
    int stackSize=0;
    for(int i=0;i<primaryField;i++){
        stackSize+=this->listFields.campos.at(i).size;
    }
    array.remove(0,stackSize);
    int keyValue;
    memcpy(&keyValue,array,sizeof(int));
    return keyValue;
}
QStringList Table::getRegisterUsingTree(int key){
    qDebug()<<"Entro GetRegisterUsingTree";
    int found_rrn=-1,found_pos=-1;
    int finded=this->searchInTree(this->metaData->treeStart,key,&found_rrn,&found_pos);
    qDebug()<<"Finded?"<<finded;
    if(finded==-1){
        QStringList stringList;
        return stringList;
    }
    nodoTree temp=this->getNodoTree(found_rrn);
    this->fileOpened->seek(temp.keys[found_pos].pointer);
    QByteArray array=this->fileOpened->read(this->metaData->register_size);
    return this->convertQByteToStringList(array);
}
int Table::searchInTree(int rrn, int key, int *found_rrn, int *found_pos){
    qDebug()<<"CURRENT"<<rrn<<key;
    if(rrn==-1){
        return -1;
    }else{
        nodoTree page;
        int pos;
        page=this->getNodoTree(rrn);
        qDebug()<<"KEY COUNt"<<page.keyCount;
        int founded=this->searchValueIntoAnArray(page.keys,key,page.keyCount,pos);
        qDebug()<<"ENCONTRADO ARRAY"<<founded<<"Pos:"<<pos;
        if(founded==0){
            *found_rrn=rrn;
            *found_pos=pos;
            return 0;
        }else{
            return searchInTree(page.hijos[pos],key,found_rrn,found_pos);
        }
    }
}

