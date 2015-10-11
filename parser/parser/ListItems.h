#ifndef LISTITEMS_H
#define LISTITEMS_H
#include "Item.h"

#include <QMap>

class ListItems
{
public:
    ListItems();
    Item findItem(QString id);
    Item valuesFromBegin();
    Item valuesFromEnd();
};

#endif // LISTITEMS_H
