Nailya Bogrova - Guillaume Claudic - Louis Ormi�res
M2 - WMR
Projet � RSS-Intelligence �
Sous projet �FEED-collector�

Ce projet utilise plusieurs librairies, pour plus d'information sur leurs licences, consultez le fichier licences.txt

Pr�requis � l'ex�cution :
	- Microsoft Windows Vista, 7, 8, 8.1 ou 10 (XP n'est pas support�)
	- Le package redistribuable Visual C++ pour Visual Studio 2013 32bit (vcredist_x86.exe disponible sur http://www.microsoft.com/fr-FR/download/details.aspx?id=40784)
	- Une JRE version 7 ou sup�rieur (tests r�alis�s sur la version 8) dont les ex�cutables figurent dans la variable d'environnement PATH

Ex�cution :
Le logiciel s'ex�cute via une invite de commandes Windows (cmd) ou un terminal MinGW (bash). Dans la suite, les exemples s'appliquerons pour une utilisation avec le cmd.

	- Placez-vous dans le dossier contenant l'ex�cutable parser.exe
	- Lancer "parser.exe -h" pour obtenir l'aide sur les diff�rents param�tres de lancements

Exemples :
"parser.exe -f ListFluxRSS-v2.txt bdd.kch" Lance le parser pour les flux RSS dont les adresses se trouvent dans le fichier ListFluxRSS-v2.txt (une par ligne). La base de donn�e sera sauvegard�e dans le fichier bdd.kch, s'il n'existe pas, ce fichier sera cr��, dans le cas contraire les donn�es seront ajout�es � celles d�j� existantes

"parser.exe -u http://www.france24.com/en/timeline/rss bdd.kch" Lance le parser pour le flux http://www.france24.com/en/timeline/rss et stocke les donn�es dans le fichier bdd.kch

"parser.exe -f ListFluxRSS-v2.txt -u http://www.france24.com/en/timeline/rss bdd.kch" Lance le parser pour le flux http://www.france24.com/en/timeline/rss ainsi que ceux contenus dans le fichier ListFluxRSS-v2.exe

"parser.exe bdd.kch" Liste le contenu de la base de donn�e (titre des items RSS uniquement)


Probl�mes connus :
Le parser lance lui-m�me un serveur Tika pour certaines op�rations de traitement des donn�es. Il arrive sur certains syst�mes que celui-ci ne se lance pas correctement via le parser. Si des messages d'erreur type "connexion refused" s'affiche lors du traitement de donn�es par Tika, lancez manuellement le serveur Tika via la commande "java -jar tika-server-1.10.jar" avec un cmd. Si le probl�me persiste, v�rifiez qu'un autre logiciel n'occupe pas d�j� le port 9998 requis par le serveur Tika.