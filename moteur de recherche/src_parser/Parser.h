#ifndef PARSER_H
#define PARSER_H

#include <QDomElement>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
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
        Parser(QUrl url, QObject* parent);

    private:
        void requestFeed();
        void readItem(QDomElement & elements);
		void setTimer(int timeToWait = 0);

        QUrl url;
        QString src;
        Tika* tika;

		int processingItem;
		bool timerStarted;

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
