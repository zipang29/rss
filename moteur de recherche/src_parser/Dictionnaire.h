#pragma once

#include <QString>
#include <QMap>

class Dictionnaire
{
	public:
		Dictionnaire();
		void addWord(QString word);
		double idf(QString mot);
		double tf(QString word);
		~Dictionnaire();

	private:
		QMap<QString, int> nbMots;
		QMap<QString, int> id;
		QMap<QString, double> tfList;
		bool tfListUpdated;

		static int idGenerator;

		void updateTfList();
};

