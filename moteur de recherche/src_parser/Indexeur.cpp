#include "Indexeur.h"
#include "IO.h"
#include <QFileInfo>

Indexeur::Indexeur(QString dbPath)
{
    this->dbPath = dbPath;
    a = new StandardAnalyzer();
	QFileInfo file(dbPath);
	if (file.exists() || file.isDir())
		writer = new IndexWriter(dbPath.toStdString().c_str(), a, false);
	else
		writer = new IndexWriter(dbPath.toStdString().c_str(), a, true);
}

void Indexeur::indexing(Item * item)
{
    Document * doc = new Document();
	Field * f = new Field("url_du_flux", (const TCHAR*)item->get_url_du_flux().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
	f = new Field("url_de_la_page", (const TCHAR*)item->get_url_de_la_page().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
	f = new Field("titre", (const TCHAR*)item->get_titre().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
	f = new Field("description", (const TCHAR*)item->get_description().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
	f = new Field("contenu", (const TCHAR*)item->get_contenu().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
	f = new Field("langue", (const TCHAR*)item->get_langue().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
	f = new Field("category", (const TCHAR*)item->get_category().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
	f = new Field("date", (const TCHAR*)item->get_date().toString().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);

    writer->addDocument(doc, writer->getAnalyzer());
}

Indexeur::~Indexeur()
{
	writer->close();
}
