#include "mydb.h"

MyDB::MyDB()
{
}

int MyDB::createDB(int tamanio, QString path){
    QFile archivo(path);
    if(!archivo.open(QIODevice::ReadWrite))
        return 1;
    this->header.tamanio=tamanio;
    QString temp=this->getNameWithoutExtention(path);
    this->header.setName(const_cast<char*>(temp.toStdString().c_str()));
    this->header.setAuthors("Industrias BuenRecord");
    qDebug()<<"Bytes Header"<<sizeof(Header);
    this->header.countBlocksBitsMap=this->getBlockCant(tamanio*1024*1024,this->SIZE_BLOCK);
    qDebug()<<"cantidad de bloques vacios"<<this->header.countBlocksBitsMap;
    this->header.sizeBitsMap=this->header.countBlocksBitsMap/8;
    qDebug()<<"Bytes del Bitsmap"<<this->header.sizeBitsMap;
    this->header.start_metaData=sizeof(Header)+this->header.sizeBitsMap;
    qDebug()<<"Byte donde inicia la metadata"<<this->header.start_metaData;
    qDebug()<<"tamanio de la MetaDataTable"<<sizeof(MetaDataTable);
    this->header.size_metaData=this->getBlockCant(50*sizeof(MetaDataTable),this->SIZE_BLOCK)*this->SIZE_BLOCK;
    qDebug()<<"Size metadata"<<this->header.size_metaData;
    this->header.rellenoMetadata=this->header.size_metaData-(50*sizeof(MetaDataTable));
    qDebug()<<"relleno MetaData"<<this->header.rellenoMetadata;
    this->header.all_Header_size=sizeof(Header)+this->header.sizeBitsMap+this->header.size_metaData;
    archivo.write(reinterpret_cast<char*>(&this->header),sizeof(Header));
    QByteArray array(this->header.sizeBitsMap,'\0');
    archivo.write(array);
    for(int i=0;i<50;i++){
        MetaDataTable metadat;
        archivo.write(reinterpret_cast<char *>(&metadat),sizeof(MetaDataTable));
    }    
    this->rellenar(this->header.rellenoMetadata,archivo);
    qDebug()<<"xD"<<this->header.all_Header_size;
    this->rellenar(this->header.countBlocksBitsMap*1024,archivo);
    archivo.close();
    return 0;
}

QString MyDB::getNameWithoutExtention(QString path){
    QStringList temp=path.split("/");
    return temp.at(temp.length()-1);
}
bool MyDB::openDB(QString path){
    this->FileOpened.setFileName(path);
    if(!this->FileOpened.open(QIODevice::ReadWrite)){
        return false;
    }
    qDebug()<<this->header.name;
    qDebug()<<this->header.tamanio;
    qDebug()<<this->header.authors;
    this->FileOpened.read(reinterpret_cast<char*>(&this->header),sizeof(Header));
    QByteArray bytes=this->FileOpened.read(this->header.sizeBitsMap);
    qDebug()<<bytes.count();
    this->bitsmap.setBitArray(this->bitsmap.convertByteToBit(bytes));
    for(int i=0;i<50;i++){
        MetaDataTable temps;
        this->FileOpened.read(reinterpret_cast<char*>(&temps),sizeof(MetaDataTable));
        this->metaDataTable.push_back(temps);
    }
    qDebug()<<this->header.all_Header_size<<"xd";
    this->FileOpened.seek(this->header.all_Header_size);
    return true;
}
int MyDB::getBlockCant(int totalBytes, int divisorBytes){
    int temp=totalBytes/divisorBytes;
    if(totalBytes%divisorBytes==0){
        return temp;
    }else{
        return (temp+1);
    }
}

void MyDB::rellenar(int val, QFile &file){
    int lim=val/this->SIZE_BLOCK;
    int dif;
    for(int i=0;i<lim;i++){
        char temp[this->SIZE_BLOCK];
        memset(temp,0,this->SIZE_BLOCK);
        file.write(temp);
    }
    if(val%this->SIZE_BLOCK!=0){
        dif=val-(this->SIZE_BLOCK*lim);
        char temp[dif];
        file.write(temp);
    }
}
int MyDB::getByteSize(int BlocksCant){
    return BlocksCant*this->SIZE_BLOCK;
}
int MyDB::crearTable(QString name, QString descrip, QString fecha){
    int pos=-1;
    for(int i=0;i<this->metaDataTable.size();i++){
        if(metaDataTable.at(i).free)
            pos=i;
    }
    if(pos==-1)
        return 1;
    int emptyPos=this->bitsmap.getBlockEmpty();
    if(emptyPos==-1)
        return 2;
    this->metaDataTable.value(pos).setName(const_cast<char*>(name.toStdString().c_str()));
    this->metaDataTable.value(pos).setDescrip(const_cast<char*>(descrip.toStdString().c_str()));
    this->metaDataTable.value(pos).setFecha(const_cast<char*>(fecha.toStdString().c_str()));
    this->metaDataTable.value(pos).free=false;
    this->metaDataTable.value(pos).pointerToFields=emptyPos;
    return 0;
}

