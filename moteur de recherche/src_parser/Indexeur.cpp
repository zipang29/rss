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
    /*Document * doc = new Document();
	Field * f = new Field("url_du_flux", item->get_url_du_flux().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
    f = new Field("url_de_la_page", item->get_url_de_la_page().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
    f = new Field("titre", item->get_titre().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
    f = new Field("description", item->get_description().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
    f = new Field("contenu", item->get_contenu().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
    f = new Field("langue", item->get_langue().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
    f = new Field("category", item->get_category().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);
    f = new Field("date", item->get_date().toString().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
    doc->add(*f);

    writer->addDocument(doc, writer->getAnalyzer());
	*/
    

}

Indexeur::~Indexeur()
{
	writer->close();
}
