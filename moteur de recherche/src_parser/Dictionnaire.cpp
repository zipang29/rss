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

	int newMax = qMax(maxValueNbMots, nbMots[word]);
	if (newMax != maxValueNbMots)
	{
		maxValueNbMots = newMax;
		tfListUpdated = false;
	}
	else
		tfListToUpdate.push_back(word);

	id.insert(word, idGenerator);
	Dictionnaire::idGenerator++;
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
		if (!tfListUpdated || !tfListToUpdate.isEmpty())
			updateTfList();
		result = tfList[word];
	}

	return result;
}

void Dictionnaire::updateTfList()
{
	if (!tfListUpdated)
	{
		tfList.clear();

		QMapIterator<QString, int> i(nbMots);
		while (i.hasNext())
		{
			i.next();
			double tf = i.value() / (double)maxValueNbMots;
			tfList.insert(i.key(), tf);
		}
	}
	else if (!tfListToUpdate.isEmpty())
	{
		for (int i = 0; i < tfListToUpdate.size(); i++)
		{
			QString key = tfListToUpdate.at(i);
			tfList[key] = nbMots[key] / (double)maxValueNbMots;
		}
	}
	tfListUpdated = true;
	tfListToUpdate.clear();
}

Dictionnaire::~Dictionnaire()
{
}
