#pragma once

#include <QString>
#include <QList>
#include <QMap>
#include <QDataStream>
#include "Constantes.h"

class Dictionnaire
{
	public:
		Dictionnaire(Language lang = ENGLISH);
		void addWord(QString word);
		double idf(QString mot);
		void save(QString path);
		static Dictionnaire * load(QString path);

		short& language();
		QList<QString> getWords();
		~Dictionnaire();

		friend QDataStream& operator<<(QDataStream& data, const Dictionnaire& dico);
		friend QDataStream& operator>>(QDataStream& data, Dictionnaire& dico);
	private:
		QMap<QString, int> nbMots;// nombre d'item qui contient ce mot au moins une fois
		QMap<QString, int> id;

		int maxValueNbMots;
		short lang;

		static int idGenerator;
};

