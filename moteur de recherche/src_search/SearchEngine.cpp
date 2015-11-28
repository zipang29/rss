#include "SearchEngine.h"
#include <QDebug>
#include <QFileInfo>

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
	try {
		QFileInfo file(db_path);
		if (file.exists() || file.isDir())
		{
			this->reader = IndexReader::open(db_path.toStdString().c_str());
			this->searcher = new IndexSearcher(this->reader);
			this->analyser = new StandardAnalyzer();
			this->parser = new QueryParser("titre", this->analyser); // Le champs de recherche par d�faut est le titre
		}
		else
			qCritical() << "La base de donnees d'index n'existe pas : " + db_path;
	}
	catch (...)
	{
		qCritical() << "Une exception a etee lancee depuis le constructeur de la classe SearchEngine";
	}
}

/*!
* Ex�cute une \a query simple. Retourne les Hits les plus pertinents
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
		qCritical() << "Une exception a etee lancee depuis la methode simpleQuery de la classe SearchEngine";
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
