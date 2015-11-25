#include <QCoreApplication>
#include <QTextStream>
#include "SearchEngine.h"
#include <QDebug>

using namespace lucene::search;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	
	QString query;
	SearchEngine * search = new SearchEngine("bdd.db");

	QTextStream cin(stdin);

	while (true)
	{
		query = cin.readLine();
		if (query == "q")
			break;
		if (!query.isEmpty())
		{
			Hits * results = search->simpleQuery(query);
			qDebug() << "Nombre de résultats : " + results->length();
			for (int i = 0; i < results->length(); i++)
			{
				Document doc = results->doc(i);
				QString result = (QString)doc.get((const TCHAR *)"title");
				qDebug() << "Titre : " << result;
			}
			delete results;
		}
	}

	delete search;

	return 0;
}
