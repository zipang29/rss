#include "Stemmer.h"
#include <QRegularExpression>

/*!
 * \class Stemmer
 * \brief Classe faisant office d'interface avec la librairie de stemmage
 * \inmodule CLASSIFIER
 *
 * S'occupe de la découpe du contenu des items en liste de mots stemmés.
 *
 * Ne fonctionne que pour les items en francais ou en anglais.
 */

/*!
 * Crée un nouveau stemmer avec optionellement un objet \a parent pour simplifier la géstion mémoire.
 */
Stemmer::Stemmer(QObject* parent) : QObject(parent)
{
	english = sb_stemmer_new("english", "UTF_8");
	french = sb_stemmer_new("french", "UTF_8");
}

/*!
 * Détruit le stemmer
 */
Stemmer::~Stemmer()
{
	sb_stemmer_delete(english);
	sb_stemmer_delete(french);
}

/*!
 * Découpe le contenu de l'item \a i en liste de mots qui sont ensuite stemmés (par rapport à la langue \a lang) et sauvegardés dans ce même item.
 *
 * Ne fonctionne que pour le francais ou l'anglais.
 */
void Stemmer::stem(Item* i, Language lang)
{
	QStringList mots = i->get_contenu().split(' ');

	switch (lang) {
	case FRENCH:
		foreach(QString mot, mots) {
			if (!mot.isEmpty()){
				QString motTraite = mot.remove(QRegularExpression("[^a-zA-Z]"));
				const char* stem = (const char*)sb_stemmer_stem(french, (const sb_symbol*)motTraite.toStdString().c_str(), motTraite.size());
				QString stem_s_fr(stem);
				if (!stem_s_fr.isEmpty())
					i->add_word(stem_s_fr, FRENCH);
			}
		}
		break;
	case ENGLISH:
	default:
		foreach(QString mot, mots) {
			if (!mot.isEmpty()){
				QString motTraite = mot.remove(QRegularExpression("[^a-zA-Z]"));
				const char* stem = (const char*)sb_stemmer_stem(english, (const sb_symbol*)motTraite.toStdString().c_str(), motTraite.size());
				QString stem_s_en(stem);
				if (!stem_s_en.isEmpty())
					i->add_word(stem_s_en, ENGLISH);
			}
		}
	}
}
