#include "mydb.h"

MyDB::MyDB(int tamanio)
{
    this->header.tamanio=tamanio;
}
void MyDB::setName(const char *nam){
    strcpy(this->header.name,nam);
}

void MyDB::setVersion(const char *ver){
    strcpy(this->header.version,ver);
}

void MyDB::setDate(const char *d){
    strcpy(this->header.date,d);
}

void MyDB::setAuthors(const char *authors){
    strcpy(this->header.authors,authors);
}

void MyDB::setModifyDate(const char *d){
    strcpy(this->header.modify,d);
}

void MyDB::setStart_MD(int startpoint){
    this->header.start_metaData=startpoint;
}

void MyDB::setSize_MD(int size){
    this->header.size_metaData=size;
}
int MyDB::creatDB(QString path){

    QFile *archivo=new QFile(path+".brdb");
    if(!archivo->open(QIODevice::ReadWrite))
        return 1;
    QString temp=this->getNameWithoutExtention(path);

    this->setName(temp.toStdString().c_str());
    this->setVersion("1.1");
    this->setAuthors("Frank");
    this->setDate("06/89/23");
    this->setModifyDate("23/23/12");
    this->setStart_MD(20);
    this->setSize_MD(100);
    archivo->write(reinterpret_cast<char*>(&this->header),sizeof(Header));

    char *veloz;

    qDebug()<<this->header.name;
    qDebug()<<this->header.tamanio;
    qDebug()<<this->header.modify;
    qDebug()<<this->header.date;
    qDebug()<<sizeof(Header);
    archivo->read(veloz,sizeof(Header));
    qDebug()<<"\n\n";
    qDebug()<<this->header.name;
    qDebug()<<this->header.tamanio;
    qDebug()<<this->header.modify;
    qDebug()<<this->header.date;
    Header valiz;
    valiz=reinterpret_cast<Header&>(veloz);
    qDebug()<<valiz.name;
    qDebug()<<valiz.tamanio;
    qDebug()<<valiz.modify;
    qDebug()<<valiz.date;
    archivo->close();
    return 0;
}

QString MyDB::getNameWithoutExtention(QString path){
    QStringList temp=path.split("/");
    return temp.at(temp.length()-1);
}

