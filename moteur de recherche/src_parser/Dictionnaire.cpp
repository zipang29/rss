#include "Dictionnaire.h"
#include "IO.h"
#include <QFile>
/*!
 * \class Dictionnaire
 * \brief Classe gérant un dictionnaire pour une langue
 * \inmodule CLASSIFIER
 *
 * S'occupe de gérer un dictionnaire (pour une seule langue) et permet de calculer les idfs des mots. 
 */

int Dictionnaire::idGenerator = 0;

/*!
 * Constructeur prenant en paramètre la \a lang (français ou anglais)
 */
Dictionnaire::Dictionnaire(Language lang)
{
	this->lang = lang;
}

/*!
 * Ajoute un mot (\a word ) au dictionnaire. Le mot ne doit être passé qu'une fois par item.
 */
void Dictionnaire::addWord(QString word)
{
	nbMots[word]++;
}

/*!
 * Retourne l'idf d'un \a mot. Les idfs doivent avoir étés mis à jours avec la méthode updateIdfs() de préférence une fois pour toute
 * après que les mots aient tous étés ajoutés au dictionnaire.
 */
double Dictionnaire::idf(QString mot)
{
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
	int N = IO::getInstance()->countItemSaved(); //TODO: compter que les items de la langue du dico ?
	for (int i = 0; i < words.size(); i++)
	{
		int ni = nbMots.value(words.at(i));

		listIdfs[words.at(i)] = log(N / (double)ni); // nombre total d'item / nombre d'item qui contienne le mot au moins une fois
	}
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
 * Surcharge d'opérateur pour la sérilisation
 */
QDataStream& operator<<(QDataStream& data, const Dictionnaire& dico)
{
	data << dico.lang << dico.nbMots << dico.maxValueNbMots << dico.idGenerator;
	return data;
}

/*!
 * Surcharge d'opérateur pour la désérialisation
 */
QDataStream& operator>>(QDataStream& data, Dictionnaire& dico)
{
	data >> dico.lang >> dico.nbMots >> dico.maxValueNbMots >> dico.idGenerator;
	return data;
}
