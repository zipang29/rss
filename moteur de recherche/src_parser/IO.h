#ifndef IO_H
#define IO_H

#include <kchashdb.h>
#include <QString>
#include "Item.h"
#include <QObject>
#include "Parser.h"
#include "Stemmer.h"
#include <QList>
#include "Indexeur.h"
#include "Dictionnaire.h"
#include "Classifier.h"

using namespace std;
using namespace kyotocabinet;

class Classifier;

class IO : public QObject
{
    Q_OBJECT

    public:
		static QMap<QString, Item*> read(QString path);
        void readFeeds(QString path);
        void readFeed(QUrl url);
		void collectTrainingData(QString path);
		int countItemSaved();
		Dictionnaire* getDictionary(Language lang = ENGLISH);

        void readDB();
		static void toCSV(QString bdd_path, QString csv_path);
		static IO * getInstance();
		static void deleteInstance();

		static QString path, training_data_fr, training_data_en;

	public slots:
        void write(Item* item);
		void classify();

    private:
		IO(QString database_path, QString training_data_fr, QString training_data_en);

        QList<Parser*> parsers;
		QList<QString> ids;
        //QString path;
		Indexeur * i;
		Classifier* classifier;
		Stemmer stemmer;
		Dictionnaire fra, eng;

		bool trainingMode;
		int trainingDone = 0;

		static IO * io;

		void loadDictionaries();
		void loadSavedIds();

		void saveDictionaries();
};

#endif // IO_H
