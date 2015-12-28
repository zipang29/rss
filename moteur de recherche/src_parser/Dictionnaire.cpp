#include "Dictionnaire.h"
#include "IO.h"
#include <QFile>

int Dictionnaire::idGenerator = 0;

Dictionnaire::Dictionnaire(Language lang)
{
	this->lang = lang;
}

/*!
*	Ajout un mot au dictionnaire. Le mot ne doit être passé qu'une fois par item.
*/
void Dictionnaire::addWord(QString word)
{
	nbMots[word]++;
	id.insert(word, idGenerator);
	Dictionnaire::idGenerator++;
}

double Dictionnaire::idf(QString mot)
{
	if (!nbMots.contains(mot))
		return -1.0;

	int ni = nbMots.value(mot);
	int N = IO::getInstance()->countItemSaved();
	
	return -log2(ni / (double)N); // nombre d'item qui contienne le mot au moint une fois / nombre total d'item
}

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

short& Dictionnaire::language()
{
	return lang;
}

QList<QString> Dictionnaire::getWords()
{
	return nbMots.keys();
}

Dictionnaire::~Dictionnaire()
{
}

QDataStream& operator<<(QDataStream& data, const Dictionnaire& dico)
{
	data << dico.lang << dico.nbMots << dico.id << dico.maxValueNbMots << dico.idGenerator;
	return data;
}

QDataStream& operator>>(QDataStream& data, Dictionnaire& dico)
{
	data >> dico.lang >> dico.nbMots >> dico.id >> dico.maxValueNbMots >> dico.idGenerator;
	return data;
}
