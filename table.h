#ifndef TABLE_H
#define TABLE_H
#include <table_pointers.h>
#include <table_fields.h>
#include <bitsmap.h>
#include <metadatatable.h>
#include <QDataStream>
#include <header.h>
#include <QStringList>
#include <nodotree.h>

class Table
{
public:
    Table(int num,MetaDataTable *meta,Table_Fields cam);
    void setBitsMap(QBitArray *bits);
    void setFile(QFile *file);
    void setHeader(Header &head);
    int addRegister(QList<QString>Regis);
    int canAddRegister();
    QByteArray joinToMe(QList<QString>list);
    int num_table;
    QList<QByteArray> StackRegisters;
    int addAllRegistersToFile();
    QStringList getRegister(int num);
    QStringList getRegisterUsingTree(int key);
    int createNodoTree(keyTree key,int left,int right);
    //int createNewNodoTree();
    nodoTree getNodoTree(int num);
    void writeNodoTree(int num,nodoTree nodo);
    //int getPositionNodoTreeByPosNodo(int num);
    int insertKey(int key,int pointer);
    int duplicates;

private:
    Table_Fields listFields;
    MetaDataTable *metaData;
    int registerCant;
    BitsMap bitsmap;
    QFile *fileOpened;
    Header header;
    int getBytesPositionToWriteRegister(int val);
    void fullBlockWithRegister(int blockNum);
    void creatListPointers(int blockNum);
    int addRegisterToFile(QByteArray info);
    int stackRegisterCant;
    int getPositionRegister(int num);
    QStringList convertQByteToStringList(QByteArray array);
    int getPositionNodoTree(int num);
    int promo_rrn;
    int promo_key;
    int searchValueIntoAnArray(keyTree Arreglo[],int elemento,int size,int &pos);
    void insertInPage(keyTree key,int r_chid,nodoTree *p_page);
    void splitArray(keyTree key,int r_child,nodoTree *p_oldpage,keyTree *promo_key,
                    int *promo_r_child,nodoTree *p_newpage,int block);
    int search_node(keyTree key, nodoTree* p_page,int *pos);
    int MaxKey;
    int minKey;
    int getPrimaryKeyValueFromQByteRegister(QByteArray array);
    int inserValueAtTheTree(int rrn,keyTree key,int *promo_r_child,keyTree *promo_key);
    int searchInTree(int rrn,int key,int *found_rrn,int *found_pos);




};

#endif // TABLE_H
