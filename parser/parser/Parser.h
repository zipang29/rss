#ifndef PARSER_H
#define PARSER_H

#include <QDomElement>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iostream>

#include "Item.h"

using namespace std;

class Parser : QObject
{
    Q_OBJECT

    public:
        Parser(QUrl url);
        void requestFeed();

    public slots:
        void readFeed();
        void parseFeed();

    private:
        void readItem(QDomElement & elements);

        QUrl url;
        QString src;

    signals:
        void feedRecovered();
};

#endif // PARSER_H
