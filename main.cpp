#include <QtGui/QApplication>
#include "mainwindow.h"
#include <mydb.h>
#include <QFileDialog>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    MyDB baseDatos;
    QString path=QFileDialog::getSaveFileName(0,"Lala");
    baseDatos.createDB(100,path);
    path=QFileDialog::getOpenFileName(0,"Lolo");
    baseDatos.openDB(path);

    Table_Fields field;
    field.addCampo("Cuenta",'S',20,'f');
    field.addCampo("Cuenta2",'l',25,'f');
    field.addCampo("Cuenta3",'m',30,'f');
    field.addCampo("Cuenta4",'p',35,'f');
    baseDatos.newTable("Hola","Tabla prueba","11/22/33",field,0,1);
    Table_Fields field2;
    field2.addCampo("1Cuenta",'S',20,'t');
    field2.addCampo("1Cuenta2",'l',25,'f');
    field2.addCampo("1Cuenta3",'m',30,'f');
    field2.addCampo("1Cuenta4",'p',35,'t');
    baseDatos.newTable("1Hola","Tabla prueba2","22/44/66",field2,1,2);
    Table_Fields field3;
    field3.addCampo("2Cuenta",'S',20,'f');
    field3.addCampo("2Cuenta2",'l',25,'t');
    field3.addCampo("2Cuenta3",'m',30,'t');
    field3.addCampo("2Cuenta4",'p',35,'f');
    baseDatos.newTable("2Hola","Tabla prueba3","33/66/99",field3,2,3);
    baseDatos.save();
    baseDatos.openDB(path);
    for(int i=0;i<50;i++){
        qDebug()<<baseDatos.tables_control.metaData.at(i).free<<i;
        if(!baseDatos.tables_control.metaData.at(i).free){
            qDebug()<<">>>>>TABLA INFO<<<<<";
            qDebug()<<"-Nombre:"<<baseDatos.tables_control.metaData.at(i).nombre;
            qDebug()<<"-Descrip: "<<baseDatos.tables_control.metaData.at(i).descrip;
            qDebug()<<"-Fecha: "<<baseDatos.tables_control.metaData.at(i).fecha;
            qDebug()<<"-key: "<<baseDatos.tables_control.metaData.at(i).primaryKey;
            qDebug()<<"-secondary: "<<baseDatos.tables_control.metaData.at(i).secondaryIndex;
            for(int e=0;e<baseDatos.tables_control.metaData.at(i).cant_camp;e++){
                qDebug()<<">>>>>CAMPOS INFO<<<<<";
                qDebug()<<"-Nombre:"<<baseDatos.tables_control.loadedFields.at(i).campos.at(e).name;
                qDebug()<<"-Tipo:"<<baseDatos.tables_control.loadedFields.at(i).campos.at(e).type;
                qDebug()<<"-Size:"<<baseDatos.tables_control.loadedFields.at(i).campos.at(e).size;
                qDebug()<<"-Nulo:"<<baseDatos.tables_control.loadedFields.at(i).campos.at(e).nulo;
            }
        }
    }
    //ESTO ES SOLO UNA PRUEBA
    return 0;
}
