#ifndef TABLE_CONTROL_H
#define TABLE_CONTROL_H
#include <QList>
#include <metadatatable.h>
#include <table_fields.h>
#include <QMap>
#include <QString>
#include <QFile>
#include <table.h>
#include <header.h>
#include <bitsmap.h>
#include <QDataStream>


class Table_Control
{
public:
    QList<MetaDataTable>metaData;
    QList<Table_Fields> loadedFields;
    Table_Control();
    void clearAll();
    int crearTable(QString name, QString descrip, QString fecha,Table_Fields Field,int key,int second);
    void saveTablesInfo();
    void openTable(int num);
    void closeTable();
    void setBitsMap(QBitArray *bits);
    void setFile(QFile *f);
    void setHeader(Header &h);
    Table *tableOpened;

private:
    QMap<int,Table_Fields> campos;
    BitsMap bitsmap;
    QFile *fileOpened;
    Header header;
    void createFirstDirectsData(int pos);
    void writeMetaDataTable(int num);
};

#endif // TABLE_CONTROL_H
