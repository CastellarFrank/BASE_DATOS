#include "mydb.h"

MyDB::MyDB(char *name, int tamanio)
{
    this->header.tamanio=tamanio;
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
