#include <QtGui/QApplication>
#include "mainwindow.h"
#include <QDebug>

int prueba(int &num){
    return num++;
}
int searchValueIntoAnArray(int Arreglo[], int elemento, int size,int &pos){
    int centro=0,inferior=0,superior=size-1;
    while(inferior <= superior){
        centro=(superior + inferior)/2;
        qDebug()<<centro;
        if(Arreglo[centro]==elemento){
            pos=centro;
            return 0;
        }else if (Arreglo[centro] > elemento)
            superior = centro-1;
        else
            inferior = centro+1;
        qDebug()<<superior<<inferior;
    }
    pos=inferior;

    return -1;
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    int array[1000];
    array[0]=10043;
    array[1]=10048;
    array[2]=10058;
    array[3]=10068;
    int pos;
    qDebug()<<"RESULT"<<searchValueIntoAnArray(array,10058,4,pos);
    qDebug()<<"POS"<<pos;
    return 0;
}
