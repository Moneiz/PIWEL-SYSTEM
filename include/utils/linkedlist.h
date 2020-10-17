#ifndef __LINKED_LIST_H
#define __LINKED_LIST_H

#include "types.h"
#include "mm.h"

class Cell
{
private:
    Cell next;
    int value;
public:
    Cell(/* args */);
    ~Cell();
};

Cell::Cell(/* args */)
{
}

Cell::~Cell()
{
}


#endif