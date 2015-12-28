#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <QObject>
#include <QString>
#include <BayesianClassifier.h>
#include "IO.h"

class Classifier : public QObject
{
Q_OBJECT
public:
	Classifier(QString trainingData_fra, QString trainingData_eng, QObject* parent = 0);
	~Classifier();

	void classify(QList<Item*>* items, Dictionnaire* dico);

private:
	BayesianClassifier* bc;
	QString training_fra;
	QString training_eng;

	QList<QString> categories;
	QList<QString> words;

	void init(QList<Item*>* items, Dictionnaire* dico);
};

#endif //CLASSIFIER_H