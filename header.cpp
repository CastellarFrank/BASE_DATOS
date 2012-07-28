#include "header.h"

Header::Header()
{
    memset(this->authors,' ',25);
    memset(this->date,' ',8);
    memset(this->modify_date,' ',8);
    memset(this->version,' ',20);
    this->size_metaData=0;
    this->start_metaData=0;
}
