#include "Parser.h"

/**
 * Constructeur
 * @param url L'URL du flux RSS à traiter
 */
Parser::Parser(QUrl url)
{
    this->url = url;
    requestFeed();
}

/**
 * Permet de débuter la récupération du code source de l'URL spécifié au constructeur
 */
void Parser::requestFeed()
{
    QNetworkAccessManager * manager = new QNetworkAccessManager;
    QNetworkReply * reply = manager->get(QNetworkRequest(this->url));
    connect(reply, SIGNAL(finished()), this, SLOT(readFeed()));
    connect(this, SIGNAL(feedRecovered()), this, SLOT(parseFeed()));
}

/**
 * Parse le flux RSS qui aura été précédement récupéré
 */
void Parser::parseFeed()
{
    cout << "[*] Recreation du document de source : " << this->url.toString().toStdString() << endl;
    QDomDocument doc;
    QString * errors = NULL;
    doc.setContent(this->src, false, errors);
    if (errors != NULL)
    {
        cout << "Erreurs : " << endl << errors << endl;
        cout << "Impossible de parser le flux RSS." << endl;
        return;
    }
    cout << "[*] Document pret a etre parse" << endl;
    cout << "[*] Debut du parsing" << endl;
    // Début du parsing
    QDomElement root = doc.documentElement();
    while(!root.isNull())
    {
        if (root.tagName() == "rss")
        {
            QDomElement rss = root.firstChildElement();
            while(!rss.isNull())
            {
                if (rss.tagName() == "channel")
                {
                    QDomElement channelElements = rss.firstChildElement();
                    while(!channelElements.isNull())
                    {
                        if (channelElements.tagName() == "lastBuildDate")
                        {
                            qDebug() << channelElements.text() << endl;
                            //TODO
                        }
                        else if (channelElements.tagName() == "item")
                        {
                            QDomElement item = channelElements.firstChildElement();
                            this->readItem(item);
                        }
                        channelElements = channelElements.nextSiblingElement();
                    }
                }
                rss = rss.nextSiblingElement();
            }
        }
        root = root.nextSiblingElement();
    }
    cout << "[*] Fin parsing" << endl;
}

/**
 * Parse un item du flux RSS
 * @param elements Les éléments de l'item
 */
void Parser::readItem(QDomElement & elements)
{
    Item item;
    while(!elements.isNull())
    {
        if (elements.tagName() == "category")
        {

        }
        else if (elements.tagName() == "title")
        {
            cout << "Titre: " << elements.text().toStdString() << endl;
        }
        else if (elements.tagName() == "link")
        {

        }
        else if (elements.tagName() == "description")
        {

        }
        else if (elements.tagName() == "thumbnail")
        {

        }
        else if (elements.tagName() == "enclosure")
        {

        }
        else if (elements.tagName() == "guid")
        {

        }
        else if (elements.tagName() == "pubDate")
        {

        }
        else if (elements.tagName() == "source")
        {

        }
        else if (elements.tagName() == "author")
        {
            cout << "Auteur: " << elements.text().toStdString() << endl;
        }
        elements = elements.nextSiblingElement();
    }
}

/**
 * Slot déclenché lorsque la requête déclenché par requestFeed() aboutit
 * Enregistrement du résultat de la requête dans this->src
 * Emission d'un signal feedRecovered() spécifiant que l'on peut débuter le traitement.
 */
void Parser::readFeed()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    this->src = reply->readAll();
    emit feedRecovered();
}
