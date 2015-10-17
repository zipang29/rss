#include <QCoreApplication>
#include "Parser.h"
#include "ListItems.h"
#include "IO.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Parser p(QUrl("http://www.france24.com/en/timeline/rss"));
    ListItems * items = p.getListItems();
    IO::write("bdd.kch", items);

    std::cout << "[*] Test de lecture de la BDD" << std::endl;
    ListItems list2 = IO::read("bdd.kch");
    foreach (Item * item, list2)
    {
        std::cout << "ok1" << item->get_titre().toStdString() << std::endl;
    }
    std::cout << "ok" << std::endl;
    return a.exec();
}

