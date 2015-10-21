#include "IO.h"

IO::IO(QString database_path)
{
    path = database_path;
}

//path : fichier .kch
void IO::write(Item * item)
{
    HashDB db;
    //Ouverture de la BDD en écriture
    if (!db.open(path.toStdString(), HashDB::OWRITER | HashDB::OCREATE))
    {
        qCritical() << "Erreur à l'écriture dans la BDD :" << db.error().name();
    }
    db.set(item->get_id().toStdString(), item->toString().toStdString());

	item->deleteLater();
}

QMap<QString, Item*> IO::read(QString path)
{
    QMap<QString, Item*> items;
    HashDB db;
    if (!db.open(path.toStdString(), HashDB::OREADER))
    {
		qCritical() << "Erreur à la lecture dans la BDD :" << db.error().name();
    }
    DB::Cursor * cur = db.cursor();
    cur->jump();
    string key;
    string value;
    while (cur->get(&key, &value, true))
    {
        Item * it = Item::fromString(QString::fromStdString(value));
        it->set_id(QString::fromStdString(key));
        items.insert(it->get_id(),it);
    }
    delete cur;
    return items;
}

void IO::readFeeds(QString path)
{
    QFile f(path);
    f.open(QIODevice::ReadOnly);
    while (!f.atEnd())
    {
        QString line = f.readLine();
        readFeed(QUrl(line));
    }
}

void IO::readFeed(QUrl url)
{
    Parser* p = new Parser(url, this);
	connect(p, SIGNAL(itemProcessed(Item*)), this, SLOT(write(Item*)));
    parsers.append(p);
}

void IO::readDB()
{
	qInfo() << "Contenu de la BDD :";
	QMap<QString, Item*> l = IO::read(path);
    int i = 0;
    foreach(Item * item, l)
    {
        i++;
        qInfo() << i << ":" << item->get_titre();
    }
}
