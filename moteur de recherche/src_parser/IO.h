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
		static QMap<QString, Item*> read(QString path);
        void readFeeds(QString path);
        void readFeed(QUrl url);
		int countItemSaved();

        void readDB();
		static void toCSV(QString bdd_path, QString csv_path);
		static IO * getInstance();
		static void deleteInstance();

		static QString path;

	public slots:
        void write(Item* item);

    private:
		IO(QString database_path);

        QList<Parser*> parsers;
		QList<QString> ids;
        //QString path;
		Indexeur * i;

		static IO * io;

		void loadSavedIds();
};

#endif // IO_H
