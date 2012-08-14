#include <QtGui/QApplication>
#include "mainwindow.h"
#include <mydb.h>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    MyDB baseDatos;
    QString path=QFileDialog::getSaveFileName(0,"Lala");
    baseDatos.createDB(200,path);
    //path=QFileDialog::getOpenFileName(0,"Lolo");
    baseDatos.openDB(path);
    return 0;
}
