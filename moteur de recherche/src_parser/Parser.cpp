#include "Parser.h"
#include <QTimer>
#include <QNetworkReply>

/*!
 * \class Parser
 * \brief Classe de parsing de flux RSS
 * \inmodule FEED_COLLECTOR
 *
 * Procèdes aux actions suivantes :
 * \list
 *	\li Télécharge le flux RSS
 *	\li Parse le flux et crée une série d'objets \l{Item}
 *	\li Envoie chaque item à \l{Tika} pour détéction de la langue et traitement de sa page cible
 *	\li Emet le signal \l {Parser::itemProcessed()} une fois l'item traité par Tika
 *	\li Emet le signal \l {Parser::feedProcessed()} une fois que tous les items ont été traités
 * \endlist
 *
 * Ce processus est ensuite répété à intervalle variable.
 *
 * Cet intervalle est défini de la sorte :
 * \list
 *	\li Si le flux possède une date de dernière mise à jour, l'intervalle est égal au temps qui s'est écoulé
 *	entre cette date et l'heure courante.
 *	\li Si le flux ne possède pas de date de dernière mise à jours, l'intervalle est égal au temps qui s'est écoulé
 *	entre la date de publication de l'item le plus récent et l'heure courante.
 *	\li Si le flux ne possède pas de date de dernière mise à jours et qu'aucun item ne possède de date de publication,
 *	l'intervalle est fixé à dix minutes.
 * \endlist
 * La valeur de l'intervalle est recalculé à chaque visite du flux.
 *
 * Par ailleurs, sa valeur minimale est de dix minutes.
 */

/*!
 * \fn Parser::feedProcessed()
 * Signal émit quand le traitement de tous les items du flux est terminé
 */

/*!
 * \fn Parser::feedRecovered()
 * Signal émit quand le flux à été téléchargé
 */

/*!
 * \fn Parser::itemProcessed(Item * item)
 * Signal émit quand un \a item à finit d'être traité
 */

/*!
 * Prend en paramètre l'\a url du flux RSS à traiter et optionellement un objet \a parent et lance le traitement du flux
 */
Parser::Parser(QUrl url, QObject * parent) : QObject(parent)
{
	manager = new QNetworkAccessManager(this);
    this->url = url;
    tika = Tika::getInstance();
    connect(tika, SIGNAL(completed(Item*)), this, SLOT(completedItem(Item*)));

	timerStarted = false;

	connect(this, SIGNAL(feedRecovered()), this, SLOT(parseFeed()));
    requestFeed();
}

/*!
 * Lance le parser en mode de collecte d'items d'entrainement pour le classifier, 
 * dans ce mode, tous les items du flux \a url voient sur catégorie prédite systématiquement assignée à \a category.
 * 
 * Prend optionellement un objet \a parent pour simplifier la géstion mémoire.
 */
Parser::Parser(QUrl url, QString category, QObject* parent)
{
	manager = new QNetworkAccessManager(this);
	this->category = category;
	this->url = url;
	tika = Tika::getInstance();
	connect(tika, SIGNAL(completed(Item*)), this, SLOT(completedItem(Item*)));

	timerStarted = false;

	connect(this, SIGNAL(feedRecovered()), this, SLOT(parseFeed()));
	requestFeed();
}

/*!
 * Permet de débuter la récupération du code source de l'URL spécifié au constructeur
 */
void Parser::requestFeed()
{
	qInfo() << "Lancement du traitement du flux" << this->url.toString();

    QNetworkReply * reply = manager->get(QNetworkRequest(this->url));
    connect(reply, SIGNAL(finished()), this, SLOT(readFeed()));
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
							QDateTime date = QDateTime::fromString(channelElements.text(), Qt::RFC2822Date).toUTC();
							int timeToWait = QDateTime::currentMSecsSinceEpoch() - date.toMSecsSinceEpoch();
							setTimer(timeToWait);
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

	if (!timerStarted)
		setTimer();

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
            item->set_url_de_la_page(elements.text());
        }
        else if (elements.tagName() == DESCRIPTION)
        {
            item->set_description(elements.text());
        }
        else if (elements.tagName() == PUB_DATE)
        {
            QDateTime date = QDateTime::fromString(elements.text(), Qt::RFC2822Date).toUTC();
            item->set_date(date);

			if (!timerStarted) {
				int timeToWait = QDateTime::currentMSecsSinceEpoch() - date.toMSecsSinceEpoch();
				setTimer(timeToWait);
			}
        }
        else if (elements.tagName() == LANGUAGE)
        {
            langue = elements.text();
        }

        elements = elements.nextSiblingElement();

    }
    item->set_url_du_flux(url.toString());
	if (!category.isEmpty())
		item->set_predicted_category(category);

	//Génération de l'ID
    QString stringHash = item->get_titre() + item->get_description() + item->get_url_de_la_page();
    QByteArray hash = QCryptographicHash::hash(stringHash.toUtf8(), QCryptographicHash::Md5);
    item->set_id(QString(hash.toHex()));

	processingItem++;
    tika->processItem(item, langue); // Détection de la langue, téléchargement et parsing du document cible de l'item
}

/*!
 * Démarre un minuteur qui déclenchera la revisite du flux après \a timeToWait ms. La durée d'attente
 * est d'au moins 10 minutes, si \a timeToWait est inférieur à cette valeur, sa valeur sera ignorée et le 
 * minuteur sera lancé avec une attente de 10 minutes
 */
void Parser::setTimer(int timeToWait)
{
	if (timeToWait < (10 * 60 * 1000))
		timeToWait = 10 * 60 * 1000;

	if (!timerStarted) {
		QTimer::singleShot(timeToWait, this, SLOT(revisite()));
		timerStarted = true;
		qInfo() << "Revisite du flux" << url.url() << "dans" << timeToWait << "ms";
	}
}

/*!
 * Lance la revisite du flux.
 */
void Parser::revisite()
{
	qInfo() << "Revisite du flux" << url.url();
	timerStarted = false;
	requestFeed();
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
	static int redirection = 0;
    QNetworkReply * reply = qobject_cast<QNetworkReply*>(sender());
	if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 301 || reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 302)
	{
		if (redirection > 3)
		{
			emit feedProcessed();
			redirection = 0;
			return;
		}
		QUrl url = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();
		if (url.isRelative())
			url = reply->url().resolved(url);
		QNetworkRequest request(url);
		QNetworkReply* newReply = manager->get(request);
		connect(newReply, SIGNAL(finished()), this, SLOT(readFeed()));
		redirection++;
	}
	else
	{
		this->src = reply->readAll();
		emit feedRecovered();
	}
    

	reply->deleteLater();
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
			qInfo() << "Traitement du flux" << url.url() << "termine";
			emit(feedProcessed());
		}
	}
}
