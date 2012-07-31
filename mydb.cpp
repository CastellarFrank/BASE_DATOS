#include "mydb.h"

MyDB::MyDB(int tamanio)
{
    this->header.tamanio=tamanio;
    this->mathHomeWork(tamanio);

}

int MyDB::creatDB(QString path){

    QFile *archivo=new QFile(path+".brdb");
    if(!archivo->open(QIODevice::ReadWrite))
        return 1;
    QString temp=this->getNameWithoutExtention(path);
    this->header.setName(const_cast<char*>(temp.toStdString().c_str()));
    this->header.setAuthors("Frank");
    this->header.setDate("23/33/89");
    this->header.setModify("90/65/12");
    this->header.setVersion("2.5");
    archivo->write(reinterpret_cast<char*>(&this->header),sizeof(Header));
    archivo->close();

    return 0;
}

QString MyDB::getNameWithoutExtention(QString path){
    QStringList temp=path.split("/");
    return temp.at(temp.length()-1);
}
void MyDB::mathHomeWork(int tam){

}

