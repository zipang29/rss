#include "Item.h"

Item::Item()
{
	
}

Item::~Item()
{
	
}

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

QDateTime Item::get_date(){
  return date;
} 

void Item::set_date(QDateTime d){
  date = d;
}


