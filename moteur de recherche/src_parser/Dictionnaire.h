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
		double tf(QString word);
		~Dictionnaire();

	private:
		QMap<QString, int> nbMots;
		QMap<QString, int> id;
		QMap<QString, double> tfList;

		QList<QString> tfListToUpdate;
		int maxValueNbMots;

		bool tfListUpdated;

		static int idGenerator;

		void updateTfList();
};

