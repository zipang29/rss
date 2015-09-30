#ifndef PARSER_H
#define PARSER_H

#include <QDomElement>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Parser
{
    Q_OBJECT

    public:
        Parser(QUrl url);
        void requestFeed(QUrl url);
        void readItem(const QDomElement & item);
        void parseFeed();

    public slots:
        void readFeed(QNetworkReply * reply);

    private:
        QString src;

    signals:
        void
};

#endif // PARSER_H
