#ifndef PARSER_H
#define PARSER_H

#include <QDomElement>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iostream>

#include "Item.h"
#include <QDebug>

using namespace std;

class Parser : QObject
{
    Q_OBJECT

    public:
        Parser(QUrl url);

    public slots:
        void readFeed();
        void parseFeed();

    private:
        void requestFeed();
        void readItem(QDomElement & elements);

        QUrl url;
        QString src;

    signals:
        void feedRecovered();
};

#endif // PARSER_H
