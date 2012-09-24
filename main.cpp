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
    baseDatos.createDB(200,path);
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

//    //CERRANDO BASE DE DATOS
//    baseDatos.closeDB();

//    //ABRIENDO DE NUEVO LA BASE DE DATOS
//    baseDatos.openDB(path);

    //ABRIENDO LA TABLA NUMERO 1 (LA PRIMERA)
    baseDatos.tables_control.openTable(0);

    qDebug()<<"#CREANDO 100 REGISTROS E INGRESANDOLOS EN MEMORIA PARA LA TABLA 1#";
    for(int i=0;i<1000;i++){
        QList<QString> lista;
        QString temp;
        QString convert;
        temp="100"+convert.setNum(i+1);
        lista.push_back(temp);
        temp="Grecia"+convert.setNum(i+1);
        lista.push_back(temp);
        temp="Larrama"+convert.setNum(i+1);
        lista.push_back(temp);
        char lolo=((i%100)+20);
        temp=QString(QChar(lolo));
        lista.push_back(temp);
        baseDatos.tables_control.tableOpened->addRegister(lista);
    }

    qDebug()<<"#GUARDANDO LOS REGISTROS DE LA MEMORIA AL ARCHIVO#";
    baseDatos.tables_control.tableOpened->addAllRegistersToFile();

//    //CERRANDO TABLA
//    baseDatos.tables_control.closeTable();

//    //CERRANDO BASE DE DATOS
//    baseDatos.closeDB();

//    //ABRIENDO BASE DE DATOS
//    baseDatos.openDB(path);

//    //ABRIENDO TABLA #1
//    baseDatos.tables_control.openTable(0);

    qDebug()<<"#MOSTRANDO 50 REGISTROS AL AZAR TABLA N°1#";
    srand(time(NULL));
    for(int i=0;i<50;i++){
        int azar=(rand()%1000);
        qDebug()<<"Mostrando Registro #"<<azar;
        QStringList tempList=baseDatos.tables_control.tableOpened->getRegister(azar);
        for(int e=0;e<tempList.count();e++){
            qDebug()<<"\t"<<tempList.at(e);
        }
    }

//    //CERRANDO TABLA #1
//    baseDatos.tables_control.closeTable();

//    baseDatos.closeDB();

//    baseDatos.openDB(path);

//    baseDatos.tables_control.openTable(0);

//    srand(time(NULL));
//    qDebug()<<"DUPLICADOS"<<baseDatos.tables_control.tableOpened->duplicates;

//    baseDatos.FileOpened.seek(37888+(2*1024));
//    nodoTree temp;
//    baseDatos.FileOpened.read(reinterpret_cast<char*>(&temp),sizeof(nodoTree));
//    qDebug()<<"2 Cant"<<temp.keyCount<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
//    for(int i=0;i<temp.keyCount;i++){
//        qDebug()<<"FOR"<<i;
//        if(i==0)
//            qDebug()<<temp.hijos[i];
//        qDebug()<<temp.keys[i].value<<temp.keys[i].pointer;
//        qDebug()<<temp.hijos[i+1];
//    }
//    baseDatos.FileOpened.seek(37888+(9*1024));
//    baseDatos.FileOpened.read(reinterpret_cast<char*>(&temp),sizeof(nodoTree));
//    qDebug()<<"9 Cant"<<temp.keyCount<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
//    for(int i=0;i<temp.keyCount;i++){
//        qDebug()<<"FOR"<<i;
//        if(i==0)
//            qDebug()<<temp.hijos[i];
//        qDebug()<<temp.keys[i].value<<temp.keys[i].pointer;
//        qDebug()<<temp.hijos[i+1];

//    }
//    baseDatos.FileOpened.seek(37888+(10*1024));
//    baseDatos.FileOpened.read(reinterpret_cast<char*>(&temp),sizeof(nodoTree));
//    qDebug()<<"10 Cant"<<temp.keyCount<<">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>";
//    for(int i=0;i<temp.keyCount;i++){
//        qDebug()<<"FOR"<<i;
//        if(i==0)
//            qDebug()<<temp.hijos[i];
//        qDebug()<<temp.keys[i].value<<temp.keys[i].pointer;
//        qDebug()<<temp.hijos[i+1];

//    }
    qDebug()<<"#BUSQUEDA USANDO EL ARBOL#";


    QStringList registerTree=baseDatos.tables_control.tableOpened->getRegisterUsingTree(100245);
    qDebug()<<"Registro"<<100245;
    for(int i=0;i<registerTree.count();i++){
        qDebug()<<registerTree.at(i);
    }
    registerTree=baseDatos.tables_control.tableOpened->getRegisterUsingTree(100465);
    qDebug()<<"Registro"<<100465;
    for(int i=0;i<registerTree.count();i++){
        qDebug()<<registerTree.at(i);
    }

    registerTree=baseDatos.tables_control.tableOpened->getRegisterUsingTree(100865);
    qDebug()<<"Registro"<<100865;
    for(int i=0;i<registerTree.count();i++){
        qDebug()<<registerTree.at(i);
    }
    registerTree=baseDatos.tables_control.tableOpened->getRegisterUsingTree(10000);
    qDebug()<<"Registro"<<10000;
    for(int i=0;i<registerTree.count();i++){
        qDebug()<<registerTree.at(i);
    }


    return 0;
}
