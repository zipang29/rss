#ifndef IO_H
#define IO_H

#include <kchashdb.h>
#include <QString>
#include "ListItems.h"
#include "Item.h"
#include <QObject>
#include <QFile>
#include <QThread>
#include "Parser.h"

using namespace std;
using namespace kyotocabinet;

class IO : public QObject
{
    Q_OBJECT

    public:
        IO();
        static void write(const QString path, Item * item);
        static ListItems read(QString path);
        void readFeeds();

};

#endif // IO_H
