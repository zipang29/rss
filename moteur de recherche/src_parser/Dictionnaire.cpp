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
}

double Dictionnaire::idf(QString mot)
{
	if (!nbMots.contains(mot))
		return -1.0;

	int ni = nbMots.value(mot);
	int N = id.size();
	return -log2(ni / (double)N);
}

double Dictionnaire::tf(QString word)
{

}

Dictionnaire::~Dictionnaire()
{
}
