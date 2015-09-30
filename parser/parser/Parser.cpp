#include "Parser.h"

Parser::Parser(QUrl url)
{
    parse(url);
}

void Parser::requestFeed(QUrl url)
{
    QNetworkAccessManager * manager = new QNetworkAccessManager;
    manager->get(QNetworkRequest(url));
    connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getSrcRss()));
}

void Parser::parseFeed()
{

}

void Parser::readItem(const QDomElement & item)
{

}

void Parser::readFeed(QNetworkReply * reply)
{
    this->src = reply->readAll();
    //this->
}
