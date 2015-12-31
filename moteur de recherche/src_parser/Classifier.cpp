#include "Classifier.h"
#include <QFile>
#include <QRegularExpression>

/*!
 * \class Classifier
 * \brief Classe de classification d'items
 * \inmodule CLASSIFIER
 *
 * Classifie les items rss français et anglais dans diverses catégories. 
 * Ces catégories sont déterminées grâces aux données d'entrainement qui auront été précédement récupérées et stockées dans des BDD HashDB (.kch).
 * 
 * La classification se fait de manière séquencielle et peut en conséquence prendre quelques minutes en cas d'un grand nombre d'items à traiter, aussi bien dans la phase d'entrainement de l'algorithme
 * que dans le phase de classification.
 *
 * La classification se déroule ainsi :
 * \list
 *	\li Entrainement de l'algorithme
 *		\list
 *			\li Récupération des items d'entrainement
 *			\li Calcul des IFiDF de ces items et répertoriage des catégories
 *			\li Génération d'un fichier .arff (trainingData.arff)
 *		\endlist
 *	\li Classification des items
 *		\list
 *			\li Calcul des TFiDF des items
 *			\li Génération d'un fichier .arff (testData.arff)
 *			\li Interrogation de Weka en utilisant un classifieur Bayesien naif
 *			\li Traitement du retour de Weka
 *		\endlist
 * \endlist
 *
 * Due à la possibilité d'ajout d'un mot au dictionnaire à chaque nouvel item téléchargé, ce processus est réalisé à chaque lancement d'une classification.
 * (Actuellement, une classification à chaque fin de traitement d'un flux).
 *
 * Le jar de Weka doit se trouver dans le même dossier que l'exécutable et s'appeler \e weka.jar
 */

const QString Classifier::weka_prog = "java";
const QStringList Classifier::weka_args = QStringList() << "-cp" << "weka.jar" << "weka.classifiers.bayes.NaiveBayes" << "-t" << "trainingData.arff" << "-T" << "testData.arff" << "-p" << "0";

/*!
 * Crée un nouveau classifieur.
 *
 * Le classifieur utilisera lors de la phase d'entrainement les items des BDD HashDB \a trainingData_fra et \a trainingData_eng pour les items en français et anglais respectivement.
 *
 * Optionellement, un attribut \a parent peut être indiqué afin de faciliter la gestion mémoire.
 */
Classifier::Classifier(QString trainingData_fra, QString trainingData_eng, QObject* parent) : QObject(parent)
{
	training_fra = trainingData_fra;
	training_eng = trainingData_eng;
	weka = NULL;
}

/*!
 * Initialise le classifieur, procède à la génération du fichier .arff contenant les données d'entrainement.
 *
 * Se base sur le \a dico pour le calcul des TFiDF
 */
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

	qInfo() << "Creation .arff d'entrainement (" << trainingItems.size() << "items)";
	QFile trainingData("trainingData.arff");
	if (trainingData.open(QFile::WriteOnly | QFile::Truncate | QIODevice::Text)) {
		QTextStream stream(&trainingData);
		stream << "@relation test\n";

		words = dico->getWords();
		foreach(const QString& word, words) {
			stream << "@attribute " << word << " numeric\n";
		}
		stream << "@attribute cate {";

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

		stream << "@data\n";
		foreach(Item* item, trainingItems.values()) {
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
		}

		qInfo() << "Generation terminee";

		trainingData.close();
	}
	else{
		qCritical() << "Echec de l'ouverture du fichier" << trainingData.fileName();
	}
}

/*!
 * Crée le fichier .arff contenant les données à classifier pour les \a items et le \a dico passé en paramètre.
 */
void Classifier::createTestFile(QList<Item*>* items, Dictionnaire* dico)
{
	qInfo() << "Generation du fichier .arff d'items a classifier (" << items->size() << "items)";
	QFile testData("testData.arff");
	if (testData.open(QFile::WriteOnly | QFile::Truncate | QFile::Text)) {
		QTextStream stream(&testData);
		stream << "@relation test\n";

		words = dico->getWords();
		foreach(const QString& word, words) {
			stream << "@attribute " << word << " numeric\n";
		}
		stream << "@attribute cate {";

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

		stream << "@data\n";
		foreach(Item* item, *items) {
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
		}

		testData.close();
		qInfo() << "Generation terminee";
	}
	else {
		qCritical() << "Impossible d'ouvrir le fichier" << testData.fileName();
	}
}

/*!
 * Lance le traitement des deux fichiers .arff par weka, récupère le résultat et met à jours les \a items en conséquence.
 */
void Classifier::interrogateWeka(QList<Item*>* items)
{
	qInfo() << "Interrogation de weka";
	weka = new QProcess(this);
	weka->start(weka_prog, weka_args);
	weka->waitForFinished();

	QByteArray ret(weka->readAllStandardOutput());
	QString retString(ret);

	QStringList lines = retString.split('\n');
	for (int i = 5; i < lines.size() - 2; i++) {
		QStringList parts = lines[i].split(QRegularExpression("\\s{1,}"));
		(*items)[parts[1].toInt()-1]->set_predicted_category(parts[3].split(':')[1]);
	}
	qInfo() << "Classification terminee";
}

/*!
 * Lance le processus de classification pour les \a items donnés en utilisant le \a dico de mots fourni pour les calculs de TFiDF
 */
void Classifier::classify(QList<Item*>* items, Dictionnaire* dico)
{
	init(dico);
	createTestFile(items, dico);
	interrogateWeka(items);
}