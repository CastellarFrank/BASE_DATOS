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
    int b1=this->bitsmap.getBlockEmpty();
    int b2=this->bitsmap.getBlockEmpty();

    qDebug()<<"OCUPADOS"<<b1<<b2;
    qDebug()<<this->bitsmap.bits->at(b1);
    qDebug()<<this->bitsmap.bits->at(b2);
    if(b1==-1)
        return 2;
    else if(b2==-1)
        return 2;
    this->metaData[pos].setName(const_cast<char*>(name.toStdString().c_str()));
    this->metaData[pos].setDescrip(const_cast<char*>(descrip.toStdString().c_str()));
    this->metaData[pos].setFecha(const_cast<char*>(fecha.toStdString().c_str()));
    this->metaData[pos].free=false;
    this->metaData[pos].pointerToFields=b1;
    this->metaData[pos].pointersData.direct1=b2;
    this->metaData[pos].cant_camp=Field.campos.count();
    this->metaData[pos].primaryKey=key;
    this->metaData[pos].secondaryIndex=second;
    this->metaData[pos].register_size=Field.RegisterSize;
    this->campos.insert(pos,Field);
    this->createFirstDirectsData(pos);
    return 0;
}
void Table_Control::saveTablesInfo(){
    qDebug()<<"Cantidad campo to save:"<<this->campos.count();
    QMap<int,Table_Fields>::ConstIterator it;
    for(it=this->campos.constBegin();it!=this->campos.constEnd();it++){
        this->loadedFields[it.key()]=it.value();
        this->writeMetaDataTable(it.key());
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
    this->saveTablesInfo();
    this->tableOpened=new Table(num,this->metaData[num],this->loadedFields[num]);
    this->tableOpened->setBitsMap(this->bitsmap.bits);
    this->tableOpened->setFile(this->fileOpened);
    this->tableOpened->setHeader(this->header);
}
void Table_Control::setBitsMap(QBitArray *bits){
    this->bitsmap.assignBitArray(bits);
}
void Table_Control::createFirstDirectsData(int pos){
    Table_Fields temp=this->campos.value(pos);
    int registerCant=1024/(temp.RegisterSize+sizeof(int));
    this->metaData[pos].nextDataFree=0;
    this->fileOpened->seek(this->header.all_Header_size+this->metaData[pos].pointersData.direct1*1024);
    for(int i=0;i<registerCant;i++){
        QByteArray block;
        QString bytes;
        bytes.fill('\0',temp.RegisterSize);
        block.append(bytes);
        int num=i+1;
        qDebug()<<"FIRST DIRECT"<<num<<registerCant<<temp.RegisterSize;
        block.append(reinterpret_cast<char*>(&num),sizeof(int));
        this->fileOpened->write(block);
    }
}
void Table_Control::setFile(QFile *f){
    this->fileOpened=f;
}
void Table_Control::setHeader(Header &h){
    this->header=h;
}
void Table_Control::closeTable(){
    this->bitsmap.writeBitsMap();
    this->writeMetaDataTable(this->tableOpened->num_table);
}
void Table_Control::writeMetaDataTable(int num){
    this->fileOpened->seek(this->header.start_metaData+(num*sizeof(MetaDataTable)));
    this->fileOpened->write(reinterpret_cast<char*>(&this->metaData[num]),sizeof(MetaDataTable));
}
