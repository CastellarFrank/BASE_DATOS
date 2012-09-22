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
#include <table_control.h>

using namespace std;

class MyDB
{
public:
    static const int SIZE_BLOCK=1024;

    MyDB();
    int createDB(int tamanio,QString path);
    bool openDB(QString path);
    Table_Control tables_control;
    int newTable(QString name, QString descrip, QString fecha, Table_Fields Field, int key, int second);
    BitsMap bitsmap;
    void closeDB();

private:
    QString getNameWithoutExtention(QString path);
    Header header;
    int getBlockCant(int totalBytes,int divisorBytes);
    QFile FileOpened;
    void rellenar(int val,QFile &file);
    int getByteSize(int BlocksCant);
    void writeHeader();
    void save();
};

#endif // MYDB_H
