#ifndef STEMMER_H
#define STEMMER_H

#include "Item.h"
#include "libstemmer.h"

class Stemmer : public QObject
{
Q_OBJECT
private:
	sb_stemmer* english;
	sb_stemmer* french;
public:
	enum Language {
		ENGLISH,
		FRENCH
	};

	Stemmer(QObject* parent = 0);
	~Stemmer();

	void stem(Item* i, Language lang = ENGLISH);
};

#endif // STEMMER_H