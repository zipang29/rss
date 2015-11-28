#include <QCoreApplication>
#include <QTextStream>
#include "SearchEngine.h"
#include <QDebug>

using namespace lucene::search;

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	
	QString query;
	SearchEngine * search = new SearchEngine("bdd.kch.index");

	QTextStream cin(stdin);

	while (true)
	{
		query = cin.readLine();
		if (query == "q")
			break;
		if (!query.isEmpty())
		{
			Hits * results = search->simpleQuery(query);
			if (results == NULL)
			{
				qCritical() << "La requ�te a retournee une valeur NULL. Le programme va se terminer.";
				return -1;
			}
			qDebug() << "Nombre de resultats : " << results->length();
			for (int i = 0; i < results->length(); i++)
			{
				Document doc = results->doc(i);
				//QString result = (QString)doc.get((const TCHAR *)"titre");
				Field * result = doc.getField("titre");
				qDebug() << "Titre : " << result->toString();
			}
			//delete results;
		}
	}

	delete search;

	return 0;
}
