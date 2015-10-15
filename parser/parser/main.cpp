#include <QCoreApplication>
#include "Parser.h"
#include "ListItems.h"
#include "IO.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ListItems * list = new ListItems();
    Parser p(QUrl("http://www.france24.com/en/timeline/rss"), list);
    IO::write("bdd.kch", list);

    return a.exec();
}

