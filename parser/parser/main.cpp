#include <QCoreApplication>
#include "Parser.h"
#include "ListItems.h"
#include "IO.h"
#include <QObject>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Parser * p = new Parser(QUrl("http://www.france24.com/en/timeline/rss"));


    return a.exec();
}

