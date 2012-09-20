#include <QtGui/QApplication>
#include "mainwindow.h"
#include <mydb.h>
#include <QFileDialog>
#include <QDebug>
#include <QList>
#include <QString>

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
    field.addCampo("Cuenta",'i',4,'f');
    field.addCampo("Nombre",'s',25,'f');
    field.addCampo("Apellido",'s',30,'f');
    field.addCampo("Sexo",'c',1,'f');
    baseDatos.newTable("JUMM","Tabla prueba","11/22/33",field,0,1);
    Table_Fields field2;
    field2.addCampo("1Cuenta",'S',20,'t');
    field2.addCampo("1Cuenta2",'l',25,'f');
    field2.addCampo("1Cuenta3",'m',30,'f');
    field2.addCampo("1Cuenta4",'p',35,'t');
    baseDatos.newTable("JUMMM1","Tabla prueba2","22/44/66",field2,1,2);
    Table_Fields field3;
    field3.addCampo("2Cuenta",'S',20,'f');
    field3.addCampo("2Cuenta2",'l',25,'t');
    field3.addCampo("2Cuenta3",'m',30,'t');
    field3.addCampo("2Cuenta4",'p',35,'f');
    baseDatos.newTable("2Hola","Tabla prueba3","33/66/99",field3,2,3);

    baseDatos.save();
    baseDatos.openDB(path);
    baseDatos.tables_control.openTable(0);

    qDebug()<<"LLEGO HATA ACA";
    for(int i=0;i<16480/2;i++){
        qDebug()<<"FOR ENTRADA"<<i;
        QList<QString> lista;
        QString temp;
        QString convert;
        temp="100"+convert.setNum(i+1);
        lista.push_back(temp);
        temp="Devon"+convert.setNum(i+1);
        lista.push_back(temp);
        temp="Rubbo"+convert.setNum(i+1);
        lista.push_back(temp);
        char lolo=i+65;
        temp=QString(QChar(lolo));
        lista.push_back(temp);
        baseDatos.tables_control.tableOpened->addRegister(lista);
    }
    qDebug()<<"SALIO DE ACA";
    for(int i=0;i<20;i++){
        QByteArray array=baseDatos.tables_control.tableOpened->StackRegisters[i];
        int first;
        memcpy(&first,array,sizeof(int));
        array.remove(0,4);
        QByteArray second=array.left(25);
        array.remove(0,25);
        QByteArray thirth=array.left(30);
        array.remove(0,30);
        char c;
        memcpy(&c,array,1);
        qDebug()<<"Cuenta"<<first;
        qDebug()<<"Nombre"<<second;
        qDebug()<<"Apellido"<<thirth;
        qDebug()<<"Sexo"<<c;
    }
    baseDatos.tables_control.tableOpened->addAllRegistersToFile();
    qDebug()<<"SALVADA PRIMERA PARTE";
    baseDatos.tables_control.closeTable();
    baseDatos.save();

    qDebug()<<"GRAN MITAD";
    baseDatos.openDB(path);
    baseDatos.tables_control.openTable(0);

    for(int i=0;i<16480/2;i++){
        qDebug()<<"FOR ENTRADA"<<i;
        QList<QString> lista;
        QString temp;
        QString convert;
        temp="100"+convert.setNum(i+1);
        lista.push_back(temp);
        temp="Devon"+convert.setNum(i+1);
        lista.push_back(temp);
        temp="Rubbo"+convert.setNum(i+1);
        lista.push_back(temp);
        char lolo=i+65;
        temp=QString(QChar(lolo));
        lista.push_back(temp);
        baseDatos.tables_control.tableOpened->addRegister(lista);
    }
    baseDatos.tables_control.tableOpened->addAllRegistersToFile();
    qDebug()<<"SALVADA SEGUNDA PARTE";
    baseDatos.tables_control.closeTable();
    baseDatos.save();

//    baseDatos.openDB(path);
//    for(int i=0;i<50;i++){
//        qDebug()<<baseDatos.tables_control.metaData.at(i).free<<i;
//        if(!baseDatos.tables_control.metaData.at(i).free){
//            qDebug()<<">>>>>TABLA INFO<<<<<";
//            qDebug()<<"-Nombre:"<<baseDatos.tables_control.metaData.at(i).nombre;
//            qDebug()<<"-Descrip: "<<baseDatos.tables_control.metaData.at(i).descrip;
//            qDebug()<<"-Fecha: "<<baseDatos.tables_control.metaData.at(i).fecha;
//            qDebug()<<"-key: "<<baseDatos.tables_control.metaData.at(i).primaryKey;
//            qDebug()<<"-secondary: "<<baseDatos.tables_control.metaData.at(i).secondaryIndex;
//            for(int e=0;e<baseDatos.tables_control.metaData.at(i).cant_camp;e++){
//                qDebug()<<">>>>>CAMPOS INFO<<<<<";
//                qDebug()<<"\t-Nombre:"<<baseDatos.tables_control.loadedFields.at(i).campos.at(e).name;
//                qDebug()<<"\t-Tipo:"<<baseDatos.tables_control.loadedFields.at(i).campos.at(e).type;
//                qDebug()<<"\t-Size:"<<baseDatos.tables_control.loadedFields.at(i).campos.at(e).size;
//                qDebug()<<"\t-Nulo:"<<baseDatos.tables_control.loadedFields.at(i).campos.at(e).nulo;
//            }
//        }
//    }

    return 0;
}
