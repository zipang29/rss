#include "CLucene.h"
#include <QString>
#include "CLucene\search\QueryFilter.h"

using namespace lucene::search;
using namespace lucene::queryParser;
using namespace lucene::document;
using namespace lucene::analysis::standard;
using namespace lucene::index;
using namespace lucene::store;

class SearchEngine
{
	public:
		SearchEngine(QString db_path);
		Hits * simpleQuery(QString query);
		~SearchEngine();

	private:
		IndexReader * reader;
		IndexSearcher * searcher;
		Query * query = NULL;
		StandardAnalyzer * analyser;
		QueryParser * parser;
};

