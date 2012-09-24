#include "nodotree.h"

nodoTree::nodoTree()
{
    memset(this->hijos,-1,(4*sizeof(int)));
    this->keyCount=0;
}
