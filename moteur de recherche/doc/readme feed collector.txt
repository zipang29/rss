Nailya Bogrova - Guillaume Claudic - Louis Ormières
M2 - WMR
Projet « RSS-Intelligence »
Sous projet «FEED-collector»

Ce projet utilise plusieurs librairies, pour plus d'information sur leurs licences, consultez le fichier licences.txt

Prérequis à l'exécution :
	- Microsoft Windows Vista, 7, 8, 8.1 ou 10 (XP n'est pas supporté)
	- Le package redistribuable Visual C++ pour Visual Studio 2013 32bit (vcredist_x86.exe disponible sur http://www.microsoft.com/fr-FR/download/details.aspx?id=40784 ou dans le dossier Binaires)
	- Une JRE version 7 ou supérieur (tests réalisés sur la version 8) dont les exécutables figurent dans la variable d'environnement PATH

Exécution :
Le logiciel s'exécute via une invite de commandes Windows (cmd) ou un terminal MinGW (bash). Dans la suite, les exemples s'appliquerons pour une utilisation avec le cmd.

	- Placez-vous dans le dossier contenant l'exécutable parser.exe
	- Lancer "parser.exe -h" pour obtenir l'aide sur les différents paramètres de lancement

Exemples :
"parser.exe -f ListFluxRSS-v2.txt bdd.kch" Lance le parser pour les flux RSS dont les adresses se trouvent dans le fichier ListFluxRSS-v2.txt (une par ligne). La base de donnée sera sauvegardée dans le fichier bdd.kch, s'il n'existe pas, ce fichier sera créé, dans le cas contraire les données seront ajoutées à celles déjà existantes

"parser.exe -u http://www.france24.com/en/timeline/rss bdd.kch" Lance le parser pour le flux http://www.france24.com/en/timeline/rss et stocke les données dans le fichier bdd.kch

"parser.exe -f ListFluxRSS-v2.txt -u http://www.france24.com/en/timeline/rss bdd.kch" Lance le parser pour le flux http://www.france24.com/en/timeline/rss ainsi que ceux contenus dans le fichier ListFluxRSS-v2.exe

"parser.exe bdd.kch" Liste le contenu de la base de donnée (titre des items RSS uniquement)

"parser.exe -c contenu.csv bdd.kch" Met en page les données de la base de donnée dbb.kch dans le fichier contenu.csv (sera remplacé si il existe déjà)


Problèmes connus :
Le parser lance lui-même un serveur Tika pour certaines opérations de traitement des données. Il arrive sur certains systèmes que celui-ci ne se lance pas correctement via le parser. Si des messages d'erreur type "connexion refused" s'affiche lors du traitement de données par Tika, lancez manuellement le serveur Tika via la commande "java -jar tika-server-1.10.jar" avec un cmd. Si le problème persiste, vérifiez qu'un autre logiciel n'occupe pas déjà le port 9998 requis par le serveur Tika.

Bien que puissant, la librairie Tika ne peut prendre en charge tous les types de documents, il arrive donc parfois que celui-ci affiche une erreur de type "Unsupported Media Type". L'item concerné aura donc la valeur "Sans contenu" dans son contenu