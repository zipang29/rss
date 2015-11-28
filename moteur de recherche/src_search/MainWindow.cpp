#include "MainWindow.h"
#include "SearchEngine.h"
#include <QPushButton>
#include <QMessageBox>

/*!
 * \class MainWindow
 * \brief Classe gérant la GUI du programme de recherche
 * \inmodule INDEXER-SEARCHER
 *
 * Permet de créer une interface permettant d'effectuer des recherches dans un index CLucene
 */

/*!
 * Constructeur prenant en paramètre \a index correspondant au chemin vers la BDD d'indexation.
 */
MainWindow::MainWindow(QString index)
{
	ui.setupUi(this);
	this->index = index;
	ui.display->setOpenExternalLinks(true);
	connect(ui.rechercher, SIGNAL(clicked()), this, SLOT(simpleQuery()));
}

/*!
 * Slot permettant d'effectuer une requête de recherche à partir de la barre de recherche de l'interface. 
 * 
 * Affiche les résultats de la recherche dans la zone de l'interface prévue à cet effet.
 */
void MainWindow::simpleQuery()
{
	ui.rechercher->setEnabled(false);
	QString query = ui.requete->text();
	if (query.isEmpty())
	{
		QMessageBox::warning(this, "Recherche vide", "La recherche n'a pas pu aboutir. Le champs de recherche est vide.");
	}
	else
	{
		SearchEngine * search = new SearchEngine(this->index);
		Hits * results = search->simpleQuery(query);
		if (results->length() == 0)
			ui.display->setHtml("<h1>Aucun resultat pour la recherche '" + query + "'</h1>");
		else
		{
			QString html;
			for (int i = 0; i < results->length(); i++)
			{
				Document doc = results->doc(i);
				Field * titreField = doc.getField("titre");
				QString titre = titreField->stringValue();
				Field * url_du_fluxField = doc.getField("url_du_flux");
				QString url_du_flux = url_du_fluxField->stringValue();
				Field * url_de_la_pageField = doc.getField("url_de_la_page");
				QString url_de_la_page = url_de_la_pageField->stringValue();

				Field * descriptionField = doc.getField("description");
				QString description = descriptionField->stringValue();

				html += "<h1>" + titre + "</h1><br>" + description + "<br>Flux : <a href='" + url_du_flux + "'>" + url_du_flux + "</a><br>Source : <a href='" + url_de_la_page + "'>" + url_de_la_page + "</a><br>";
			}
			ui.display->setHtml(html);
		}
		delete search;
	}
	ui.rechercher->setEnabled(true);
}
