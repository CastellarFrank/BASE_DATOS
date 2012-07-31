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
#include <iostream>

using namespace std;

class MyDB
{
public:
    static const int SIZE_BLOCK=1024;
    MyDB(int tamanio);
    Header header;
    BitsMap bitsmap;
    int creatDB(QString path);

private:
    QString getNameWithoutExtention(QString path);
    void mathHomeWork(int tam);
};

#endif // MYDB_H
