#ifndef PARSER_H
#define PARSER_H

#include <QDomElement>
#include <QUrl>
#include <iostream>
#include "Constantes.h"
#include "Item.h"
#include "Tika.h"
#include <QDebug>

using namespace std;

class Parser : public QObject
{
    Q_OBJECT

    public:
        Parser(QUrl url, QObject* parent = 0);
		Parser(QUrl url, QString category, QObject* parent = 0);


    private:
        void requestFeed();
        void readItem(QDomElement & elements);
		void setTimer(int timeToWait = 0);

        QUrl url;
        QString src, category;
        Tika* tika;

		int processingItem;
		int timeToWait;
		bool timerStarted;

		QNetworkAccessManager * manager;

	private slots:
		void parseFeed();
		void readFeed();
		void completedItem(Item* item);

		void revisite();

    signals:
        void feedRecovered();
        void itemProcessed(Item * item);

		void feedProcessed();
};

#endif // PARSER_H
