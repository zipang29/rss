#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <QObject>
#include <QString>
#include <QProcess>
#include "IO.h"

class Classifier : public QObject
{
Q_OBJECT
public:
	Classifier(QString trainingData_fra, QString trainingData_eng, QObject* parent = 0);
	~Classifier();

	void classify(QList<Item*>* items, Dictionnaire* dico);

private:
	static const QString weka_prog;
	static const QStringList weka_args;

	QProcess* weka;

	QString training_fra;
	QString training_eng;

	QList<QString> categories;
	QList<QString> words;

	QMap<QString, Item*> trainingData_fr;
	QMap<QString, Item*> trainingData_en;

	void init(Dictionnaire* dico);
	void createTestFile(QList<Item*>* items, Dictionnaire* dico);
	void interrogateWeka(QList<Item*>* items);
};

#endif //CLASSIFIER_H