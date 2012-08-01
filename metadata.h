#ifndef METADATA_H
#define METADATA_H

class MetaData
{
public:
    MetaData(char * nombre,char type, int primaryKey);//Hola a todos como estan
    char nombre[40];
    char type;
    char free;
    int primaryKey;
    int firstIndex;
    int secondaryIndex;
    int thirthIndex;
};

#endif // METADATA_H
