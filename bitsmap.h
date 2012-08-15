#ifndef BITSMAP_H
#define BITSMAP_H
#include <QBitArray>
#include <QByteArray>
#include <QDebug>
class BitsMap
{
public:
    BitsMap(); //Constructor vacio, no hace nada
    QByteArray convertBitToByte(QBitArray array); //Convierte el QBit de parámetro y lo regresa en un QByte
    QBitArray convertByteToBit(QByteArray array); //Convierte el QByte de parámetro y lo regresa en un QBit
    QByteArray convertLocalBitToByte(bool &ok);  //Convierte el QBit local y lo regresa en QByte, el boolean es para
    //saber si el procedimiento fue exitoso.

    QBitArray convertLocalByteToBit(bool &ok); //Convierte el QByte local y lo regresa en QBit, el boolean es para
    //saber si el procedimiento fue exitoso.

    QBitArray getLocalBit(); //Regresa el QBit cargado.
    QByteArray getLocalByte(); //Regresa el QByte Cargado.
    void setBitArray(QBitArray array); //Define cuál será el QBit a modificar
    void setByteArray(QByteArray array); //Define cuál será el QByte a modificar.
    void assignBitArray(QBitArray &array);//Define el QBit a modificar sin crear copia
    void assignByteArray(QByteArray &array);//Define el QByte a modificar sin crear copia
    void resizeBitsArray(int size);//Redimensiona o inicializa por así decirlo el BitArray interno
    void resizeBytesArray(int size);//Redimensiona o inicializa por asi ecirlo el ByteArray interno
    int getBlockEmpty(); //Retorna una posición libre del bitsmap o sino hay retorna -1
private:
    QByteArray bytes; //Arreglo QByte interno.
    QBitArray bits; //Arreglo QBit interno
    bool activoByte; //Boolean para saber si se ha ingresado un QByte.
    bool activoBit; //Boolean para saber si se ha ingresado un QBit.

};

#endif // BITSMAP_H
