#include "Dictionnaire.h"
#include "IO.h"

int Dictionnaire::idGenerator = 0;

Dictionnaire::Dictionnaire()
{
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
	int N = IO::getInstance()->countItemSaved();;
	
	return -log2(ni / (double)N); // nombre d'item qui contienne le mot au moint une fois / nombre total d'item
}

Dictionnaire::~Dictionnaire()
{
}
