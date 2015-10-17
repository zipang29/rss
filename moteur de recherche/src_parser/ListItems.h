#ifndef LISTITEMS_H
#define LISTITEMS_H

#include <QMap>
#include "Item.h"
#include <iostream>

class ListItems : public QMap<QString, Item*>
{
    public:
        ListItems();
        void addItem(Item * item);
};

#endif // LISTITEMS_H
