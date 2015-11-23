#ifndef INDEXEUR_H
#define INDEXEUR_H

#include "CLucene.h"
#include "CLucene\analysis\standard\StandardAnalyzer.h"
#include <QObject>
#include "Item.h"

using namespace lucene::index;
using namespace lucene::document;
using namespace lucene::analysis::standard;

class Indexeur : public QObject
{
    Q_OBJECT

    public:
        Indexeur(QString dbPath);

    public slots:
        void indexing(Item * item);

    private:
        QString dbPath;
        StandardAnalyzer * a;
        IndexWriter * writer = NULL;
};

#endif // IO_H
