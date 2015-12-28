#include "Classifier.h"

Classifier::Classifier(QString trainingData_fra, QString trainingData_eng, QObject* parent) : QObject(parent)
{
	training_fra = trainingData_fra;
	training_eng = trainingData_eng;
	bc = NULL;
}

Classifier::~Classifier()
{
	if (bc != NULL)
		delete bc;
}

void Classifier::init(QList<Item*>* items, Dictionnaire* dico)
{
	qDebug() << "Initialisation du classifieur";
	QMap<QString, Item*> trainingItems;
	switch (dico->language()) {
	case FRENCH:
		trainingItems = IO::read(training_fra);
		break;
	case ENGLISH:
	default:
		trainingItems = IO::read(training_eng);
	}

	words = dico->getWords();
	categories.clear();
	std::vector<Domain> domains(words.size()+1);

	//Création des domaines
	qDebug() << "Création des domaines";
	for (int i = 0; i < words.size(); i++) {
		domains[i].setMin(0);
		domains[i].setMax(trainingItems.size() + items->size());
		domains[i].setNumberOfValues(trainingItems.size() + items->size() - 1);
	}

	//Définition du domaine pour les categories
	qDebug() << "Définition du domaine pour les categories";
	foreach(Item* i, trainingItems.values()) {
		if (!categories.contains(i->get_predicted_category()))
			categories.append(i->get_predicted_category());
	}
	domains[domains.size() - 1].setMin(0);
	domains[domains.size() - 1].setMax(categories.size()-1);
	domains[domains.size() - 1].setNumberOfValues(categories.size() - 1);

	//Création du classifier Bayesien et apprentissage
	qDebug() << "Création du classifier Bayesien et apprentissage";
	if (bc != NULL) {
		qDebug() << "Supression ancien classifier";
		delete bc;
		qDebug() << "Suppression réussie";
	}
	bc = new BayesianClassifier(domains);
	int count = 0;
	foreach(Item* i, trainingItems.values()) {
		qDebug() << "Apprentissage item" << count;
		RawTrainingData training_data(domains.size());
		for (int j = 0; j < words.size(); j++) {
			training_data[j] = i->tf(words[j]) * dico->idf(words[j]);
		}
		training_data[training_data.size() - 1] = categories.indexOf(i->get_predicted_category());

		bc->addRawTrainingData(training_data);
		count++;
	}
	qDebug() << "Fin apprentissage";
}

void Classifier::classify(QList<Item*>* items, Dictionnaire* dico)
{
	qDebug() << "Lancement de la classification";
	init(items, dico);

	qDebug() << "Lancement de la catégorisation, nbitems=" << items->size();
	foreach(Item* i, *items) {
		qDebug() << "catégorisation de" << i->get_id();
		std::vector<float> input(words.size());
		for (int j = 0; j < words.size(); j++) {
			input[j] = dico->idf(words[j]) * i->tf(words[j]);
		}
		i->set_predicted_category(categories[bc->calculateOutput(input)]);
		qDebug() << "catégorisé en" << i->get_predicted_category();
	}
}