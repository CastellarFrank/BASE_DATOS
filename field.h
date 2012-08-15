#ifndef FIELD_H
#define FIELD_H
#include <string.h>
#include <stdio.h>


class Field
{
public:
    Field();
    char name[25];
    char type;
    int size;
    char nulo;
    void setName(char* nombre);
};

#endif // FIELD_H
