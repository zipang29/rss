#include "Tika.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include "Constantes.h"

/*!
 * \class Tika
 * \brief Classe d'interaction avec Tika
 * \inmodule FEED_COLLECTOR
 *
 * Utilise un serveur Tika, lancé dans le constructeur et terminé à la déstruction de 
 * l'instance de la classe, pour détécter la langue d'un Item et convertir la source de celui-ci
 * en un QString.
 *
 * Le jar du serveur Tika doit se trouver dans le même dossier que l'exécutable et s'appeler \e tika-server-1.10.jar
 *
 * Implémente le patron de conception Singleton.
 */

/*!
 * \fn Tika::completed(Item* item)
 * Signal émit quand les deux étapes du traitement de l'\a item (détéction de langue et 
 * conversion de la source en string) sont terminées.
 */

Tika* Tika::instance = NULL;

/*!
 * Lance le serveur Tika
 */
Tika::Tika() : QObject()
{
    accessManager = new QNetworkAccessManager(this);

    QString program = "java";
    QStringList args;
    args << "-jar" << "tika-server-1.10.jar";

    tika_server = new QProcess(this);
    tika_server->start(program, args);
}

/*!
 * Retourne l'unique instance de la classe, la crée si elle n'existe pas encore.
 */
Tika* Tika::getInstance()
{
    if (instance == NULL) {
        instance = new Tika();
    }
    return instance;
}

/*!
 * Détruit l'instance de la classe si elle existe.
 *
 * Ceci à pour effet d'arrêter le serveur Tika
 */
void Tika::destroy()
{
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

/*!
 * Lance le téléchargement et la conversion en QString du document source de l'\a item si celui-ci
 * en possède un. Dans le cas contraire, assigne au contenu de l'\a item la valeur \e Vide
 *
 * Si \a foundLanguage est un identifiant de langue connue (ie: fr-FR ou en-GB), assigne à 
 * l'\a item le nom complet de la langue (ie: French, English). Dans le cas contraire, utilise Tika
 * pour détécter la langue de l'\a item via sa description. Si la langue est inconnue, elle est notée \e Inconnue dans l'\a item.
 */
void Tika::processItem(Item* item, QString foundLanguage)
{
	processingItems.insert(item->get_id(), item);
	waitingForLanguage.append(item->get_id());
	waitingForDocument.append(item->get_id());

	detectLanguage(item, foundLanguage);
	downloadLink(item);
	checkFinishedItem(item->get_id());
}

/*!
 * Si \a foundLanguage est un identifiant de langue connue (ie: fr-FR ou en-GB), assigne à 
 * l'\a item le nom complet de la langue (ie: French, English). Dans le cas contraire, utilise Tika
 * pour détécter la langue de l'\a item via sa description. Si la langue est inconnue, elle est notée \e Inconnue dans l'\a item.
 */
void Tika::detectLanguage(Item* item, QString foundLanguage)
{
	if (foundLanguage.isEmpty()) {
		requestLanguage(item);
	}
	else {
		QString identifier;
		if (foundLanguage.contains('-'))
			identifier = foundLanguage.left(foundLanguage.indexOf("-"));
		else
			identifier = foundLanguage;

		QString language = getLanguageName(identifier);
		if (language == "Inconnue") {
			requestLanguage(item);
		}
		else {
			item->set_langue(language);
			waitingForLanguage.removeOne(item->get_id());
		}
	}
}

/*!
 * Lance une requête de détéction de langue à Tika pour cet \a item
 */
void Tika::requestLanguage(Item* item)
{
	QNetworkRequest request(QUrl("http://localhost:9998/language/stream"));
	QNetworkReply* reply = accessManager->put(request, item->get_description().toUtf8());
	reply->setProperty("item", item->get_id());
	connect(reply, SIGNAL(finished()), this, SLOT(setLanguage()));
}

/*!
 * Assigne la valeur de retour de la détéction de langue de Tika à l'Item concerné ou \e Inconnue si Tika
 * n'a pas été en mesure de détécter la langue.
 */
void Tika::setLanguage()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	Item* item = processingItems[reply->property("item").toString()];

    if (reply->error() == QNetworkReply::NoError) {
        QString identifier = reply->readAll();
        item->set_langue(getLanguageName(identifier));
    }
    else {
        qWarning() << "Erreur lors de la detection de la langue via Tika :" << reply->errorString();
        item->set_langue("Inconnue");
    }

	waitingForLanguage.removeOne(item->get_id());
	checkFinishedItem(item->get_id());
}

/*!
 * Télécharge le document source de l'\a item si il en a un.
 */
void Tika::downloadLink(Item* item)
{
	if (!item->get_url_de_la_page().isEmpty()) {
		QNetworkRequest request(item->get_url_de_la_page());
		QNetworkReply* reply = accessManager->get(request);
		reply->setProperty("item", item->get_id());
		connect(reply, SIGNAL(finished()), this, SLOT(convertDocument()));
	}
	else {
		item->set_contenu("Vide");
		waitingForDocument.removeOne(item->get_id());
	}
}

/*!
 * Envoi le document téléchargé à Tika pour sa conversion en string.
 *
 * En cas d'erreur de téléchargement, le document de l'item est assigné à la valeur \e Vide
 */
void Tika::convertDocument()
{
    QNetworkReply* oldReply = qobject_cast<QNetworkReply*>(sender());
	Item* item = processingItems[oldReply->property("item").toString()];

    if (oldReply->error() == QNetworkReply::NoError) {
        QNetworkRequest request(QUrl("http://localhost:9998/tika"));
        request.setRawHeader("Accept", "text/plain");

        QNetworkReply* reply = accessManager->put(request, oldReply->readAll());
        reply->setProperty("item", item->get_id());

        connect(reply, SIGNAL(finished()), this, SLOT(parseDocument()));
    }
    else {
        qWarning() << "Erreur lors du telechargement du contenu :" << oldReply->errorString();
		item->set_contenu("Vide");
		waitingForDocument.removeOne(item->get_id());
		checkFinishedItem(item->get_id());
    }
    oldReply->deleteLater();
}

/*!
 * Récupère le string convertit par Tika et l'assigne à l'item concerné.
 * 
 * En cas d'erreur, le document de l'item est assigné à la valeur \e Vide
 */
void Tika::parseDocument()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	Item* item = processingItems[reply->property("item").toString()];
    if (reply->error() == QNetworkReply::NoError) {
        QString text = reply->readAll();
		item->set_contenu(text);
    }
    else {
        qWarning() << "Erreur lors de la conversion du contenu via Tika :" << reply->errorString();
		item->set_contenu("Vide");
    }

	waitingForDocument.removeOne(item->get_id());
	checkFinishedItem(item->get_id());

    reply->deleteLater();
}

/*!
 * Vérifie si les deux étapes du traitement de l'item \a id (détéction de langue et conversion
 * de la source en string) sont terminées dans quel cas le signal completed(Item* item) est émit.
 */
void Tika::checkFinishedItem(QString id)
{
	if (!waitingForLanguage.contains(id) && !waitingForDocument.contains(id)) {
		Item* item = processingItems[id];
		processingItems.remove(id);
		emit(completed(item));
	}
}
