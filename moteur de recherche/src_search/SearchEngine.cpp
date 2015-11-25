#include "SearchEngine.h"
#include <QDebug>

SearchEngine::SearchEngine(QString db_path)
{
	this->searcher = new IndexSearcher(db_path.toStdString().c_str());
	this->analyser = new StandardAnalyzer();
	this->parser = new QueryParser("title", this->analyser); // Le champs de recherche par défaut est le titre
}

Hits * SearchEngine::simpleQuery(QString query)
{
	if (query != NULL)
		delete this->query;
	this->query = this->parser->parse((const TCHAR*)query.toStdString().c_str());
	Hits * hits = this->searcher->search(this->query);
	qDebug() << "Nombre de résultats :" << hits->length();
	return hits;
}

SearchEngine::~SearchEngine()
{
	searcher->close();
	if (query != NULL)
		delete this->query;
	delete this->searcher;
	delete this->analyser;
	delete this->parser;
}
