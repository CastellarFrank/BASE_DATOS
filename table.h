#ifndef TABLE_H
#define TABLE_H
#include <table_pointers.h>
#include <table_fields.h>
#include <bitsmap.h>
#include <metadatatable.h>
#include <QDataStream>
#include <header.h>

class Table
{
public:
    Table(int num,MetaDataTable meta,Table_Fields cam);
    void setBitsMap(BitsMap &bits);
    void setFile(QFile *file);
    void setHeader(Header &head);
    int addRegister(QByteArray info);
    int validateAndCreateDataSpace(int val);
private:
    Table_Fields listFields;
    MetaDataTable metaData;
    int num_table;
    int registerCant;
    BitsMap bitsmap;
    QFile *fileOpened;
    Header header;
};

#endif // TABLE_H
