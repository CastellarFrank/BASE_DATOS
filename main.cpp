#include <QtGui/QApplication>
#include "mainwindow.h"
#include <mydb.h>
#include <QFileDialog>
#include <QDebug>
#include <QList>
#include <QString>
#include <stdlib.h>
#include <time.h>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    MyDB baseDatos;
    QString path=QFileDialog::getSaveFileName(0,"Guardar Base de Datos");
    //CREANDO BASE DE DATOS
    baseDatos.createDB(100,path);

    path=QFileDialog::getOpenFileName(0,"Abrir Base de Datos");
    //ABRIENDO BASE DE DATOS
    baseDatos.openDB(path);

    qDebug()<<"#CREANDO TABLAS#";
    Table_Fields field;
    field.addCampo("Cuenta",'i',4,'f');
    field.addCampo("Nombre",'s',25,'f');
    field.addCampo("Apellido",'s',30,'f');
    field.addCampo("Sexo",'c',1,'f');
    baseDatos.newTable("Tabla Prueba 1","Descripción Tabla prueba1","11/22/33",field,0,1);

    Table_Fields field2;    
    field2.addCampo("Nombre",'s',25,'t');
    field2.addCampo("Cuenta",'i',4,'f');
    field2.addCampo("Sexo",'c',1,'f');
    field2.addCampo("Apellido",'s',30,'t');
    baseDatos.newTable("Table Prueba 2","Descripción Tabla prueba2","22/44/66",field2,1,2);

    Table_Fields field3;
    field3.addCampo("2Cuenta",'S',20,'f');
    field3.addCampo("2Cuenta2",'l',25,'t');
    field3.addCampo("2Cuenta3",'m',30,'t');
    field3.addCampo("2Cuenta4",'p',35,'f');
    baseDatos.newTable("Table Prueba 3","Descripción Tabla prueba3","33/66/99",field3,2,3);

    //CERRANDO BASE DE DATOS
    baseDatos.closeDB();

    //ABRIENDO DE NUEVO LA BASE DE DATOS
    baseDatos.openDB(path);

    //ABRIENDO LA TABLA NUMERO 1 (LA PRIMERA)
    baseDatos.tables_control.openTable(0);

    qDebug()<<"#CREANDO 10000 REGISTROS E INGRESANDOLOS EN MEMORIA PARA LA TABLA 1#";
    for(int i=0;i<10000;i++){
        QList<QString> lista;
        QString temp;
        QString convert;
        temp="100"+convert.setNum(i+1);
        lista.push_back(temp);
        temp="Devon"+convert.setNum(i+1);
        lista.push_back(temp);
        temp="Rubbo"+convert.setNum(i+1);
        lista.push_back(temp);
        char lolo=(i%100);
        temp=QString(QChar(lolo));
        lista.push_back(temp);
        baseDatos.tables_control.tableOpened->addRegister(lista);
    }

    qDebug()<<"#GUARDANDO LOS REGISTROS DE LA MEMORIA AL ARCHIVO#";
    baseDatos.tables_control.tableOpened->addAllRegistersToFile();

    //CERRANDO TABLA
    baseDatos.tables_control.closeTable();


    //ABRIENDO LA TABLA #2
    baseDatos.tables_control.openTable(1);

    qDebug()<<"#CREANDO 10000 REGISTROS E INGRESANDOLOS EN MEMORIA PARA LA TABLA 2#";
    for(int i=0;i<10000;i++){
        QList<QString> lista;
        QString temp;
        QString convert;
        temp="Grecia"+convert.setNum(i+1);
        lista.push_back(temp);
        temp="100"+convert.setNum(i+1);
        lista.push_back(temp);
        char lolo=(i%100);
        temp=QString(QChar(lolo));
        lista.push_back(temp);
        temp="Larrama"+convert.setNum(i+1);
        lista.push_back(temp);
        baseDatos.tables_control.tableOpened->addRegister(lista);
    }
    qDebug()<<"#GUARDANDO LOS REGISTROS DE LA MEMORIA AL ARCHIVO#";
    baseDatos.tables_control.tableOpened->addAllRegistersToFile();

    //CERRANDO TABLA
    baseDatos.tables_control.closeTable();

    //CERRANDO BASE DE DATOS
    baseDatos.closeDB();

    //ABRIENDO BASE DE DATOS
    baseDatos.openDB(path);

    //ABRIENDO TABLA #1
    baseDatos.tables_control.openTable(0);

    qDebug()<<"#MOSTRANDO 100 REGISTROS AL AZAR TABLA N°1#";
    srand(time(NULL));
    for(int i=0;i<100;i++){
        int azar=(rand()%10000);
        qDebug()<<"Mostrando Registro #"<<azar;
        QStringList tempList=baseDatos.tables_control.tableOpened->getRegister(azar);
        for(int e=0;e<tempList.count();e++){
            qDebug()<<"\t"<<tempList.at(e);
        }
    }

    //CERRANDO TABLA #1
    baseDatos.tables_control.closeTable();

    //ABRIENDO TABLA #2
    baseDatos.tables_control.openTable(1);

    qDebug()<<"#MOSTRANDO 100 REGISTROS AL AZAR TABLA N°2#";
    for(int i=0;i<100;i++){
        int azar=(rand()%10000);
        qDebug()<<"Mostrando Registro #"<<azar;
        QStringList tempList=baseDatos.tables_control.tableOpened->getRegister(azar);
        for(int e=0;e<tempList.count();e++){
            qDebug()<<"\t"<<tempList.at(e);
        }
    }
    //CERRANDO TABLA
    baseDatos.tables_control.closeTable();

    //CERRANDO BASE DE DATOS
    baseDatos.closeDB();

    return 0;
}
