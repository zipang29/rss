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
#include "ListItems.h"
#include <QDebug>

using namespace std;

class Parser : public QObject
{
    Q_OBJECT

    public:
        Parser(QUrl url, QObject* parent);

    public slots:
        void readFeed();
        void parseFeed();

    private:
        void requestFeed();
        void readItem(QDomElement & elements);

        QUrl url;
        QString src;
        Tika* tika;

		int processingItem;

	private slots:
		void completedItem(Item* item);

    signals:
        void feedRecovered();
        void itemProcessed(Item * item);
        void itemWasRead(Item item);
        void fullList(ListItems * items);

		void feedProcessed();
};

#endif // PARSER_H
