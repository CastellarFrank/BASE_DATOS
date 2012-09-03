#ifndef TABLE_CONTROL_H
#define TABLE_CONTROL_H
#include <QList>
#include <metadatatable.h>
#include <table_fields.h>
#include <QMap>
#include <QString>
#include <QFile>
#include <table.h>
#include <bitsmap.h>


class Table_Control
{
public:
    QList<MetaDataTable>metaData;
    QList<Table_Fields> loadedFields;
    Table_Control();
    void clearAll();
    int crearTable(QString name, QString descrip, QString fecha,Table_Fields Field,int key,int second);
    void saveTablesInfo(QFile &archivo,int sizeBlock,int HeadSize,int startMeta);
    void openTable(int num);
    void closeTable();
    void setBitsMap(BitsMap &bits);


private:
    QMap<int,Table_Fields> campos;
    Table *tableOpened;
    BitsMap bitsmap;
};

#endif // TABLE_CONTROL_H
