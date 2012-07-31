#include "header.h"

Header::Header()
{
    memset(this->name,0,208);
    memset(this->version,0,104);
    memset(this->authors,0,208);
    memset(this->date,0,12);
    memset(this->modify,0,12);
    memset(this->padding,0,468);
    this->tamanio=0;
    this->start_metaData=0;
    this->size_metaData=0;
}

void Header::setName(char *n){
    strcpy(this->name,n);
}

void Header::setVersion(char *v){
    strcpy(this->version,v);
}

void Header::setAuthors(char *a){
    strcpy(this->authors,a);
}

void Header::setDate(char *d){
    strcpy(this->date,d);
}

void Header::setModify(char *m){
    strcpy(this->modify,m);
}
