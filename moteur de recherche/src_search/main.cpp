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

	QCommandLineOption guiOption(QStringList() << "g" << "gui", "Lance l'application en mode fenetree");
	args.addOption(guiOption);

	args.process(a);

	QStringList positionalArgs = args.positionalArguments();
	if (positionalArgs.size() == 0)
		args.showHelp(1);

	QString index = positionalArgs[0];
	if (index.endsWith("/")) //IndexReader plante si le chemin d'accès finit par / donc on le supprime
		index = index.left(index.length() - 1);

	if (args.isSet(guiOption))
	{

		MainWindow w(index);
		w.show();

		return a.exec();
	}
	else
	{
		QString query;
		SearchEngine * search = new SearchEngine(index);

		QTextStream cin(stdin);

		while (true)
		{
			qInfo() << "\n\nChamps de recherche disponibles : url_du_flux, url_de_la_page, titre (par defaut), description, contenu, langue, category, date, id";
			qInfo() << "Recherche (q pour quitter) > ";
			query = cin.readLine();
			if (query == "q")
				break;
			if (!query.isEmpty())
			{
				Hits * results = search->simpleQuery(query);
				if (results == NULL)
				{
					qWarning() << "La requete a retournee une valeur NULL";
				}
				else
				{
					qInfo() << "Nombre de resultats : " << results->length();
					for (int i = 0; i < results->length(); i++)
					{
						Document doc = results->doc(i);
						Field * result = doc.getField("titre");
						qInfo() << "Titre : " << result->stringValue();
					}
				}
				//delete results;
			}
		}

		delete search;
		return 0;
	}

	return 0;
}
