#pragma once

#include <QString>
#include <QList>
#include <QMap>

class Dictionnaire
{
	public:
		Dictionnaire();
		void addWord(QString word);
		double idf(QString mot);
		void save(QString path);
		~Dictionnaire();

	private:
		QMap<QString, int> nbMots;// nombre d'item qui contient ce mot au moins une fois
		QMap<QString, int> id;

		static int idGenerator;
};

