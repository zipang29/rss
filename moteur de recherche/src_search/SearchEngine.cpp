#include "SearchEngine.h"
#include <QDebug>

/*!
* \class SearchEngine
* \brief Classe permettant d'effectuer des requ�tes dans la BDD afin de retrouver des items en fonction de mots cl�s
* \inmodule INDEXER-SEARCHER
*
* S'occupe d'effectuer des recherches dans la base de donn�es et de renvoyer les r�sultats trouv�s
*/

/*!
* Constructeur prenant en param�tre \a db_path correspondant au chemin vers la BDD d'indexation.
*/
SearchEngine::SearchEngine(QString db_path)
{
	this->searcher = new IndexSearcher(db_path.toStdString().c_str());
	this->analyser = new StandardAnalyzer();
	this->parser = new QueryParser("title", this->analyser); // Le champs de recherche par d�faut est le titre
}

/*!
* Ex�cute une \a query simple. Retourne les Hits les plus pertinents
*/
Hits * SearchEngine::simpleQuery(QString query)
{
	if (query != NULL)
		delete this->query;
	this->query = this->parser->parse((const TCHAR*)query.toStdString().c_str());
	Hits * hits = this->searcher->search(this->query);
	qDebug() << "Nombre de r�sultats :" << hits->length();
	return hits;
}

/*!
* Destructeur
*/
SearchEngine::~SearchEngine()
{
	searcher->close();
	if (query != NULL)
		delete this->query;
	delete this->searcher;
	delete this->analyser;
	delete this->parser;
}
