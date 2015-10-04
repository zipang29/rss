#ifndef ITEM_H
#define ITEM_H

#include <QString>
#include <QDateTime>

class Item
{
public:
    Item();
	~Item();
	QString get_id();
	QString get_url_du_flux();
	QString get_url_de_la_page();
	QString get_titre();
	QString get_description();
	QString get_resume();
	QString get_langue();
	QDateTime get_date();
	
	void set_id(Qstring i);
	void set_url_du_flux(Qstring udf);
	void set_url_de_la_page(Qstring udp);
	void set_titre(Qstring t);
	void set_description(Qstring des);
	void set_resume(Qstring r);
	void set_langue(Qstring l);
	void set_date(QDateTime d);

private:
    QString url_du_flux, url_de_la_page, titre, description, resume, langue;
    QDateTime date;
    QString id;
};
#endif // ITEM_H
