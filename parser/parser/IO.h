#ifndef IO_H
#define IO_H

#include <kchashdb.h>
#include <QString>
#include "ListItems.h"
#include "Item.h"
#include <QObject>

using namespace std;
using namespace kyotocabinet;

class IO : public QObject
{
    Q_OBJECT

    public:
        IO();
        static void write(const QString path, ListItems * items);
        static ListItems read(QString path);

    public slots:
        void writeList(ListItems * items);
};

#endif // IO_H
