#ifndef NODOTREE_H
#define NODOTREE_H
#include <keytree.h>
#include <stdio.h>
#include <string.h>

class nodoTree
{
public:
    nodoTree();
    keyTree keys[84];
    int hijos[85];
    int keyCount;
};

#endif // NODOTREE_H
