#ifndef TABLE_CONTROL_H
#define TABLE_CONTROL_H
#include <QList>
#include <metadatatable.h>
#include <table_fields.h>
#include <QMap>
#include <QString>
#include <QFile>


class Table_Control
{
public:
    Table_Control();
    QList<MetaDataTable>metaData;
    QMap<int,Table_Fields> campos;
    void clearAll();
    int crearTable(QString name, QString descrip, QString fecha,Table_Fields Field,int key,int second, int blockEmpty);
    void saveTablesInfo(QFile &archivo,int sizeBlock,int HeadSize);
};

#endif // TABLE_CONTROL_H
