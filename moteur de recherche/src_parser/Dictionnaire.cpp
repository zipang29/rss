#include "Dictionnaire.h"
#include "IO.h"
#include <QFile>

int Dictionnaire::idGenerator = 0;

/*!
* Constructeur prenant en paramètre la \a lang (français ou anglais)
*/
Dictionnaire::Dictionnaire(Language lang)
{
	this->lang = lang;
}

/*!
* Ajoute un mot au dictionnaire. Le mot ne doit être passé qu'une fois par item.
*/
void Dictionnaire::addWord(QString word)
{
	nbMots[word]++;
	id.insert(word, idGenerator);
	Dictionnaire::idGenerator++;
}

/*!
* Retourne l'idf d'un \a mot. Les idfs doivent avoir étés mis à jours avec la méthode updateIdfs() de préférence une fois pour toute
* après que les mots aient tous étés ajoutés au dictionnaire.
*/
double Dictionnaire::idf(QString mot)
{
	/*if (!nbMots.contains(mot))
		return -1.0;

	int ni = nbMots.value(mot);
	int N = IO::getInstance()->countItemSaved(); //TODO: compter que les items de la langue du dico
	
	return log(N / (double)ni); // nombre total d'item / nombre d'item qui contienne le mot au moins une fois*/
	if (!listIdfs.contains(mot))
		return -1.0;
	return listIdfs[mot];
}

/*!
* Met à jours les idf de tout les mots du dictionnaire
*/
void Dictionnaire::updateIdfs()
{
	listIdfs.clear();
	QList<QString> words = getWords();
	int N = IO::getInstance()->countItemSaved(); //TODO: compter que les items de la langue du dico
	for (int i = 0; i < words.size(); i++)
	{
		int ni = nbMots.value(words.at(i));

		listIdfs[words.at(i)] = log(N / (double)ni); // nombre total d'item / nombre d'item qui contienne le mot au moins une fois
	}
}

/*!
* Sauvegarde à l'emplacement \a path le dictionnaire. Un fichier path.nbmots.txt et path.id.txt seront créés
*/
void Dictionnaire::save(QString path)
{
	// Enregistrement de la QMap nbMots
	QFile fileNbMots(path + ".nbmots.txt");
	if (!fileNbMots.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qCritical() << "Impossible d'ouvrir le fichier " << path << ".nbmots.txt en écriture. Le fichier n'a pas été écrit";
		return;
	}
	QTextStream fluxNbMots(&fileNbMots);
	fluxNbMots.setCodec("UTF-8");

	QMapIterator<QString, int> i(nbMots);
	while (i.hasNext()) 
	{
		fluxNbMots << i.key() << " " << i.value() << endl;
	}
	fileNbMots.close();

	// Enregistrement de la QMap id
	QFile fileIds(path + ".id.txt");
	if (!fileIds.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qCritical() << "Impossible d'ouvrir le fichier " << path << ".id.txt en écriture. Le fichier n'a pas été écrit";
		return;
	}
	QTextStream fluxIds(&fileIds);
	fluxIds.setCodec("UTF-8");
	QMapIterator<QString, int> i2(id);
	while (i2.hasNext())
	{
		fluxIds << i2.key() << " " << i2.value() << endl;
	}
	fileIds.close();
}

/*!
* Charge un dictionnaire à partir du \a path indiqué. Le path doit être une adresse absolue ou relative avec le nom du dictionnaire sans les extensions de fichiers.
* .nbmots.txt et .id.txt sont automatiquement rajoutés au \a path.
*/
Dictionnaire * Dictionnaire::load(QString path)
{
	QFile fileNbMots(path + ".nbmots.txt");
	if (!fileNbMots.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		qCritical() << "Impossible d'ouvrir le fichier " << path << ".nbmots.txt en lecture. Le fichier n'a pas été lu.";
		return NULL;
	}
	QTextStream fluxNbMots(&fileNbMots);

	QFile fileIds(path + ".id.txt");
	if (!fileIds.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qCritical() << "Impossible d'ouvrir le fichier " << path << ".id.txt en lecture. Le fichier n'a pas été lu";
		return NULL;
	}
	QTextStream fluxIds(&fileIds);

	Dictionnaire * dico = new Dictionnaire();
	QString line;
	QRegExp reg("\\s+");
	while (!fluxNbMots.atEnd())
	{
		line = fluxNbMots.readLine();
		QStringList list = line.split(reg);
		QString key = list.at(0);
		int value = list.at(1).toInt();
		dico->nbMots.insert(key, value);
	}
	while (!fluxIds.atEnd())
	{
		line = fluxIds.readLine();
		QStringList list = line.split(reg);
		QString key = list.at(0);
		int value = list.at(1).toInt();
		dico->id.insert(key, value);
		Dictionnaire::idGenerator = qMax(Dictionnaire::idGenerator, value);
	}

	fileNbMots.close();
	fileIds.close();
	return dico;
}

/*!
* Retourne la langue du dictionnaire
*/
short& Dictionnaire::language()
{
	return lang;
}

/*!
* Retourne la liste des mots actuellement dans le dictionnaire.
*/
QList<QString> Dictionnaire::getWords()
{
	return nbMots.keys();
}

/*! 
* Destructeur
*/
Dictionnaire::~Dictionnaire()
{
}

/*!
* Surcharge d'opérateur pour la sérilisation
*/
QDataStream& operator<<(QDataStream& data, const Dictionnaire& dico)
{
	data << dico.lang << dico.nbMots << dico.id << dico.maxValueNbMots << dico.idGenerator;
	return data;
}

/*!
* Surcharge d'opérateur pour la désérialisation
*/
QDataStream& operator>>(QDataStream& data, Dictionnaire& dico)
{
	data >> dico.lang >> dico.nbMots >> dico.id >> dico.maxValueNbMots >> dico.idGenerator;
	return data;
}
