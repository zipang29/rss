#include "Dictionnaire.h"


Dictionnaire::Dictionnaire()
{
}

void Dictionnaire::addWord(QString word)
{
	QMap<QString, int>::iterator i;
	for (i = nbMots.begin(); i != nbMots.end(); ++i)
	{
		if (i.key() == word)
		{
			i.value()++;
			return;
		}
	}
	nbMots.insert(word, 1);

	//Peut être simplifié par 
	//nbMots[word]++;
	//cf. http://doc.qt.io/qt-5/qmap.html#operator-5b-5d

	//TODO: Si nouveau mot, l'inserer dans la map id avec un nouvel id
}

double Dictionnaire::idf(QString mot)
{
	if (!nbMots.contains(mot))
		return -1.0;

	int ni = nbMots.value(mot);
	int N = id.size();
	return -log2(ni / (double)N);
}

/*
double Dictionnaire::tf(QString word)
{

}
*/

Dictionnaire::~Dictionnaire()
{
}
