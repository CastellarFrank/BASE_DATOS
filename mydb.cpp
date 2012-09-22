#include "mydb.h"

MyDB::MyDB()
{
}

int MyDB::createDB(int tamanio, QString path){
    qDebug()<<"#CREANDO NUEVA BD#";
    QFile archivo(path);
    if(!archivo.open(QIODevice::ReadWrite))
        return 1;
    this->header.tamanio=tamanio;
    QString temp=this->getNameWithoutExtention(path);
    this->header.setName(const_cast<char*>(temp.toStdString().c_str()));
    this->header.setAuthors("Industrias BuenRecord");
    qDebug()<<"\t#Size Header#"<<sizeof(Header);
    this->header.countBlocksBitsMap=this->getBlockCant(tamanio*1024*1024,this->SIZE_BLOCK);
    qDebug()<<"\t#Cantdad bloques disponibles#"<<this->header.countBlocksBitsMap;
    this->header.sizeBitsMap=this->header.countBlocksBitsMap/8;
    qDebug()<<"\t#Tamaño bitsmap(bytes)#"<<this->header.sizeBitsMap;
    this->header.start_metaData=sizeof(Header)+this->header.sizeBitsMap;
    this->header.size_metaData=this->getBlockCant(50*sizeof(MetaDataTable),this->SIZE_BLOCK)*this->SIZE_BLOCK;
    qDebug()<<"\t#Tamaño metadata (bytes)#"<<this->header.size_metaData;
    this->header.rellenoMetadata=this->header.size_metaData-(50*sizeof(MetaDataTable));
    this->header.all_Header_size=sizeof(Header)+this->header.sizeBitsMap+this->header.size_metaData;
    archivo.write(reinterpret_cast<char*>(&this->header),sizeof(Header));
    QByteArray array(this->header.sizeBitsMap,'\0');    
    archivo.write(array);
    for(int i=0;i<50;i++){
        MetaDataTable metadat;
        archivo.write(reinterpret_cast<char *>(&metadat),sizeof(MetaDataTable));
    }    
    this->rellenar(this->header.rellenoMetadata,archivo);
    qDebug()<<"\t#Tamaño total de los encabezados#"<<this->header.all_Header_size;
    qDebug()<<"\t#Rellenando el archivo#";
    this->rellenar(this->header.countBlocksBitsMap*1024,archivo);
    archivo.close();
    return 0;
}

QString MyDB::getNameWithoutExtention(QString path){
    QStringList temp=path.split("/");
    return temp.at(temp.length()-1);
}
bool MyDB::openDB(QString path){
    qDebug()<<"#OPEN BD#";
    this->FileOpened.setFileName(path);
    if(!this->FileOpened.open(QIODevice::ReadWrite)){
        return false;
    }
    qDebug()<<"\t#Name#"<<this->header.name;
    qDebug()<<"\t#Tamaño#"<<this->header.tamanio;
    qDebug()<<"\t#Autores#"<<this->header.authors;
    this->FileOpened.read(reinterpret_cast<char*>(&this->header),sizeof(Header));
    QByteArray bytes=this->FileOpened.read(this->header.sizeBitsMap);
    this->bitsmap.setBitArray((this->bitsmap.convertByteToBit(bytes)));
    qDebug()<<"\t#Leyendo bitsmap#";
    this->tables_control.clearAll();
    qDebug()<<"\t#Cargando metada y campos#";
    for(int i=0;i<50;i++){
        MetaDataTable temps;
        this->FileOpened.read(reinterpret_cast<char*>(&temps),sizeof(MetaDataTable));
        int postemp=this->FileOpened.pos();
        Table_Fields fieldTemp;
        for(int e=0;e<temps.cant_camp;e++){
            Field ftemp;
            this->FileOpened.seek(this->header.all_Header_size+temps.pointerToFields*this->SIZE_BLOCK+(e*sizeof(Field)));
            this->FileOpened.read(reinterpret_cast<char*>(&ftemp),sizeof(Field));
            fieldTemp.campos.push_back(ftemp);
        }
        this->tables_control.metaData.push_back(temps);
        this->tables_control.loadedFields.push_back(fieldTemp);
        this->FileOpened.seek(postemp);
    }
    qDebug()<<"\t#Asignando info a otras clases#";
    this->bitsmap.setFile(&this->FileOpened);
    this->tables_control.setBitsMap(this->bitsmap.bits);
    this->tables_control.setFile(&this->FileOpened);
    this->tables_control.setHeader(this->header);
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
    qDebug()<<"\t#GUARDANDO INFO#";
    this->writeHeader();
    this->bitsmap.writeBitsMap();
    this->tables_control.saveTablesInfo();
}
void MyDB::writeHeader(){
    this->FileOpened.seek(0);
    this->FileOpened.write(reinterpret_cast<char*>(&this->header),sizeof(Header));
}
void MyDB::closeDB(){
    qDebug()<<"#CERRANDO BASE DE DATOS#";
    this->save();
    this->FileOpened.close();
}


