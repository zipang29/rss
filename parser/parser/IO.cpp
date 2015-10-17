#include "IO.h"

IO::IO()
{

}

//path : fichier .kch
void IO::write(const QString path, ListItems * items)
{
    HashDB db;
    //Ouverture de la BDD en écriture
    if (!db.open(path.toStdString(), HashDB::OWRITER | HashDB::OCREATE))
    {
        cerr << "Erreur à l'écriture dans la BDD : " << db.error().name() << endl;
    }
    foreach (Item * item, *items)
    {
        qDebug() << "Ajout d'un item à la bdd";
        db.set(item->get_id().toStdString(), item->toString().toStdString());
    }
    qDebug() << "fin de l'écriture dans la bdd";
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
        Item * it = Item::fromString(QString::fromStdString(value));
        it->set_id(QString::fromStdString(key));
        items.addItem(it);
    }
    delete cur;
    return items;
}

void IO::writeList(ListItems *items)
{
    cout << "[*] Ecriture" << endl;
    IO::write("bdd.kch", items);

    std::cout << "[*] Test de lecture de la BDD" << std::endl;
    ListItems list2 = IO::read("bdd.kch");
    foreach (Item * item, list2)
    {
        std::cout << "Lecture : " + item->get_titre().toStdString() << std::endl;
    }
}
