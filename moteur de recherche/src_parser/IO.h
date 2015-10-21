#ifndef IO_H
#define IO_H

#include <kchashdb.h>
#include <QString>
#include "Item.h"
#include <QObject>
#include <QFile>
#include <QThread>
#include "Parser.h"
#include <QList>

using namespace std;
using namespace kyotocabinet;

class IO : public QObject
{
    Q_OBJECT

    public:
        IO(QString database_path);
		static QMap<QString, Item*> read(QString path);
        void readFeeds(QString path);
        void readFeed(QUrl url);

        void readDB();

	public slots:
        void write(Item* item);

    private:
        QList<Parser*> parsers;
        QString path;

};

#endif // IO_H
