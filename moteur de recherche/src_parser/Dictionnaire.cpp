#include "Dictionnaire.h"

int Dictionnaire::idGenerator = 0;

Dictionnaire::Dictionnaire()
{
	tfListUpdated = true;
}

void Dictionnaire::addWord(QString word)
{
	if (nbMots.contains(word))
		nbMots[word]++;
	else
		nbMots.insert(word, 1);
	id.insert(word, idGenerator);
	Dictionnaire::idGenerator++;
	tfListUpdated = false;
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
	double result = 0;
	
	if (tfList.contains(word))
	{
		if (!tfListUpdated)
			updateTfList();
		result = tfList[word];
	}

	return result;
}

/** 
* TODO a optimiser en utilisant une liste de mot à mettre à jour. Si la valeur maximum change, on doit mettre à jour toute la liste
*/
void Dictionnaire::updateTfList()
{
	tfList.clear();
	double result = 0;
	int max = 0;

	QMapIterator<QString, int> i(nbMots);
	while (i.hasNext())
	{
		i.next();
		max = qMax(max, i.value());
	}

	i.toFront();
	while (i.hasNext())
	{
		i.next();
		double tf = i.value() / (double)max;
		tfList.insert(i.key(), tf);
	}
}

Dictionnaire::~Dictionnaire()
{
}
