#ifndef MYDB_H
#define MYDB_H
#include <header.h>
#include <string.h>
#include <stdio.h>
#include <bitsmap.h>
#include <QString>
#include <QFile>
#include <QDebug>
#include <cstdlib>
#include <QStringList>

using namespace std;

class MyDB
{
public:
    static const int SIZE_BLOCK=1024;
    static const int SIZE_HEADER=sizeof(Header);

    int SIZE_BITSMAP;

    MyDB(int tamanio);
    Header header;
    BitsMap bitsmap;
    void setName(const char *nam);
    void setVersion(const char * ver);
    void setDate(const char *d);
    void setModifyDate(const char *d);
    void setStart_MD(int startpoint);
    void setSize_MD(int size);
    void setAuthors(const char *authors);
    int creatDB(QString path);

private:
    QString getNameWithoutExtention(QString path);


};

#endif // MYDB_H
