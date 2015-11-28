#include <QCoreApplication>
#include <QTextStream>
#include "SearchEngine.h"
#include <QDebug>
#include <QCommandLineParser>
#include "MainWindow.h"

using namespace lucene::search;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	
	QCommandLineParser args;
	args.setApplicationDescription("Application GUI et Console permettant d'effectuer des recherches sur un index CLucene");
	args.addHelpOption();

	args.addPositionalArgument("Index CLucene", "Chemin d'acces au dossier d'index CLucene");

	QCommandLineOption guiOption(QStringList() << "g" << "gui", "Lance l'application en mode fenetrees");
	args.addOption(guiOption);

	args.process(a);

	QStringList positionalArgs = args.positionalArguments();
	if (positionalArgs.size() == 0)
		args.showHelp();

	if (args.isSet(guiOption))
	{

		MainWindow w(positionalArgs[0]);
		w.show();

		return a.exec();
	}
	else
	{
		QString query;
		SearchEngine * search = new SearchEngine(positionalArgs[0]);

		QTextStream cin(stdin);

		while (true)
		{
			qDebug() << "\n\nChamps de recherche disponibles : url_du_flux, url_de_la_page, titre (par défaut), description, contenu, langue, category, date, id";
			qDebug() << "Recherche > ";
			query = cin.readLine();
			if (query == "q")
				break;
			if (!query.isEmpty())
			{
				Hits * results = search->simpleQuery(query);
				if (results == NULL)
				{
					qCritical() << "La requête a retournee une valeur NULL. Le programme va se terminer.";
					return -1;
				}
				qDebug() << "Nombre de resultats : " << results->length();
				for (int i = 0; i < results->length(); i++)
				{
					Document doc = results->doc(i);
					Field * result = doc.getField("titre");
					qDebug() << "Titre : " << result->stringValue();
				}
				//delete results;
			}
		}

		delete search;
		return 0;
	}

	return 0;
}
