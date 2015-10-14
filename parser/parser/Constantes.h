#ifndef CONSTANTES
#define CONSTANTES
#include <QString>

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

