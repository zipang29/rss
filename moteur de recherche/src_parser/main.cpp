#include <QCoreApplication>
#include <QCommandLineParser>
#include "IO.h"
#include <QDebug>

/*!
 * \page index overview
 * \title Projet RSS-Intelligence - Nailya Bogrova, Guillaume Claudic et Louis Ormières
 * 
 * \l {FEED_COLLECTOR} {Sous projet FEED-collector}
 *
 * \l {INDEXER-SEARCHER} {Sous projet INDEXER-SEARCHER}
 */

/*!
 * \module FEED_COLLECTOR
 * \title Projet RSS-Intelligence - Nailya Bogrova, Guillaume Claudic et Louis Ormières
 * \subtitle Sous projet FEED-collector
 *
 * \l {index} {Retour à la liste des sous projets}
 */

/*!
 * \module INDEXER-SEARCHER
 * \title Projet RSS-Intelligence - Nailya Bogrova, Guillaume Claudic et Louis Ormières
 * \subtitle Sous projet INDEXER-SEARCHER
 *
 * \l {index} {Retour à la liste des sous projets}
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser args;
    args.setApplicationDescription("RSS feed collector : Collecte les flux designes et stocke le resultat dans une base de donnees NoSQL HashDB. Ce resultat est indexe via la librairie CLucene dans un dossier .index \nEn absence d'options le programme listera les items present dans la base de donnee \n Si l'option csv est donnee, les donnes du fichier base de donne serons exportes dans le fichier csv, aucun parsing n'aura lieu");
    args.addHelpOption();

    args.addPositionalArgument("Base de donnees", "Chemin d'acces au fichier HashDB (.kch), sera cree en cas de collection d'un ou plusieurs flux. Son contenu ne sera jamais efface");
	args.addPositionalArgument("Donnees d'entrainement francaises", "Chemin d'acces au fichier HashDB (.kch) contenant les items d'entrainement francais");
	args.addPositionalArgument("Donnees d'entrainement anglaises", "Chemin d'acces au fichier HashDB (.kch) contenant les items d'entrainement anglaises");

    QCommandLineOption fileOption(QStringList() << "f" << "fichier", "Collecte les flux RSS designes dans le fichier (une URL par ligne)", "Chemin d'acces");
    args.addOption(fileOption);

    QCommandLineOption urlOption(QStringList() << "u" << "url", "URL d'un flux rss a collecter", "URL");
    args.addOption(urlOption);

	QCommandLineOption csvOption(QStringList() << "c" << "csv", "Nom d'un fichier CSV dans lequel le contenu de la base de donnee sera exporte", "Chemin d'acces");
	args.addOption(csvOption);

	QCommandLineOption trainingOption(QStringList() << "t" << "training", "Lance le parser en mode entrainement pour la classification", "Chemin d'acces");
	args.addOption(trainingOption);

    args.process(a);

	QStringList positionalArgs = args.positionalArguments();
	if (positionalArgs.size() < 1) {
		qDebug() << "toto" << positionalArgs.size();
        args.showHelp(1);
    }

	if ((!args.isSet(fileOption) && !args.isSet(urlOption) && !args.isSet(csvOption) && !args.isSet(trainingOption))) {
		IO::path = positionalArgs[0];
		IO* io = IO::getInstance();
        io->readDB();
		return 0;
    }
	else if (args.isSet(csvOption)) {
		IO::toCSV(positionalArgs[0], args.value(csvOption));
		return 0;
	}
	else if (args.isSet(trainingOption)) {
		qSetMessagePattern("[%{time dd-MM-yyyy HH:mm:ss} - %{type}]: %{message}");
		IO::path = positionalArgs[0];
		IO* io = IO::getInstance();
		io->collectTrainingData(args.value(trainingOption));
	}
    else {
		qSetMessagePattern("[%{time dd-MM-yyyy HH:mm:ss} - %{type}]: %{message}");
		IO::path = positionalArgs[0];

		if (positionalArgs.size() == 3) {
			qDebug() << "training data set";
			IO::training_data_fr = positionalArgs[1];
			IO::training_data_en = positionalArgs[2];
		}

		IO* io = IO::getInstance();

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
	

	/*
	const char** list = sb_stemmer_list();
	

	for (int i = 0; i < 10000000000000000; i++) {
		qDebug() << list[i];
	}

	return 0;
	*/
}
