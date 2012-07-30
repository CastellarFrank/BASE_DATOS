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
    void setVersion(char * ver);
    void setDate(char *date);
    void setModifyDate(char *date);
    void setStart_MD(int startpoint);
    void setSize_MD(int size);
    void setAuthors(char *authors);
    int creatDB(QString path);

private:
    int math_homework();
    QString getNameWithoutExtention(QString path);
    int getSizeBitsMap(int tamanio);


};

#endif // MYDB_H
