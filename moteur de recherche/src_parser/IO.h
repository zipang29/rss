#ifndef IO_H
#define IO_H

#include <kchashdb.h>
#include <QString>
#include "Item.h"
#include <QObject>
#include "Parser.h"
#include <QList>
#include "Indexeur.h"

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
		static void toCSV(QString bdd_path, QString csv_path);

	public slots:
        void write(Item* item);

    private:
        QList<Parser*> parsers;
        QString path;
		Indexeur * i;

};

#endif // IO_H
