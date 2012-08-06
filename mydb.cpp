#include "mydb.h"

MyDB::MyDB()
{
    qDebug()<<sizeof(MetaDataTable);
}

int MyDB::createDB(int tamanio, QString path){
    QFile archivo(path);
    if(!archivo.open(QIODevice::ReadWrite))
        return 1;
    this->header.tamanio=tamanio;
    QString temp=this->getNameWithoutExtention(path);
    this->header.setName(const_cast<char*>(temp.toStdString().c_str()));
    this->header.setAuthors("Industrias BuenRecord");
    archivo.write(reinterpret_cast<char*>(&this->header),sizeof(Header));
    int cantCasillas=this->getBlockCant(tamanio*1024*1024,this->SIZE_BLOCK);
    QByteArray array(cantCasillas/8,'\0');
    archivo.write(array);
    for(int i=0;i<50;i++){
        MetaDataTable metadat;
        archivo.write(reinterpret_cast<char *>(&metadat),sizeof(MetaDataTable));
    }    
    this->HEADER_SIZE=archivo.size();
    qDebug()<<HEADER_SIZE;
    archivo.close();
    return 0;
}

QString MyDB::getNameWithoutExtention(QString path){
    QStringList temp=path.split("/");
    return temp.at(temp.length()-1);
}
bool MyDB::openDB(QString path){
    this->FileOpened.setFileName(path);;
    if(!this->FileOpened.open(QIODevice::ReadWrite)){
        return false;
    }
    qDebug()<<this->header.name;
    qDebug()<<this->header.tamanio;
    qDebug()<<this->header.authors;
    this->FileOpened.read(reinterpret_cast<char*>(&this->header),sizeof(Header));
    QByteArray bytes=this->FileOpened.read(this->header.tamanio*1024*1024/this->SIZE_BLOCK/8);
    qDebug()<<bytes.count();
    for(int i=0;i<50;i++){
        MetaDataTable temps;
        this->FileOpened.read(reinterpret_cast<char*>(&temps),sizeof(MetaDataTable));
        this->metaDataTable.push_back(temps);
    }
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

