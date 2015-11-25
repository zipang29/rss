#include <QCoreApplication>
#include <QCommandLineParser>
#include "IO.h"
#include <QDebug>

/*!
 * \page index overview
 * \title Projet RSS-Intelligence - Nailya Bogrova, Guillaume Claudic et Louis Ormières
 * 
 * \l {FEED_COLLECTOR} {Sous projet FEED-collector}
 */

/*!
 * \module FEED_COLLECTOR
 * \title Projet RSS-Intelligence - Nailya Bogrova, Guillaume Claudic et Louis Ormières
 * \subtitle Sous projet FEED-collector
 */

/*!
* \module INDEXER-SEARCHER
* \title Projet RSS-Intelligence - Nailya Bogrova, Guillaume Claudic et Louis Ormières
* \subtitle Sous projet INDEXER-SEARCHER
*/

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser args;
    args.setApplicationDescription("RSS feed collector : Collecte les flux designes et stocke le resultat dans une base de donnees NoSQL HashDB \nEn absence d'options le programme listera les items present dans la base de donnee \n Si l'option csv est donnee, les donnes du fichier base de donne serons exportes dans le fichier csv, aucun parsing n'aura lieu");
    args.addHelpOption();

    args.addPositionalArgument("Base de donnees", "Chemin d'acces au fichier HashDB (.kch), sera cree en cas de collection d'un ou plusieurs flux. Son contenu ne sera jamais efface");
	args.addPositionalArgument("Index", "Nom du dossier gérant le contenu de l'indexation");

    QCommandLineOption fileOption(QStringList() << "f" << "fichier", "Collecte les flux RSS designes dans le fichier (une URL par ligne)", "Chemin d'acces");
    args.addOption(fileOption);

    QCommandLineOption urlOption(QStringList() << "u" << "url", "URL d'un flux rss a collecter", "URL");
    args.addOption(urlOption);

	QCommandLineOption csvOption(QStringList() << "c" << "csv", "Nom d'un fichier CSV dans lequel le contenu de la base de donnee sera exporte", "Chemin d'acces");
	args.addOption(csvOption);

    args.process(a);

	QStringList positionalArgs = args.positionalArguments();
	if (positionalArgs.size() < 2) {
        args.showHelp(1);
    }

    if ((!args.isSet(fileOption) && !args.isSet(urlOption) && !args.isSet(csvOption))) {
		IO* io = new IO(positionalArgs[0], positionalArgs[1]);
        io->readDB();
		return 0;
    }
	else if (args.isSet(csvOption)) {
		IO::toCSV(positionalArgs[0], args.value(csvOption));
		return 0;
	}
    else {
		qSetMessagePattern("[%{time dd-MM-yyyy HH:mm:ss} - %{type}]: %{message}");
		IO* io = new IO(positionalArgs[0], positionalArgs[1]);
        if (args.isSet(fileOption)) {
            QString path = args.value(fileOption);
            io->readFeeds(path);
        }

        if (args.isSet(urlOption)) {
            QUrl url = QUrl(args.value(urlOption));
			io->readFeed(url);
        }
    }

    return a.exec();
}
