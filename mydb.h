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
#include <metadatatable.h>

using namespace std;

class MyDB
{
public:
    static const int SIZE_BLOCK=1024;
    MyDB();
    int createDB(int tamanio,QString path);
    bool openDB(QString path);

private:
    QString getNameWithoutExtention(QString path);
    Header header;
    BitsMap bitsmap;
    int getBlockCant(int totalBytes,int divisorBytes);
    QList<MetaDataTable> metaDataTable;
    QFile FileOpened;
    void rellenar(int val,QFile &file);
    int getByteSize(int BlocksCant);
    bool crearTable(QString name,QString descrip,QString fecha);
};

#endif // MYDB_H
