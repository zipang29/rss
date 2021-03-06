#include "Item.h"
#include "IO.h"
#include <QRegularExpression>

/*!
 * \class Item
 * \brief Classe de stockage en mémoire d'Items RSS
 * \inmodule FEED_COLLECTOR
 */

/*!
 * Constructeur par défaut, tous les attributs sont vides
 */
Item::Item()
{
}

/*!
 * Constructeur de recopie, recopie les attributs de l'item \a other
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
 * Pour récupérer le hash id
 */
QString Item::get_id(){
  return id;
} 

/*!
 * Assigne le hash \a i à l'item
 */
void Item::set_id(QString i){
  id = i;
}

/*!
 * Pour récupérer l'url du flux
 */
QString Item::get_url_du_flux(){
  return url_du_flux;
} 

/*!
 * Définit l'\a udf (url du flux)
 */
void Item::set_url_du_flux(QString udf){
  url_du_flux = udf;
}

/*!
 * Retourne l'adresse de la page pointée par l'item
 */
QString Item::get_url_de_la_page(){
  return url_de_la_page;
} 

/*!
 * Définit l'\a udp (adresse de la page pointée par l'item)
 */
void Item::set_url_de_la_page(QString udp){
  url_de_la_page = udp;
}

/*!
 * Retourne le titre de l'item
 */
QString Item::get_titre(){
  return titre;
} 

/*!
 * Définit le \a titre de l'item
 */
void Item::set_titre(QString titre){
  this->titre = titre;
}

/*!
 * Retourne la description de l'item
 */
QString Item::get_description(){
  return description;
} 

/*!
 * Définit la \a description de l'item
 */
void Item::set_description(QString description){
	this->description = description;
}

/*!
 * Renvoie le contenu de la page pointée par l'item
 */
QString Item::get_contenu(){
	return contenu;
} 

/*!
 * Définit le \a contenu de la page pointée par l'item
 */
void Item::set_contenu(QString contenu){
	this->contenu = contenu;
}

/*!
 * Retourne la langue dans laquelle a été rédigé l'item
 */
QString Item::get_langue(){
  return langue;
} 

/*!
 * Définit la \a langue
 */
void Item::set_langue(QString langue){
	this->langue = langue;
}

/*!
 * Retourne la catégorie
 */
QString Item::get_category(){
  return category;
}

/*!
 * Définit la catégorie \a c de l'item
 */
void Item::set_category(QString c){
  category = c;
}

/*!
 * Retourne la catégorie prédite par le classifier ou définie manuellement si il s'agit d'un item d'entrainement.
 */
QString Item::get_predicted_category()
{
	return predicted_category;
}

/*!
 * Définit la catégorie prédite \a c
 */
void Item::set_predicted_category(QString c)
{
	predicted_category = c;
}

/*!
 * Retourne la date d'ajout de l'item
 */
QDateTime Item::get_date(){
  return date;
} 

/*!
 * Définit la \a date d'ajout de l'item au flux (indiqué dans le xml du flux)
 */
void Item::set_date(QDateTime date){
	this->date = date;
}

/*!
 * Ajout un mot stemmé (\a word) à la liste des mots de l'item, l'ajoute également au dictionnaire de la \a lang si il n'y est pas déjà.
 *
 * Les mots sont stockés dans une QMap qui compte leur occurences dans l'item.
 */
void Item::add_word(QString word, Language lang)
{
	word = word.remove('\n');
	if (lang != -1) {
		Dictionnaire* dico = IO::getInstance()->getDictionary(lang);
		if (!words.contains(word))
			dico->addWord(word);
	}

	words[word]++;
}

/*!
 * Permet de récupérer l'item sous forme de string. Les attributs sont séparés par la constante SEPARATOR ;@;
 */
QString Item::toString()
{
    QString ret = "";
    ret += this->url_du_flux + SEPARATOR;
    ret += this->url_de_la_page + SEPARATOR;
    ret += this->titre + SEPARATOR;
    ret += this->description + SEPARATOR;
	ret += this->contenu + SEPARATOR;
	foreach(const QString& word, words.keys()) {
		for (int i = 0; i < words[word]; i++)
			ret += word + ",";
	}
	ret += SEPARATOR;
    ret += this->langue + SEPARATOR;
    ret += this->category + SEPARATOR;
    ret += this->date.toString(Qt::ISODate) + SEPARATOR;
	ret += this->predicted_category + SEPARATOR;

    return ret;
}

