Nailya Bogroba - Guillaume Claudic - Louis Ormi�res
M2 - WMR
Sous projet Feed Collector

Installation et ex�cution :
Il n'y a pas d'installation � effectuer, le programme peut �tre lanc� via le fichier parser.exe.
Pour lancer le programme, ouvrir un invit� de commande sur Windows. Se placer dans le dossier de l'ex�cutable puis ex�cuter la commande comme ceci :

	parser.exe -f ListFluxRSS-v2.txt bdd.kch : cette commande lancera le parser avec pour liste de flux "ListFluxRSS-v2.txt" et comme base de donn�es "bdd.kch".
	
	Options :
		-f, --fichier <chemin d'acc�s> : collecte les flux RSS d�sign�s dans le fichier (une URL par ligne)
		-u, --url <URL> : URL d'un flux RSS � collecter (on ne peut sp�cifier qu'un seul flux via ce param�tre). Peut �tre utilis� pour tester le programme sur un flux.
		
	Arguments : 
		Base de donn�es : Chemin d'acc�s au fichier HashDB (.kch), sera cr�� en cas de collection d'un ou plusieurs flux
		