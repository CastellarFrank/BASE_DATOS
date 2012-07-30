#ifndef BITSMAP_H
#define BITSMAP_H
#include <QBitArray>
#include <QByteArray>
class BitsMap
{
public:
    BitsMap(); //Constructor vacio, no hace nada
    QByteArray convertBitToByte(QBitArray array);
    QBitArray convertByteToBit(QByteArray array);
    QByteArray convertLocalBitToByte();
    QBitArray convertLocalByteToBit();
    void setBitArray(QBitArray &array);
    void setByteArray(QByteArray &array);
private:
    QBitArray bits;
    QByteArray bytes;
    bool activoByte;
    bool activoBit;

};

#endif // BITSMAP_H
