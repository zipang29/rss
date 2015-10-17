#include "ListItems.h"

ListItems::ListItems() : QMap()
{

}

void ListItems::addItem(Item * item)
{
    this->insert(item->get_id(), item);
}
