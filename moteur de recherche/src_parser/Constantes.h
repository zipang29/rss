#ifndef CONSTANTES_H
#define CONSTANTES_H
#include <QString>

/**
  * Symbole de séparation pour l'enregistrement dans la BDD
  */
#define SEPARATOR ";@;"


/**
  * Valeurs par défauts des attributs de la classe items
  */

#define UNCATEGORIZED "Sans catégorie"

#define NO_DESCRIPTION "Pas de description"

#define NO_URL "Pas d'adresse"

#define NO_TITLE "Sans titre"

#define NO_CONTENT "Sans contenu"

#define UNDEFINED_LANGUAGE "Langue indéfinie"

#define UNKNOWN_LANGUAGE "Langue inconnue"

#define NO_ID "Pas de hash"

/**
  * Définition des constantes pour le parsing
  */
#define RSS "rss"

#define CHANNEL "channel"

#define LAST_BUILD_DATE "lastBuildDate"

#define ITEM "item"

#define TITLE "title"

#define CATEGORY "category"

#define LINK "link"

#define DESCRIPTION "description"

#define THUMBNAIL "thumbnail"

#define ENCLOSURE "enclosure"

#define GUID "guid"

#define PUB_DATE "pubDate"

#define SOURCE "source"

#define AUTHOR "author"

#define LANGUAGE "language"

QString getLanguageName(QString identifier);

#endif // CONSTANTES

