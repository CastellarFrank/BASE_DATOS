#include "table_control.h"
#include <QDebug>

Table_Control::Table_Control()
{
}
void Table_Control::clearAll(){
    this->campos.clear();
    this->metaData.clear();
    this->loadedFields.clear();
}
int Table_Control::crearTable(QString name, QString descrip, QString fecha, Table_Fields Field, int key, int second){
    int pos=-1;
    for(int i=0;i<this->metaData.size();i++){
        if(this->metaData.at(i).free=='f'){
            qDebug()<<"POSICION BIEN "<<i;
            pos=i;
            this->metaData[i].free='t';
            break;
        }
    }
    if(pos==-1)
        return 1;
    int blockEmpty=bitsmap.getBlockEmpty();
    if(blockEmpty==-1)
        return 2;
    this->metaData[pos].setName(const_cast<char*>(name.toStdString().c_str()));
    this->metaData[pos].setDescrip(const_cast<char*>(descrip.toStdString().c_str()));
    this->metaData[pos].setFecha(const_cast<char*>(fecha.toStdString().c_str()));
    this->metaData[pos].free=false;
    this->metaData[pos].pointerToFields=blockEmpty;
    this->metaData[pos].pointersData.direct1=bitsmap.getBlockEmpty();
    this->metaData[pos].pointersData.direct2=bitsmap.getBlockEmpty();
    this->metaData[pos].pointersData.direct3=bitsmap.getBlockEmpty();
    this->metaData[pos].cant_camp=Field.campos.count();
    this->metaData[pos].primaryKey=key;
    this->metaData[pos].secondaryIndex=second;
    this->campos.insert(pos,Field);
    this->createFirstDirectsData(pos);
    return 0;
}
void Table_Control::saveTablesInfo(){
    qDebug()<<"Cantidad campo to save:"<<this->campos.count();
    QMap<int,Table_Fields>::ConstIterator it;
    for(it=this->campos.constBegin();it!=this->campos.constEnd();it++){
        this->fileOpened->seek(this->header.start_metaData+(it.key()*sizeof(MetaDataTable)));
        this->fileOpened->write(reinterpret_cast<char*>(&this->metaData[it.key()]),sizeof(MetaDataTable));
        Table_Fields tempField=it.value();
        this->fileOpened->seek(this->header.all_Header_size+this->metaData.at(it.key()).pointerToFields*1024);
        qDebug()<<this->fileOpened->pos();
        for(int i=0;i<tempField.campos.count();i++){
            qDebug()<<tempField.campos[i].name<<i<<this->header.all_Header_size+this->metaData.at(it.key()).pointerToFields*1024+(i*sizeof(Field))<<it.key()<<this->metaData.at(it.key()).pointerToFields*1024;
            this->fileOpened->write(reinterpret_cast<char*>(&tempField.campos[i]),sizeof(Field));
        }
    }
    this->campos.clear();
}
void Table_Control::openTable(int num){
    this->tableOpened=new Table(num,this->metaData[num].pointerToFields,this->metaData[num].primaryKey,
                                this->metaData[num].secondaryIndex,this->metaData[num].cant_camp);
    this->tableOpened->setPointers(this->metaData[num].pointersData,this->metaData[num].pointerArbolB,
                                   this->metaData[num].pointerIS,this->metaData[num].pointerInvertedList);
    this->tableOpened->setBitsMap(this->bitsmap);
    this->tableOpened->setFields(this->loadedFields[num]);
}
void Table_Control::setBitsMap(BitsMap &bits){
    this->bitsmap=bits;
}
void Table_Control::createFirstDirectsData(int pos){
    Table_Fields temp=this->campos.value(pos);
    int registerCant=1024/temp.RegisterSize;
    this->metaData[pos].nextDataFree=0;
    this->fileOpened->seek(this->header.all_Header_size+this->metaData[pos].pointersData.direct1*1024);
    for(int i=0;i<registerCant;i++){
        char bytes[temp.RegisterSize];
        memset(bytes,0,temp.RegisterSize);
        QByteArray block;
        QDataStream val(&block, QIODevice::WriteOnly);
        val.setVersion(QDataStream::Qt_4_6);
        val<<bytes;
        val<<(i+1);
        this->fileOpened->write(block);
    }
}
void Table_Control::setFile(QFile *f){
    this->fileOpened=f;
}
void Table_Control::setHeader(Header &h){
    this->header=h;
}
