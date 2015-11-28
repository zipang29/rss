#include "SearchEngine.h"
#include <QDebug>

/*!
* \class SearchEngine
* \brief Classe permettant d'effectuer des requêtes dans la BDD afin de retrouver des items en fonction de mots clés
* \inmodule INDEXER-SEARCHER
*
* S'occupe d'effectuer des recherches dans la base de données et de renvoyer les résultats trouvés
*/

/*!
* Constructeur prenant en paramètre \a db_path correspondant au chemin vers la BDD d'indexation.
*/
SearchEngine::SearchEngine(QString db_path)
{
	try {
		this->reader = IndexReader::open(db_path.toStdString().c_str());
		this->searcher = new IndexSearcher(this->reader);
		this->analyser = new StandardAnalyzer();
		this->parser = new QueryParser("titre", this->analyser); // Le champs de recherche par défaut est le titre
	}
	catch (...)
	{
		qDebug() << "exception constructeur";
	}
}

/*!
* Exécute une \a query simple. Retourne les Hits les plus pertinents
*/
Hits * SearchEngine::simpleQuery(QString query)
{
	if (query != NULL)
		delete this->query;
	try
	{
		this->query = this->parser->parse((const TCHAR*)query.toStdString().c_str());
		Hits * hits = this->searcher->search(this->query);
		return hits;
	}
	catch (...)
	{
		qDebug() << "exception methode";
	}
	return NULL;
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
