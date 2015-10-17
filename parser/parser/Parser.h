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

class Parser : QObject
{
    Q_OBJECT

    public:
        Parser(QUrl url);
        ListItems * getListItems();

    public slots:
        void readFeed();
        void parseFeed();
        void addItem(Item * item);

    private:
        void requestFeed();
        void readItem(QDomElement & elements);

        QUrl url;
        QString src;
        Tika* tika;
        ListItems * list;

    signals:
        void feedRecovered();
        void itemProcessed(Item * item);
        void itemWasRead(Item item);
};

#endif // PARSER_H
