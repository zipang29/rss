#include "Parser.h"

Parser::Parser(QUrl url)
{
    requestFeed(url);
}

void Parser::requestFeed(QUrl url)
{
    QNetworkAccessManager * manager = new QNetworkAccessManager;
    QNetworkReply * reply = manager->get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()), this, SLOT(readFeed()));
    connect(this, SIGNAL(feedRecovered()), this, SLOT(parseFeed()));
}

void Parser::parseFeed()
{
    QDomDocument doc;
    QString * errors = NULL;
    doc.setContent(this->src, false, errors);
    if (errors != NULL)
    {
        std::cout << "Erreurs : " << std::endl << errors << std::endl;
    }
}

void Parser::readItem(const QDomElement & item)
{

}

void Parser::readFeed()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    this->src = reply->readAll();
    emit feedRecovered();
}
