#include "Indexeur.h"
#include <QFileInfo>
/*!
* \class Indexeur
* \brief Classe d'indexation d'item
* \inmodule INDEXER-SEARCHER
*
* S'occupe d'indexer des items dans une base de données sur le disque
*/

/*!
* Constructeur prenant en paramètre \a dbPath correspondant au chemin vers la BDD d'indexation.
*/
Indexeur::Indexeur(QString dbPath, QObject* parent) : QObject(parent)
{
    this->dbPath = dbPath;
    a = new StandardAnalyzer();
}

/*!
* Index un \a item dans la BDD
*/
void Indexeur::indexing(Item * item)
{
	QFileInfo file(dbPath);
	if (file.exists() || file.isDir())
		writer = new IndexWriter(dbPath.toStdString().c_str(), a, false);
	else
		writer = new IndexWriter(dbPath.toStdString().c_str(), a, true);

    Document * doc = new Document();
	Field* id = new Field("id", (const TCHAR*)item->get_id().toStdString().c_str(), Field::STORE_YES | Field::INDEX_TOKENIZED);
	doc->add(*id);

	Field* url_du_flux = new Field("url_du_flux", (const TCHAR*)item->get_url_du_flux().toStdString().c_str(), Field::STORE_YES | Field::INDEX_TOKENIZED);
	doc->add(*url_du_flux);

	Field* url_de_la_page = new Field("url_de_la_page", (const TCHAR*)item->get_url_de_la_page().toStdString().c_str(), Field::STORE_YES | Field::INDEX_TOKENIZED);
	doc->add(*url_de_la_page);

	Field* titre = new Field("titre", (const TCHAR*)item->get_titre().toStdString().c_str(), Field::STORE_YES | Field::INDEX_TOKENIZED);
	doc->add(*titre);

	Field* description = new Field("description", (const TCHAR*)item->get_description().toStdString().c_str(), Field::STORE_YES | Field::INDEX_TOKENIZED);
	doc->add(*description);

	Field* contenu = new Field("contenu", (const TCHAR*)item->get_contenu().toStdString().c_str(), Field::STORE_YES | Field::INDEX_TOKENIZED);
	doc->add(*contenu);

	Field* langue = new Field("langue", (const TCHAR*)item->get_langue().toStdString().c_str(), Field::STORE_YES | Field::INDEX_TOKENIZED);
	doc->add(*langue);

	Field* category = new Field("category", (const TCHAR*)item->get_category().toStdString().c_str(), Field::STORE_YES | Field::INDEX_TOKENIZED);
	doc->add(*category);

	Field* date = new Field("date", (const TCHAR*)item->get_date().toString().toStdString().c_str(), Field::STORE_YES | Field::INDEX_TOKENIZED);
	doc->add(*date);

    writer->addDocument(doc, writer->getAnalyzer());
	writer->flush();
	writer->close();

	delete writer;
	writer = NULL;

	fields.append(id);
	fields.append(url_du_flux);
	fields.append(url_de_la_page);
	fields.append(titre);
	fields.append(description);
	fields.append(contenu);
	fields.append(langue);
	fields.append(category);
	fields.append(date);

	docs.append(doc);
}

/*!
* Destructeur
*/
Indexeur::~Indexeur()
{
	QList<Document*>::iterator docIter = docs.begin();
	while (docIter != docs.end()) {
		Document* doc = docIter[0];
		docIter = docs.erase(docIter);
		delete doc;
	}

	QList<Field*>::iterator fieldIter = fields.begin();
	while (fieldIter != fields.end()) {
		Field* f = fieldIter[0];
		fieldIter = fields.erase(fieldIter);
		delete f;
	}

	if (a != NULL)
		delete a;
}
