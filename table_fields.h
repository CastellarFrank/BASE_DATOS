#ifndef TABLE_FIELDS_H
#define TABLE_FIELDS_H
#include <field.h>
#include <QList>

class Table_Fields
{
public:
    Table_Fields();
    QList<Field> campos;
    int addCampo(char* name,char t,int size, bool);
};

#endif // TABLE_FIELDS_H
