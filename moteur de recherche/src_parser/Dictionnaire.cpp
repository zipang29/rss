#include "Dictionnaire.h"
#include "IO.h"
#include <QFile>

int Dictionnaire::idGenerator = 0;

Dictionnaire::Dictionnaire()
{
}

/*!
*	Ajout un mot au dictionnaire. Le mot ne doit �tre pass� qu'une fois par item.
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
	int N = IO::getInstance()->countItemSaved();;
	
	return -log2(ni / (double)N); // nombre d'item qui contienne le mot au moint une fois / nombre total d'item
}

void Dictionnaire::save(QString path)
{
	// Enregistrement de la QMap nbMots
	QFile fileNbMots(path + ".nbmots.txt");
	if (!fileNbMots.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		qCritical() << "Impossible d'ouvrir le fichier " << path << ".nbmots.txt en �criture. Le fichier n'a pas �t� �crit";
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
		qCritical() << "Impossible d'ouvrir le fichier " << path << ".id.txt en �criture. Le fichier n'a pas �t� �crit";
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

Dictionnaire::~Dictionnaire()
{
}
