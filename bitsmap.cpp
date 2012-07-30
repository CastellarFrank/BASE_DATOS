#include "bitsmap.h"

BitsMap::BitsMap()
{
}

QByteArray BitsMap::convertBitToByte(QBitArray array){
    QByteArray temp;
    temp.resize(array.count()/8);
    for(int b=0; b<array.count(); b++)
        temp[b/8] = ( temp.at(b/8) | ((array[b]?1:0)<<(b%8)));
    return temp;
}

QBitArray BitsMap::convertByteToBit(QByteArray array){
    QBitArray temp(array.count()*8);
    for(int i=0; i<array.count(); ++i)
        for(int b=0; b<8; ++b)
            temp.setBit(i*8+b, array.at(i)&(1<<b));
    return temp;
}

QByteArray BitsMap::convertLocalBitToByte(bool &ok){
    if(this->activoBit){
        ok=true;
        return this->convertBitToByte(this->bits);
    }else{
        ok=false;
        return this->bytes;
    }
}

QBitArray BitsMap::convertLocalByteToBit(bool &ok){
    if(this->activoByte){
        ok=true;
        return this->convertByteToBit(this->bytes);
    }else{
        ok=false;
        return this->bits;
    }
}

void BitsMap::setBitArray(QBitArray &array){
    this->bits=array;
    this->activoBit=true;
}
void BitsMap::setByteArray(QByteArray &array){
    this->bytes=array;
    this->activoByte=true;
}