/*!
 * Construit un item à partir d'une chaine
 *
 * - \a v : La chaine pour construire l'item. Les éléments de la chaine doivent être séparés par le symbole SEPARATOR (;@;) définit dans Constantes.h
 * L'ordre des champs suit l'ordre des attributs de la classe à savoir comme ceci : url_du_flux, url_de_la_page, titre, description, contenu, langue, liste de mots stemmés, category, date, catégorie prédite
 * 
 * Retourne : L'item construit à partir de la chaine. L'id (hash) n'est pas ajouté à l'item via cette méthode, il faut l'ajouter séparément. Si la chaine n'est pas valide l'item retourné sera vide.
 */
Item * Item::fromString(QString v)
{
    Item * it = new Item();
    QStringList list = v.split(SEPARATOR);
    if (list.size() < 10)
    {
        qWarning() << "La chaine n'est pas valide. Celle-ci contient moins de 10 éléments. L'item retourné est vide.";
        return it;
    }
    it->set_url_du_flux(list.at(0));
    it->set_url_de_la_page(list.at(1));
    it->set_titre(list.at(2));
    it->set_description(list.at(3));
	it->set_contenu(list.at(4));
    it->set_langue(list.at(6));

	short lang;
	if (it->get_langue() == "French")
		lang = FRENCH;
	else if (it->get_langue() == "English")
		lang = ENGLISH;
	else
		lang = -1;

	QStringList wordList = list.at(5).split(',', QString::SkipEmptyParts);
	foreach(QString word, wordList)
		it->words[word]++;

    it->set_category(list.at(7));
    it->set_date(QDateTime::fromString(list.at(8), Qt::ISODate));
	it->set_predicted_category(list.at(9));

    return it;
}

/*!
 * Retourne une string de l'item résumant celui-ci
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
    ret += "Langue : " + this->langue + "\n";
    ret += "Catégorie : " + this->category + "\n";
	ret += "Catégorie prédite : " + this->predicted_category + "\n";
    ret += "Date : " + this->date.toString() + "\n\n";
    return ret;
}

/*!
 * Ecrit l'\a item dans le flux de \a debug
 */
QDebug operator<<(QDebug debug, const Item& item)
{
	debug << item.toHumanReadable();
	return debug;
}

/*!
 * Renvoie le contenu de l'item sous forme de QString formatté au format CSV
 * (Id; Titre; Date; Description; Contenu; Liste de mots stemmés; Langue; Catégorie; Catégorie prédite; URL de la page; URL du flux)
 */
QString Item::toCSV()
{
	//Suppression des retours à la ligne et des points-virgule
	QRegularExpression reg("[\\s;]");

	QString tireEchappe = titre;
	tireEchappe.replace(reg, " ");

	QString descriptionEchappe = description;
	descriptionEchappe.replace(reg, " ");

	QString contenuEchappe = contenu;
	contenuEchappe.replace(reg, " ");

	QString csv;
	csv.append(id).append(";");
	csv.append(tireEchappe).append(";");
	csv.append(date.toString("dd-mm-yyyy HH:mm:ss")).append(";");
	csv.append(descriptionEchappe).append(";");
	csv.append(contenuEchappe).append(";");
	csv.append("{");
	foreach(const QString& word, words.keys()) {
		for (int i = 0; i < words[word]; i++)
			csv.append(word + ",");
	}
	csv.append("};");
	csv.append(langue).append(";");
	csv.append(category).append(";");
	csv.append(predicted_category).append(";");
	csv.append(url_de_la_page).append(";");
	csv.append(url_du_flux).append(";");

	return csv;
}

/*!
 * Retourne le tf en fonction d'un mot \a word. La mise à jour des tf est faite automatiquement si besoin.
 */
double Item::tf(QString word)
{
	double result = 0;

	if (tfList.isEmpty())
		updateTfList();

	if (tfList.contains(word))
	{
		result = tfList[word];
	}

	return result;
}

/*!
 * Met à jour la liste des tf
 */
void Item::updateTfList()
{
	tfList.clear();
	int sum = 0;
	foreach(int num, words.values()) {
		sum += num;
	}

	foreach(QString word, words.keys()) {
		tfList.insert(word, words[word] / (double)sum);
	}
}
