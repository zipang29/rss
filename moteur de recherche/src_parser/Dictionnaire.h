#pragma once

#include <QString>
#include <QMap>

class Dictionnaire
{
	public:
		Dictionnaire();
		void addWord(QString word);
		double idf(QString mot);
		~Dictionnaire();

	private:
		QMap<QString, int> nbMots;
		QMap<QString, int> id;
};

