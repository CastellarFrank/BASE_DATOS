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
    this->header.setDate("23/33/89");
    this->header.setModify("90/65/12");
    archivo.write(reinterpret_cast<char*>(&this->header),sizeof(Header));
    QByteArray array(this->getBlockCant(tamanio*1024*1024,this->SIZE_BLOCK)/8,'\0');
    archivo.write(array);

    archivo.close();
    return 0;
}

QString MyDB::getNameWithoutExtention(QString path){
    QStringList temp=path.split("/");
    return temp.at(temp.length()-1);
}
bool MyDB::openDB(QString path){
    QFile archivo(path);
    if(!archivo.open(QIODevice::ReadOnly)){
        return false;
    }
}
int MyDB::getBlockCant(int totalBytes, int divisorBytes){
    int temp=totalBytes/divisorBytes;
    if(totalBytes%divisorBytes==0){
        return temp;
    }else{
        return (temp+1);
    }
}

