#include "Tika.h"
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include "Constantes.h"

Tika* Tika::instance = NULL;

Tika::Tika() : QObject()
{
    accessManager = new QNetworkAccessManager(this);

    QString program = "java";
    QStringList args;
    args << "-jar" << "tika-server-1.10.jar";

    tika_server = new QProcess(this);
    tika_server->start(program, args);
}

Tika* Tika::getInstance()
{
    if (instance == NULL) {
        instance = new Tika();
    }
    return instance;
}

void Tika::destroy()
{
    if (instance != NULL) {
        delete instance;
        instance = NULL;
    }
}

void Tika::processItem(Item* item, QString foundLanguage)
{
	processingItems.insert(item->get_id(), item);
	waitingForLanguage.append(item->get_id());
	waitingForDocument.append(item->get_id());

	detectLanguage(item, foundLanguage);
	downloadLink(item);
	checkFinishedItem(item->get_id());
}

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
		if (language == "Unknown") {
			requestLanguage(item);
		}
		else {
			item->set_langue(language);
			waitingForLanguage.removeOne(item->get_id());
		}
	}
}

void Tika::requestLanguage(Item* item)
{
	QNetworkRequest request(QUrl("http://localhost:9998/language/stream"));
	QNetworkReply* reply = accessManager->put(request, item->get_description().toUtf8());
	reply->setProperty("item", item->get_id());
	connect(reply, SIGNAL(finished()), this, SLOT(setLanguage()));
}

void Tika::setLanguage()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	Item* item = processingItems[reply->property("item").toString()];

    if (reply->error() == QNetworkReply::NoError) {
        QString identifier = reply->readAll();
        item->set_langue(getLanguageName(identifier));
    }
    else {
        qWarning() << "Error getting item language using Tika" << reply->errorString();
        item->set_langue("Unknown");
    }

	waitingForLanguage.removeOne(item->get_id());
	checkFinishedItem(item->get_id());
}

void Tika::downloadLink(Item* item)
{
	if (!item->get_url_de_la_page().isEmpty()) {
		QNetworkRequest request(item->get_url_de_la_page());
		QNetworkReply* reply = accessManager->get(request);
		reply->setProperty("item", item->get_id());
		connect(reply, SIGNAL(finished()), this, SLOT(convertDocument()));
	}
	else {
		item->set_resume("Empty"); //TODO: attribut resume pour le texte du document ?
		waitingForDocument.removeOne(item->get_id());
	}
}

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
        qWarning() << "Error downloading document " << oldReply->errorString();
        item->set_resume("Empty"); //TODO: attribut resume pour le texte du document ?
		waitingForDocument.removeOne(item->get_id());
		checkFinishedItem(item->get_id());
    }
    oldReply->deleteLater();
}

void Tika::parseDocument()
{
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
	Item* item = processingItems[reply->property("item").toString()];
    if (reply->error() == QNetworkReply::NoError) {
        QString text = reply->readAll();
        item->set_resume(text); //TODO: attribut resume pour le texte du document ?
    }
    else {
        qWarning() << "Error converting document using Tika" << reply->errorString();
        item->set_resume("Empty"); //TODO: attribut resume pour le texte du document ?
    }

	waitingForDocument.removeOne(item->get_id());
	checkFinishedItem(item->get_id());

    reply->deleteLater();
}

void Tika::checkFinishedItem(QString id)
{
	if (!waitingForLanguage.contains(id) && !waitingForDocument.contains(id)) {
		Item* item = processingItems[id];
		processingItems.remove(id);
		emit(completed(item));
	}
}
