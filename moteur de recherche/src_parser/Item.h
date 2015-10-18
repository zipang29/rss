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
	~Item();
	QString get_id();
	QString get_url_du_flux();
	QString get_url_de_la_page();
	QString get_titre();
	QString get_description();
	QString get_resume();
	QString get_langue();
    QString get_category();
	QDateTime get_date();
	
	void set_id(QString i);
	void set_url_du_flux(QString udf);
	void set_url_de_la_page(QString udp);
	void set_titre(QString t);
	void set_description(QString des);
	void set_resume(QString r);
	void set_langue(QString l);
    void set_category(QString c);
	void set_date(QDateTime d);

    QString toString();
    static Item * fromString(QString v);
    QString toHumanReadable();

private:
    QString url_du_flux, url_de_la_page, titre, description, resume, langue, category;
    QDateTime date;
    QString id;
};

#endif // ITEM_H
