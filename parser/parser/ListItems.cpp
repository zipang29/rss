#include "ListItems.h"

ListItems::ListItems()
{
    QMap <QString, Item> mapItems;
}

Item findItem(QString id){
    Item val = mapItems.find(id);
    return val;
}

Item valuesFromBegin(){
	QMap<QString, Item>::iterator i;
	for (i = mapItems.begin(); i != mapItems.end(); ++i)
		cout << i.key() << ": " << i.value() << endl;
}

Item valuesFromEnd(){
	QMap<QString, Item>::iterator i;
	for (i = mapItems.end(); i != mapItems.begin(); --i)
		cout << i.key() << ": " << i.value() << endl;
}
