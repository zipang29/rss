#ifndef INDEXEUR_H
#define INDEXEUR_H

#include "CLucene.h"
#include <QObject>

using namespace lucene::index;
using namespace lucene::document;
using namespace lucene::analysis::standard;

class Indexeur : public QObject
{
    Q_OBJECT

    public:
        Indexeur();

    public slots:
        void indexing();

    private:

};

#endif // IO_H
