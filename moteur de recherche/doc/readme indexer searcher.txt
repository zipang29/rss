Nailya Bogrova - Guillaume Claudic - Louis Ormières
M2 - WMR
Projet « RSS-Intelligence »
Sous projet «INDEXER-SEARCHER»

Ce projet utilise plusieurs librairies, pour plus d'information sur leurs licences, consultez le fichier licences.txt

Prérequis à l'exécution :
	- Microsoft Windows Vista, 7, 8, 8.1 ou 10 (XP n'est pas supporté)
	- Le package redistribuable Visual C++ pour Visual Studio 2013 32bit (vcredist_x86.exe disponible sur http://www.microsoft.com/fr-FR/download/details.aspx?id=40784 ou dans le dossier Binaires)
	- Une JRE version 7 ou supérieur (tests réalisés sur la version 8) dont les exécutables figurent dans la variable d'environnement PATH

Exécution :
Le logiciel s'exécute via une invite de commandes Windows (cmd) ou un terminal MinGW (bash). Dans la suite, les exemples s'appliquerons pour une utilisation avec le cmd.

	- Placez-vous dans le dossier contenant l'exécutable search.exe
	- Lancez "search.exe -h" pour obtenir l'aide sur les différents paramètres de lancement

La syntaxe de recherche est celle de Lucene, Cf. https://lucene.apache.org/core/2_9_4/queryparsersyntax.html pour plus d'informations.

Exemples :
"search bdd.kch.index" lance le logiciel de recherche en mode console, dans ce mode, seul les titres des articles correspondant aux critères de la recherche sont affichés

"search bdd.kch.index -g" lance le logiciel de recherche en mode graphique, dans ce mode, les titres, descriptions et liens des articles correspondant aux critères de la recherche sont affichés

A noter que dans tous les cas, la recherche s'effectue sur une image de l'index au temps du lancement du logiciel. Toute modification de l'index par le parser pendant quand le logiciel est lancé est ignoré par celui-ci. Redémarrez le logiciel de recherche pour prendre en compte les nouvelles entrées dans l'index.

Problèmes connus : 
En mode console, les caractères accentués ne s'affichent pas.

Pour plus d'information concernant le parser, referez-vous au fichier "readme feed collector.txt"