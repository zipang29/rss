#include "IO.h"

IO::IO()
{

}

//path : fichier .kch
void IO::write(const QString path, const ListItems & items)
{
    HashDB db;
    //Ouverture de la BDD en écriture
    if (!db.open(path.toStdString(), HashDB::OWRITER | HashDB::OCREATE))
    {
        cerr << "Erreur à l'ouverture de la BDD : " << db.error().name() << endl;
    }
    foreach (Item item, items)
    {
        db.set(item.get_id().toStdString(), item);
    }
}

ListItems IO::read(QString path)
{

}
