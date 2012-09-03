#ifndef TABLE_H
#define TABLE_H
#include <table_pointers.h>
#include <table_fields.h>
#include <bitsmap.h>

class Table
{
public:
    Table(int num,int pointer,int key,int second,int cant);
    void setPointers(Table_Pointers pointData,Table_Pointers pointArb,Table_Pointers pointIS, Table_Pointers pointInvert);
    void setFields(Table_Fields fields);
    void setBitsMap(BitsMap &bits);
private:
    Table_Fields campos;
    Table_Pointers pointersData;
    Table_Pointers pointerArbolB;
    Table_Pointers pointerIS;
    Table_Pointers pointerInvertedList;
    int pointerToFields;
    int key;
    int secondary;
    int cant_camp;
    int num_table;
    BitsMap bitsmap;
};

#endif // TABLE_H
