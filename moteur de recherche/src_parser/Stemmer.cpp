#include "Stemmer.h"
#include <QRegularExpression>

Stemmer::Stemmer(QObject* parent) : QObject(parent)
{
	english = sb_stemmer_new("english", "UTF_8");
	french = sb_stemmer_new("french", "UTF_8");
}

Stemmer::~Stemmer()
{
	sb_stemmer_delete(english);
	sb_stemmer_delete(french);
}

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
