#include <QtGui/QApplication>
#include "mainwindow.h"
#include <mydb.h>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    //w.show();
    MyDB baseDatos(200);
    QString path=QFileDialog::getSaveFileName(0,"Lala");
    baseDatos.creatDB(path);
    return 0;
}
