#ifndef IO_H
#define IO_H

#include <QString>
#include <kchashdb.h>
#include "ListItems.h"
#include "Item.h"

using namespace std;
using namespace kyotocabinet;

class IO
{
    public:
        IO();
        static void write(const QString path, const ListItems & items);
        static ListItems read(QString path);
};

#endif // IO_H
