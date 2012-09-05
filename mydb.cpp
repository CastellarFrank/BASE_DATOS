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
    qDebug()<<"size"<<this->header.countBlocksBitsMap;
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
    this->tables_control.clearAll();
    for(int i=0;i<50;i++){
        MetaDataTable temps;
        this->FileOpened.read(reinterpret_cast<char*>(&temps),sizeof(MetaDataTable));
        int postemp=this->FileOpened.pos();
        Table_Fields fieldTemp;
        for(int e=0;e<temps.cant_camp;e++){
            Field ftemp;
            qDebug()<<"Size:"<<this->header.all_Header_size+temps.pointerToFields*this->SIZE_BLOCK+(e*sizeof(Field));
            this->FileOpened.seek(this->header.all_Header_size+temps.pointerToFields*this->SIZE_BLOCK+(e*sizeof(Field)));
            this->FileOpened.read(reinterpret_cast<char*>(&ftemp),sizeof(Field));
            qDebug()<<ftemp.name<<FileOpened.pos()<<i<<e;
            fieldTemp.campos.push_back(ftemp);
        }
        this->tables_control.metaData.push_back(temps);
        this->tables_control.loadedFields.push_back(fieldTemp);
        this->FileOpened.seek(postemp);
    }
    this->tables_control.setBitsMap(this->bitsmap);
    this->tables_control.setFile(&this->FileOpened);
    this->tables_control.setHeader(this->header);
    qDebug()<<this->header.all_Header_size<<"xd";
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
        QByteArray b(this->SIZE_BLOCK,'\0');
        file.write(b);
    }
    if(val%this->SIZE_BLOCK!=0){
        dif=val-(this->SIZE_BLOCK*lim);
        char temp[dif];
        memset(temp,0,dif);
        file.write(reinterpret_cast<char*>(&temp),dif);
    }
}
int MyDB::getByteSize(int BlocksCant){
    return BlocksCant*this->SIZE_BLOCK;
}

int MyDB::newTable(QString name, QString descrip, QString fecha, Table_Fields Field, int key, int second){

    return this->tables_control.crearTable(name,descrip,fecha,Field,key,second);
}

void MyDB::save(){
    this->writeHeader();
    this->bitsmap.writeBitsMap(this->FileOpened);
    this->tables_control.saveTablesInfo();
    this->FileOpened.close();
}
void MyDB::writeHeader(){
    this->FileOpened.seek(0);
    this->FileOpened.write(reinterpret_cast<char*>(&this->header),sizeof(Header));
}


