#ifndef PARSER_H
#define PARSER_H

#include <QDomElement>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <iostream>

class Parser : QObject
{
    Q_OBJECT

    public:
        Parser(QUrl url);
        void requestFeed(QUrl url);
        void readItem(const QDomElement & item);

    public slots:
        void readFeed();
        void parseFeed();

    private:
        QString src;

    signals:
        void feedRecovered();
};

#endif // PARSER_H
