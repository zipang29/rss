#include "Indexeur.h"
#include "IO.h"
#include "Item.h"

Indexeur::Indexeur()
{
    indexing();
}

void Indexeur::indexing()
{
    StandardAnalyzer * a = new StandardAnalyzer();
	/*
	IndexWriter * writer = new IndexWriter("dbIndexed.db", a, true);
    QMap<QString, Item*> list = IO::read("bdd.kch");
    foreach(Item * item, list.values())
    {
        Document * doc = new Document();
        Field * f = new Field(L"url_du_flux", item->get_url_du_flux().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED);
        doc->add(*f);
        doc->add(Field(L"url_de_la_page", item->get_url_de_la_page().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED));
        doc->add(Field(L"titre", item->get_titre().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED));
        doc->add(Field(L"description", item->get_description().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED));
        doc->add(Field(L"contenu", item->get_contenu().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED));
        doc->add(Field(L"langue", item->get_langue().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED));
        doc->add(Field(L"category", item->get_category().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED));
        doc->add(Field(L"date", item->get_date().toString().toStdWString().c_str(), Field::STORE_YES, Field::INDEX_TOKENIZED));
    }
	*/
}
