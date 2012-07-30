#ifndef BITSMAP_H
#define BITSMAP_H
#include <QBitArray>
#include <QByteArray>
class BitsMap
{
public:
    BitsMap(); //Constructor vacio, no hace nada
    QByteArray convertBitToByte(QBitArray array); //Convierte el QBit de parámetro y lo regresa en un QByte
    QBitArray convertByteToBit(QByteArray array);
    QByteArray convertLocalBitToByte(bool &ok);
    QBitArray convertLocalByteToBit(bool &ok);
    void setBitArray(QBitArray &array);
    void setByteArray(QByteArray &array);
private:
    QBitArray bits;
    QByteArray bytes;
    bool activoByte;
    bool activoBit;

};

#endif // BITSMAP_H
