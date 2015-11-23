#include "IO.h"
#include <QFile>
#include "Indexeur.h"

/*!
 * \class IO
 * \brief Classe de gestion des entrées/sorties
 * \inmodule FEED_COLLECTOR
 *
 * S'occupe des opérations sur les bases NoSQL HashDB (KyotoCabinet) et de la gestion 
 * des \l{Parser}{Parsers RSS}
 */

/*!
 * Constructeur prenant en paramètre \a database_path correspondant au chemin vers la BDD.
 */
IO::IO(QString database_path)
{
    path = database_path;
}

/*!
 * Ecrit un \a item dans la BDD
 */
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

/*!
 * Lit la BDD contenue dans le fichier \a path.
 * 
 * Retourne la liste des items avec pour clé leur hash
 */
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

/*!
 * Lit une liste de flux à partir du fichier texte \a path (une url de flux par ligne)
 * et lance les traitements sur ceux-ci
 */
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

/*!
 * Lit le flux RSS \a url et lance les traitements sur celui-ci
 */
void IO::readFeed(QUrl url)
{
    Parser* p = new Parser(url, this);
    Indexeur * i = new Indexeur("bdd.db"); // TODO ajouter un argument à passer au programme pour pouvoir définir vers quel fichier on veut enregistrer les données
	connect(p, SIGNAL(itemProcessed(Item*)), this, SLOT(write(Item*)));
    connect(p, SIGNAL(itemProcessed(Item*)), i, SLOT(indexing(Item*)));
    parsers.append(p);
}

/*!
 * Lit la base de données et affiche le titre de chaque item lu dans la sortie standard
 */
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

/*!
 * Sauvegarde le contenu de la base de donnée \a bdd_path dans le fichier CSV \a csv_path.
 * \sa Item::toCSV()
 */
void IO::toCSV(QString bdd_path, QString csv_path)
{
	QMap<QString, Item*> items = IO::read(bdd_path);
	qInfo() << "Ecriture de" << items.size() << "items dans" << csv_path;

	QFile csv_file(csv_path);
	csv_file.open(QFile::WriteOnly | QFile::Truncate);

	QTextStream csv(&csv_file);

	csv << "Titre;";
	csv << "Date;";
	csv << "Description;";
	csv << "Contenu;";
	csv << "Langue;";
	csv << "Categorie;";
	csv << "Url de la page cible;";
	csv << "Url du flux RSS;";
	
	csv << "\n";

	foreach(Item* item, items.values()) {
		csv << item->toCSV();
		csv << "\n";
	}

	csv_file.close();

	qInfo() << "Ecriture terminee";
}
