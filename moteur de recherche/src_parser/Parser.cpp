#include "Parser.h"

/*!
 * \class Parser
 * \brief Classe de parsing du projet
 * \inmodule FEED_COLLECTOR
 *
 * //TODO
 */

/*!
 * \fn Parser::feedProcessed()
 * //TODO
 */

/*!
 * \fn Parser::feedRecovered()
 * //TODO
 */

/*!
 * \fn Parser::itemProcessed(Item * item)
 * Signal émit quand un \a item à finit d'être traité
 */

/*!
 * \fn Parser::itemWasRead(Item item)
 * \a item //TODO
 */


/*!
 * Prend en paramètre l'\a url du flux RSS à traiter et optionellement un objet \a parent
 */
Parser::Parser(QUrl url, QObject * parent) : QObject(parent)
{
    this->url = url;
    tika = Tika::getInstance();
    connect(tika, SIGNAL(completed(Item*)), this, SLOT(completedItem(Item*)));

    requestFeed();
}

/*!
 * Permet de débuter la récupération du code source de l'URL spécifié au constructeur
 */
void Parser::requestFeed()
{
	qInfo() << "Lancement du traitement du flux" << this->url.toString();

    QNetworkAccessManager * manager = new QNetworkAccessManager;
    QNetworkReply * reply = manager->get(QNetworkRequest(this->url));
    connect(reply, SIGNAL(finished()), this, SLOT(readFeed()));
    connect(this, SIGNAL(feedRecovered()), this, SLOT(parseFeed()));
}

/*!
 * Parse le flux RSS qui aura été précédement récupéré
 */
void Parser::parseFeed()
{
	qInfo() << "Parsing du flux" << this->url.toString();
	processingItem = 0;
    QDomDocument doc;
    QString * errors = NULL;
    doc.setContent(this->src, false, errors);
    if (errors != NULL)
    {
		qWarning() << "Erreur lors du parsing du flux" << this->url.toString() << ":" << errors;
        return;
    }
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
	qInfo() << "Fin du parsing du flux" << this->url.toString();
}

/*!
 * Parse un item du flux RSS
 * 
 * Reçoit les \a elements de l'item
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
        else if (elements.tagName() == CATEGORY)
        {
			item->set_category(elements.text());
        }
        else if (elements.tagName() == LINK)
        {
            item->set_url_du_flux(elements.text());
        }
        else if (elements.tagName() == DESCRIPTION)
        {
            item->set_description(elements.text());
        }
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
        else if (elements.tagName() == LANGUAGE)
        {
            langue = elements.text();
        }

        elements = elements.nextSiblingElement();

    }
    item->set_url_du_flux(url.toString());

	//Génération de l'ID
    QString stringHash = item->get_titre() + item->get_description() + item->get_url_de_la_page();
    QByteArray hash = QCryptographicHash::hash(stringHash.toUtf8(), QCryptographicHash::Md5);
    item->set_id(hash);

	processingItem++;
    tika->processItem(item, langue); // Détection de la langue, téléchargement et parsing du document cible de l'item
}

/*!
 * Slot déclenché lorsque la requête déclenché par requestFeed() aboutit.
 *
 * Enregistrement du résultat de la requête dans this->src
 *
 * Emission d'un signal feedRecovered() spécifiant que l'on peut débuter le traitement.
 */
void Parser::readFeed()
{
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
    this->src = reply->readAll();
    emit feedRecovered();
}


/*!
* Slot déclenché quand Tika à fini le traitement d'un Item.
*
* Emet le signal feedProcessed() si tous les items ont été traités par tika.
*/
void Parser::completedItem(Item* item)
{
	if (item->get_url_du_flux() == url.url()) {
		processingItem--;
		emit(itemProcessed(item));

		if (processingItem == 0) {
			qInfo() << "Traitement du flux" << url.url() << "terminé";
			emit(feedProcessed());
		}
	}
}
