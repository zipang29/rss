#include "Parser.h"

/**
 * Constructeur
 * @param url L'URL du flux RSS à traiter
 */
Parser::Parser(QUrl url, QObject * parent) : QObject(parent)
{
    this->url = url;
    tika = Tika::getInstance();
    connect(tika, SIGNAL(completed(Item*)), this, SLOT(completedItem(Item*)));

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
	processingItem = 0;
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
        if (root.tagName() == RSS)
        {
            QDomElement rss = root.firstChildElement();
            while(!rss.isNull())
            {
                if (rss.tagName() == CHANNEL)
                {
                    QDomElement channelElements = rss.firstChildElement();
                    while(!channelElements.isNull())
                    {
                        if (channelElements.tagName() == LAST_BUILD_DATE)
                        {
                            //qDebug() << channelElements.text() << endl;
                            //TODO
                        }
                        else if (channelElements.tagName() == ITEM)
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
    Item * item = new Item();
    QString langue = "";
    while(!elements.isNull())
    {
        if (elements.tagName() == TITLE)
        {
            item->set_titre(elements.text());
        }
        /*else if (elements.tagName() == CATEGORY)
        {

        }*/
        else if (elements.tagName() == LINK)
        {
            item->set_url_du_flux(elements.text());
        }
        else if (elements.tagName() == DESCRIPTION)
        {
            item->set_description(elements.text());
        }
        /*else if (elements.tagName() == THUMBNAIL)
        {

        }
        else if (elements.tagName() == ENCLOSURE)
        {

        }
        else if (elements.tagName() == GUID)
        {

        }*/
        else if (elements.tagName() == PUB_DATE)
        {
            QLocale locale(QLocale::English, QLocale::UnitedKingdom);
            QDateTime date = locale.toDateTime(elements.text(), "ddd, dd MMM yyyy hh:mm:ss");
            date.setTimeSpec(Qt::UTC);
            item->set_date(date);
        }
        else if (elements.tagName() == SOURCE)
        {
            item->set_url_de_la_page(elements.attribute("url"));
        }
        else if (elements.tagName() == AUTHOR)
        {
            //cout << "Auteur: " << elements.text().toStdString() << endl;
        }
        else if (elements.tagName() == LANGUAGE)
        {
            langue = elements.text();
        }

        elements = elements.nextSiblingElement();

    }
    item->set_url_du_flux(url.toString());
    QString stringHash = item->get_titre() + item->get_description() + item->get_url_de_la_page();
    QByteArray hash = QCryptographicHash::hash(stringHash.toUtf8(), QCryptographicHash::Md5);
    item->set_id(hash);
	processingItem++;
    tika->processItem(item, langue); // Détection de la langue, téléchargement et parsing du document cible de l'item
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

void Parser::completedItem(Item* item)
{
	if (item->get_url_du_flux() == url.url()) {
		processingItem--;
		emit(itemProcessed(item));

		if (processingItem == 0) {
			qDebug() << "Traitement de " << url.url() << "terminé";
			emit(feedProcessed());
		}
	}
	
}