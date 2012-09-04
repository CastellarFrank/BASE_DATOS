#ifndef TABLE_H
#define TABLE_H
#include <table_pointers.h>
#include <table_fields.h>
#include <bitsmap.h>
#include <metadatatable.h>

class Table
{
public:
    Table(int num,MetaDataTable meta,Table_Fields cam);
    void setBitsMap(BitsMap &bits);
private:
    Table_Fields campos;
    MetaDataTable metaData;
    int num_table;
    int registerCant;
    BitsMap bitsmap;
};

#endif // TABLE_H
