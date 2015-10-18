#include <QCoreApplication>
#include <QCommandLineParser>
#include "IO.h"
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QCommandLineParser args;
    args.setApplicationDescription("RSS feed parser : Parses given feeds ands stores results in a NoSQL hash database");
    args.addHelpOption();

    args.addPositionalArgument("Database", "Path to HashDB file");

    QCommandLineOption fileOption(QStringList() << "f" << "file", "Parse RSS feeds in the file (one url per line)", "path");
    args.addOption(fileOption);

    QCommandLineOption urlOption(QStringList() << "u" << "url", "URL of the RSS feed to parse", "URL");
    args.addOption(urlOption);

    args.process(a);

	QStringList positionalArgs = args.positionalArguments();
	if (positionalArgs.size() == 0) {
        args.showHelp(1);
    }
    if ((!args.isSet(fileOption) && !args.isSet(urlOption))) {
		IO* io = new IO(positionalArgs[0]);
        io->readDB();
		return 0;
    }
    else {
		IO* io = new IO(positionalArgs[0]);
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
