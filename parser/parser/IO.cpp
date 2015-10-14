#include "IO.h"

IO::IO()
{

}

//path : fichier .kch
static void IO::write(const QString path, const ListItems & items)
{
    HashDB db;
    //Ouverture de la BDD en écriture
    if (!db.open(path, HashDB::OWRITER | HashDB::OCREATE))
    {
        cerr << "Erreur à l'ouverture de la BDD : " << db.error().name() << endl;
    }
    for (auto &items : item)
    {

    }
}

static ListItems IO::read(QString path)
{

}
