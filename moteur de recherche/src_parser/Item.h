#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QDateTime>
#include "Constantes.h"
#include <QDebug>

class Item : public QObject
{
	Q_OBJECT

public:
    Item();
	Item(const Item& other);
	QString get_id();
	QString get_url_du_flux();
	QString get_url_de_la_page();
	QString get_titre();
	QString get_description();
	QString get_contenu();
	QString get_langue();
    QString get_category();
	QDateTime get_date();
	
	void set_id(QString i);
	void set_url_du_flux(QString udf);
	void set_url_de_la_page(QString udp);
	void set_titre(QString t);
	void set_description(QString des);
	void set_contenu(QString c);
	void set_langue(QString l);
    void set_category(QString c);
	void set_date(QDateTime d);
	void add_word(QString word);

    QString toString();
    static Item * fromString(QString v);
    QString toHumanReadable() const;
	friend QDebug operator<<(QDebug debug, const Item& item);
	QString toCSV();

	double tf(QString word);

private:
    QString url_du_flux = NO_URL, url_de_la_page = NO_URL, titre = NO_TITLE, description = NO_DESCRIPTION, contenu = NO_CONTENT, langue = UNDEFINED_LANGUAGE, category = UNCATEGORIZED;
    QDateTime date;
    QString id = NO_ID;
	QList<QString> words;

	QMap<QString, int> tfList;

	void updateTfList();
};

#endif // ITEM_H