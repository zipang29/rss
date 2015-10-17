#include "Item.h"

/**
 * Constructeur
 */

Item::Item()
{
	
}

Item::~Item()
{
	
}

//Getters et setters

QString Item::get_id(){
  return id;
} 

void Item::set_id(QString i){
  id = i;
}

QString Item::get_url_du_flux(){
  return url_du_flux;
} 

void Item::set_url_du_flux(QString udf){
  url_du_flux = udf;
}
QString Item::get_url_de_la_page(){
  return url_de_la_page;
} 

void Item::set_url_de_la_page(QString udp){
  url_de_la_page = udp;
}
QString Item::get_titre(){
  return titre;
} 

void Item::set_titre(QString t){
  titre = t;
}
QString Item::get_description(){
  return description;
} 

void Item::set_description(QString des){
  description = des;
}
QString Item::get_resume(){
  return resume;
} 

void Item::set_resume(QString r){
  resume = r;
}

QString Item::get_langue(){
  return langue;
} 

void Item::set_langue(QString l){
  langue = l;
}

QString Item::get_category(){
  return category;
}

void Item::set_category(QString c){
  category = c;
}

QDateTime Item::get_date(){
  return date;
} 

void Item::set_date(QDateTime d){
  date = d;
}

QString Item::toString()
{
    QString ret = "";
    ret += this->url_du_flux + SEPARATOR;
    ret += this->url_de_la_page + SEPARATOR;
    ret += this->titre + SEPARATOR;
    ret += this->description + SEPARATOR;
    ret += this->resume + SEPARATOR;
    ret += this->langue + SEPARATOR;
    ret += this->category + SEPARATOR;
    ret += this->date.toString(Qt::ISODate) + SEPARATOR;

    return ret;
}

/**
 * Construit un item à partir d'une chaine
 * @param v La chaine pour construire l'item. Les éléments de la chaine doivent êtres séparés par le symbole SEPARATOR définir dans Constantes.h.\n
 *          L'ordre des champs suit l'ordre des attributs de la classe à savoir comme ceci : url_du_flux, url_de_la_page, titre, description, resume, langue, category, date
 * @return L'item construit à partir de la chaine. L'id (hash) n'est pas ajouté à l'item via cette méthode, il faut l'ajouter séparément. Si la chaine n'est pas valide l'item retourné sera vide.
 */
Item * Item::fromString(QString v)
{
    Item * it = new Item();
    QStringList list = v.split(SEPARATOR);
    if (list.size() < 8)
    {
        qWarning() << "La chaine n'est pas valide. Celle-ci contient moins de 8 éléments. L'item retourné est vide.";
        return it;
    }
    it->set_url_du_flux(list.at(0));
    it->set_url_de_la_page(list.at(1));
    it->set_titre(list.at(2));
    it->set_description(list.at(3));
    it->set_resume(list.at(4));
    it->set_langue(list.at(5));
    it->set_category(list.at(6));
    it->set_date(QDateTime::fromString(list.at(7), Qt::ISODate));

    return it;
}

QString Item::toHumanReadable()
{
    QString ret = "";
    ret += "======================================";
    ret += this->id + "\n";
    ret += "======================================";
    ret += "Url du flux : " + this->url_du_flux + "\n";
    ret += "Url de la page : " + this->url_de_la_page + "\n";
    ret += "Titre : " + this->titre + "\n";
    ret += "Description : " + this->description + "\n";
    ret += "Resume : " + this->resume + "\n";
    ret += "Langue : " + this->langue + "\n";
    ret += "Catégorie : " + this->category + "\n";
    ret += "Date : " + this->date.toString() + "\n";
}
