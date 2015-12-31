#include "Classifier.h"
#include <QFile>
#include <QRegularExpression>

const QString Classifier::weka_prog = "java";
const QStringList Classifier::weka_args = QStringList() << "-cp" << "weka.jar" << "weka.classifiers.bayes.NaiveBayes" << "-t" << "trainingData.arff" << "-T" << "testData.arff" << "-p" << "0";

Classifier::Classifier(QString trainingData_fra, QString trainingData_eng, QObject* parent) : QObject(parent)
{
	training_fra = trainingData_fra;
	training_eng = trainingData_eng;
	weka = NULL;
}

Classifier::~Classifier()
{
}

void Classifier::init(Dictionnaire* dico)
{
	dico->updateIdfs();
	QMap<QString, Item*> trainingItems;
	switch (dico->language()) {
	case FRENCH:
		if (trainingData_fr.isEmpty())
			trainingData_fr = IO::read(training_fra);
		trainingItems = trainingData_fr;
		break;
	case ENGLISH:
	default:
		if (trainingData_en.isEmpty())
			trainingData_en = IO::read(training_eng);
		trainingItems = trainingData_en;
	}

	qDebug() << "Creation arff d'entrainement";
	QFile trainingData("trainingData.arff");
	if (trainingData.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text)) {
		QTextStream stream(&trainingData);
		stream << "@relation test\n";

		qDebug() << "Creation liste des attributs";
		words = dico->getWords();
		foreach(const QString& word, words) {
			if (word.isEmpty() || word == " ")
				qDebug() << "mot=" << word;
			stream << "@attribute " << word << " numeric\n";
		}
		stream << "@attribute cate {";

		qDebug() << "Creation liste des categories";
		categories.clear();
		bool first = true;
		foreach(Item* item, trainingItems.values()) {
			if (!categories.contains(item->get_predicted_category())) {
				categories.append(item->get_predicted_category());

				if (!first)
					stream << "," << item->get_predicted_category();
				else {
					stream << item->get_predicted_category();
					first = false;
				}
			}
		}
		stream << "}\n\n";


		qDebug() << "Creation liste des donnees";
		stream << "@data\n";
		int count = 0;
		int max = trainingItems.size();
		foreach(Item* item, trainingItems.values()) {
			qDebug() << "Item" << count << "de" << max;
			first = true;
			foreach(const QString& word, words) {
				if (!first)
					stream << "," << dico->idf(word) * item->tf(word);
				else {
					stream << dico->idf(word) * item->tf(word);
					first = false;
				}
			}
			stream << "," << item->get_predicted_category() << "\n";
			count++;
			
			/*
			if (count == 10)
				break; //TODO: valeur de debug; à supprimer
				*/
		}

		qDebug() << "Donnees d'entrainement terminees";

		trainingData.close();
	}
	else{
		qCritical() << "Echec de l'ouverture du fichier" << trainingData.fileName();
	}
}

void Classifier::createTestFile(QList<Item*>* items, Dictionnaire* dico)
{
	qDebug() << "Creation des donnees de tests";
	QFile testData("testData.arff");
	if (testData.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
		QTextStream stream(&testData);
		stream << "@relation test\n";

		qDebug() << "Creation liste des attributs";
		words = dico->getWords();
		foreach(const QString& word, words) {
			stream << "@attribute " << word << " numeric\n";
		}
		stream << "@attribute cate {";

		qDebug() << "Creation liste des categories";
		bool first = true;
		foreach(const QString& category, categories) {
			if (!first)
				stream << "," << category;
			else {
				stream << category;
				first = false;
			}
		}
		stream << "}\n\n";

		qDebug() << "Creation liste des donnees";
		stream << "@data\n";
		int count = 0;
		int max = items->size();
		foreach(Item* item, *items) {
			qDebug() << "Item" << count << "de" << max;
			first = true;
			foreach(const QString& word, words) {
				if (!first)
					stream << "," << dico->idf(word) * item->tf(word);
				else {
					stream << dico->idf(word) * item->tf(word);
					first = false;
				}
			}
			stream << ",?\n";
			count++;
		}

		testData.close();
		qDebug() << "Creation fichier test termine";
	}
	else {
		qCritical() << "Impossible d'ouvrir le fichier" << testData.fileName();
	}
}

void Classifier::interrogateWeka(QList<Item*>* items)
{
	qDebug() << "Interrogation de weka";
	weka = new QProcess(this);
	weka->start(weka_prog, weka_args);
	weka->waitForFinished();

	QByteArray ret(weka->readAllStandardOutput());
	QString retString(ret);

	QStringList lines = retString.split('\n');
	for (int i = 5; i < lines.size() - 2; i++) {
		QStringList parts = lines[i].split(QRegularExpression("\\s{1,}"));
		(*items)[parts[1].toInt()-1]->set_predicted_category(parts[3].split(':')[1]);
		qDebug() << "Item n" << parts[1].toInt() - 1 << ":" << parts[3].split(':')[1];
	}
	qDebug() << "Classification terminee";
}

void Classifier::classify(QList<Item*>* items, Dictionnaire* dico)
{
	init(dico);
	createTestFile(items, dico);
	interrogateWeka(items);
}