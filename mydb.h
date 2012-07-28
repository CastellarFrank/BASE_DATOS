#ifndef MYDB_H
#define MYDB_H
#include <header.h>
#include <string.h>
#include <stdio.h>

class MyDB
{
public:
    MyDB(char* name,int tamanio);
    Header header;
    void setVersion(char * ver);
    void setDate(char *date);
    void setModifyDate(char *date);
    void setStart_MD(int startpoint);
    void setSize_MD(int size);
    void setAuthors(char *authors);

};

#endif // MYDB_H
