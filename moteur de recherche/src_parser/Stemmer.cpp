#include "Stemmer.h"

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
		foreach(const QString& mot, mots) {
			if (!mot.isEmpty()){
				const char* stem = (const char*)sb_stemmer_stem(french, (const sb_symbol*)mot.toStdString().c_str(), mot.size());
				QString stem_s_fr(stem);
				i->add_word(stem_s_fr);
			}
		}
		break;
	case ENGLISH:
	default:
		foreach(const QString& mot, mots) {
			if (!mot.isEmpty()){
				const char* stem = (const char*) sb_stemmer_stem(english, (const sb_symbol*) mot.toStdString().c_str(), mot.size());
				QString stem_s_en(stem);
				i->add_word(stem_s_en);
			}
		}
	}
}
