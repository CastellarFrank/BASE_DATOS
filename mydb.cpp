#include "mydb.h"

MyDB::MyDB(int tamanio)
{
    this->header.tamanio=tamanio;
    this->SIZE_BITSMAP=this->getSizeBitsMap(tamanio);
}
void MyDB::setName(const char *nam){
    strcpy(this->header.name,nam);
}

void MyDB::setVersion(char *ver){
    strcpy(this->header.version,ver);
}

void MyDB::setDate(char *date){
    strcpy(this->header.date,date);
}

void MyDB::setAuthors(char *authors){
    strcpy(this->header.authors,authors);
}

void MyDB::setModifyDate(char *date){
    strcpy(this->header.modify_date,date);
}

void MyDB::setStart_MD(int startpoint){
    this->header.start_metaData=startpoint;
}

void MyDB::setSize_MD(int size){
    this->header.size_metaData=size;
}
int MyDB::creatDB(QString path){
    QFile *archivo=new QFile(path+".brdb");
    if(!archivo->open(QIODevice::WriteOnly))
        return 1;
    QString temp=this->getNameWithoutExtention(path);
    qDebug()<<temp;
    this->setName(temp.toStdString().c_str());
    QByteArray relleno(this->header.tamanio*1024*1024,'0');
    archivo->write(relleno);
    archivo->seek(0);
    archivo->write(reinterpret_cast<char*>(&this->header),sizeof(Header));
    archivo->close();
    return 0;
}
int MyDB::math_homework(){
    int blockCant=this->header.tamanio/this->SIZE_BLOCK;
    return blockCant;
}

QString MyDB::getNameWithoutExtention(QString path){
    QStringList temp=path.split("/");
    return temp.at(temp.length()-1);
}
int MyDB::getSizeBitsMap(int tamanio){
    return (tamanio*1024*1024*8/this->SIZE_BLOCK);
}
