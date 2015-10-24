#include "Item.h"

/*!
 * \brief Item::Item Constructeur
 */
Item::Item()
{
}

/*!
 * \brief Item::Item Constructeur de recopie
 * \param other L'item à recopier
 */
Item::Item(const Item& other)
{
	url_du_flux = other.url_du_flux;
	url_de_la_page = other.url_de_la_page;
	titre = other.titre;
	description = other.description;
	contenu = other.contenu;
	langue = other.langue;
	category = other.category;
	date = other.date;
	id = other.id;
}

/*!
 * \brief Item::~Item Destructeur
 */
Item::~Item()
{
	
}

/*!
 * \brief Item::get_id Pour récupérer le hash id
 * \return L'id
 */
QString Item::get_id(){
  return id;
} 

/*!
 * \brief Item::set_id Assigne le hash id à l'item
 * \param i Le nouveau hash
 */
void Item::set_id(QString i){
  id = i;
}

/*!
 * \brief Item::get_url_du_flux Pour récupérer l'url du flux
 * \return L'url du flux
 */
QString Item::get_url_du_flux(){
  return url_du_flux;
} 

/*!
 * \brief Item::set_url_du_flux Définit l'url du flux
 * \param udf L'url du flux à assigner
 */
void Item::set_url_du_flux(QString udf){
  url_du_flux = udf;
}

/*!
 * \brief Item::get_url_de_la_page Retourne l'adresse de la page
 * \return L'adresse de la page
 */
QString Item::get_url_de_la_page(){
  return url_de_la_page;
} 

/*!
 * \brief Item::set_url_de_la_page Définit l'adresse de la page
 * \param udp L'adresse de la page
 */
void Item::set_url_de_la_page(QString udp){
  url_de_la_page = udp;
}

/*!
 * \brief Item::get_titre Retourne le titre de l'item
 * \return Le titre de l'item
 */
QString Item::get_titre(){
  return titre;
} 

/*!
 * \brief Item::set_titre Définit le titre de l'item
 * \param t Le titre à assigned à l'item
 */
void Item::set_titre(QString t){
  titre = t;
}

/*!
 * \brief Item::get_description
 * \return
 */
QString Item::get_description(){
  return description;
} 

/*!
 * \brief Item::set_description Définit la description de l'item
 * \param des La description de l'item
 */
void Item::set_description(QString des){
  description = des;
}

/*!
 * \brief Item::get_contenu Renvoie le contenu correspondant à l'item
 * \return Le contenu correspondant à l'item
 */
QString Item::get_contenu(){
	return contenu;
} 

/*!
 * \brief Item::set_contenu Définit le contenu correspondant à l'item
 * \param c Le contenu
 */
void Item::set_contenu(QString c){
	contenu = c;
}

/**
 * @brief Item::get_langue Retourne la langue dans laquelle a été rédigé l'item
 * @return La langue détecté de l'item
 */
QString Item::get_langue(){
  return langue;
} 

/*!
 * \brief Item::set_langue Définit la langue
 * \param l La langue
 */
void Item::set_langue(QString l){
  langue = l;
}

/*!
 * \brief Item::get_category Retourne la catégorie
 * \return La catégorie
 */
QString Item::get_category(){
  return category;
}

/*!
 * \brief Item::set_category Définit la catégorie
 * \param c La catégorie
 */
void Item::set_category(QString c){
  category = c;
}

/*!
 * \brief Item::get_date Retourne la date d'ajout de l'item
 * \return La date d'ajout
 */
QDateTime Item::get_date(){
  return date;
} 

/*!
 * \brief Item::set_date Définit la date d'ajout de l'item au flux (indiqué dans le xml du flux)
 * \param d La date d'ajout
 */
void Item::set_date(QDateTime d){
  date = d;
}

/*!
 * \brief Item::toString Permet de récupérer l'item sous forme de string. Les attributs sont séparés par la constante SEPARATOR
 * \return L'item sous forme de string
 */
QString Item::toString()
{
    QString ret = "";
    ret += this->url_du_flux + SEPARATOR;
    ret += this->url_de_la_page + SEPARATOR;
    ret += this->titre + SEPARATOR;
    ret += this->description + SEPARATOR;
	ret += this->contenu + SEPARATOR;
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
	it->set_contenu(list.at(4));
    it->set_langue(list.at(5));
    it->set_category(list.at(6));
    it->set_date(QDateTime::fromString(list.at(7), Qt::ISODate));

    return it;
}

/*!
 * \brief Item::toHumanReadable Retourne une string de l'item résumant celui-ci
 * \return Une chaine affichable sur la sortie standart (par exemple)
 */
QString Item::toHumanReadable() const
{
    QString ret = "";
    ret += "======================================\n";
    ret += this->id + "\n";
    ret += "======================================\n";
    ret += "Url du flux : " + this->url_du_flux + "\n";
    ret += "Url de la page : " + this->url_de_la_page + "\n";
    ret += "Titre : " + this->titre + "\n";
    //ret += "Description : " + this->description + "\n";
    //ret += "Contenu : " + this->contenu + "\n";
    ret += "Langue : " + this->langue + "\n";
    ret += "Catégorie : " + this->category + "\n";
    ret += "Date : " + this->date.toString() + "\n\n";
    return ret;
}

/*!
 * \brief operator << Ecrit dans le flux de \a debug l'itam
 * \param debug Le QDebug dans lequel on veut écrire
 * \param item L'item à afficher
 * \return L'objet QDebug Le QDebug modifié
 */
QDebug& operator<<(QDebug& debug, const Item& item)
{
	debug << item.toHumanReadable();
	return debug;
}
