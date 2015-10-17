#include "IO.h"

IO::IO()
{
}

//path : fichier .kch
void IO::write(const QString path, Item * item)
{
    HashDB db;
    //Ouverture de la BDD en écriture
    if (!db.open(path.toStdString(), HashDB::OWRITER | HashDB::OCREATE))
    {
        cerr << "Erreur à l'écriture dans la BDD : " << db.error().name() << endl;
    }
    //qDebug() << "Ajout d'un item à la bdd";
    db.set(item->get_id().toStdString(), item->toString().toStdString());
    //qDebug() << "fin de l'écriture dans la bdd";
}

ListItems IO::read(QString path)
{
    ListItems items;
    HashDB db;
    if (!db.open(path.toStdString(), HashDB::OREADER))
    {
        cerr << "Erreur à la lecture dans la BDD : " << db.error().name() << endl;
    }
    DB::Cursor * cur = db.cursor();
    cur->jump();
    string key;
    string value;
    while (cur->get(&key, &value, true))
    {
        std::cout << "lecture" << endl;
        Item * it = Item::fromString(QString::fromStdString(value));
        it->set_id(QString::fromStdString(key));
        items.addItem(it);
    }
    delete cur;
    return items;
}

void IO::readFeeds()
{
    QFile f("ListFluxRSS-v2.txt");
    f.open(QIODevice::ReadOnly);
    while (!f.atEnd())
    {
        QString line = f.readLine();
        Parser * p = new Parser(QUrl(line), this);
    }

    //Test de lecture de la bdd
    /*ListItems l = IO::read("bdd.kch");
    int i = 0;
    foreach(Item * item, l)
    {
        i++;
        std::cout << i << " : " << item->get_titre().toStdString();
    }*/
}
