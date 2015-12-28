#include "IO.h"
#include <QFile>

IO * IO::io = NULL;
QString IO::path = "";
QString IO::training_data_fr = "";
QString IO::training_data_en = "";

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
 *
 * Charge la liste des identifiants des items déjà présents dans la BDD afin de ne pas creer de duplicata
 */
IO::IO(QString database_path, QString training_data_fr, QString training_data_en)
{
    path = database_path;
	trainingMode = false;
	this->i = new Indexeur(database_path + ".index", this);

	if (!training_data_fr.isEmpty() && !training_data_en.isEmpty())
		classifier = new Classifier(training_data_fr, training_data_en, this);
	else
		classifier = NULL;

	loadDictionaries();
	loadSavedIds();
}

/*!
 * Ecrit un \a item dans la BDD, l'index puis le détruit.
 *
 * L'item sera traité uniquement si il n'est pas déjà dans la BDD.
 */
void IO::write(Item * item)
{
	if (!ids.contains(item->get_id())) {
		if (item->get_langue() == "French")
			stemmer.stem(item, FRENCH);
		else if (item->get_langue() == "English")
			stemmer.stem(item, ENGLISH);

		HashDB db;
		//Ouverture de la BDD en écriture
		if (!db.open(path.toStdString(), HashDB::OWRITER | HashDB::OCREATE))
		{
			qCritical() << "Erreur à l'écriture dans la BDD :" << db.error().name();
		}
		db.set(item->get_id().toStdString(), item->toString().toStdString());
		
		if (!trainingMode)
			i->indexing(item);

		ids.append(item->get_id());
		saveDictionaries();
	}

	item->deleteLater();
}

void IO::classify()
{
	if (classifier != NULL) {
		QMap<QString, Item*> items = read(path);
		QList<Item*> items_fra;
		QList<Item*> items_eng;

		foreach(Item* i, items.values()) {
			if (i->get_predicted_category() == UNCATEGORIZED) {
				if (i->get_langue() == "French")
					items_fra.append(i);
				else if (i->get_langue() == "English")
					items_eng.append(i);
			}
		}

		classifier->classify(&items_fra, &fra);
		classifier->classify(&items_eng, &eng);

		HashDB db;
		if (!db.open(path.toStdString(), HashDB::OWRITER | HashDB::OCREATE))
		{
			qCritical() << "Erreur à l'écriture dans la BDD :" << db.error().name();
		}

		foreach(Item* item, items_fra)
			db.set(item->get_id().toStdString(), item->toString().toStdString());

		foreach(Item* item, items_eng)
			db.set(item->get_id().toStdString(), item->toString().toStdString());
	}
}

/*!
 * Lit la BDD contenue dans le fichier \a path.
 * 
 * Retourne la liste des items avec pour clé leur hash
 */
QMap<QString, Item*> IO::read(QString path)
{
	static int count = 0;
	count++;
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
	connect(p, SIGNAL(itemProcessed(Item*)), this, SLOT(write(Item*)));
	connect(p, SIGNAL(feedProcessed()), this, SLOT(classify()));
    parsers.append(p);
}

void IO::collectTrainingData(QString path)
{
	trainingMode = true;
	QFile f(path);
	f.open(QFile::ReadOnly);
	while (!f.atEnd()) {
		QString line = f.readLine();
		QStringList parts = line.split(';');
		Parser* p = new Parser(QUrl(parts[0]), parts[1].simplified(), this);
		connect(p, SIGNAL(itemProcessed(Item*)), this, SLOT(write(Item*)));
		parsers.append(p);
	}
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

void IO::loadDictionaries()
{
	QFile fr("fr.dico");
	if (fr.exists()) {
		fr.open(QFile::ReadOnly);
		QDataStream fr_stream(&fr);
		fr_stream >> fra;
		fr.close();
	}
	else
		fra.language() = Language::FRENCH;

	QFile en("en.dico");
	if (en.exists()) {
		en.open(QFile::ReadOnly);
		QDataStream en_stream(&en);
		en_stream >> eng;
		en.close();
	}
}

/*!
 * Charge les identifiants des items déjà présent dans la BDD et les enregistrent dans ids
 */
void IO::loadSavedIds()
{
	if (QFile(path).exists()) {
		QMap<QString, Item*> l = IO::read(path);
		foreach(Item * item, l)
		{
			ids.append(item->get_id());
		}
	}
}

void IO::saveDictionaries()
{
	QFile fr("fr.dico");
	fr.open(QFile::WriteOnly);
	QDataStream fr_stream(&fr);
	fr_stream << fra;
	fr.close();

	QFile en("en.dico");
	en.open(QFile::WriteOnly);
	QDataStream en_stream(&en);
	en_stream << eng;
	en.close();
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

	csv << "Id;";
	csv << "Titre;";
	csv << "Date;";
	csv << "Description;";
	csv << "Contenu;";
	csv << "Stems;";
	csv << "Langue;";
	csv << "Categorie;";
	csv << "Categorie predite;";
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

int IO::countItemSaved()
{
	HashDB db;
	if (!db.open(path.toStdString(), HashDB::OREADER))
	{
		qCritical() << "Erreur à la lecture dans la BDD :" << db.error().name();
	}
	return db.count();
}

Dictionnaire* IO::getDictionary(Language lang)
{
	switch (lang) {
	case FRENCH:
		return &fra;
	case ENGLISH:
		return &eng;
	default:
		return NULL;
	}
}

IO * IO::getInstance()
{
	if (path.isEmpty())
		return NULL;
	if (io == NULL)
	{
		io = new IO(path, training_data_fr, training_data_en);
	}
	return io;
}

void IO::deleteInstance()
{
	if (io != NULL) {
		delete io;
		io = NULL;
	}
}
