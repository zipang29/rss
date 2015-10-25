Nailya Bogroba - Guillaume Claudic - Louis Ormières
M2 - WMR
Sous projet Feed Collector

Installation et exécution :
Il n'y a pas d'installation à effectuer, le programme peut être lancé via le fichier parser.exe.
Pour lancer le programme, ouvrir un invité de commande sur Windows. Se placer dans le dossier de l'exécutable puis exécuter la commande comme ceci :

	parser.exe -f ListFluxRSS-v2.txt bdd.kch : cette commande lancera le parser avec pour liste de flux "ListFluxRSS-v2.txt" et comme base de données "bdd.kch".
	
	Options :
		-f, --fichier <chemin d'accès> : collecte les flux RSS désignés dans le fichier (une URL par ligne)
		-u, --url <URL> : URL d'un flux RSS à collecter (on ne peut spécifier qu'un seul flux via ce paramètre). Peut être utilisé pour tester le programme sur un flux.
		
	Arguments : 
		Base de données : Chemin d'accès au fichier HashDB (.kch), sera créé en cas de collection d'un ou plusieurs flux
		
Note importante : Si Tika renvoie des erreurs sur la sortie standard après que le programme ait déjà été lancé une première fois, veuillez vérifier 
que le processus Java correspondant à Tika lancé par la précédente exécution du parser n'est pas encore en cours d'exécution. Si c'est le cas, il faut le tuer.
Cela se produit car notre programme n'est pas fait pour être arrêté. Il est censé rester en fonction en permanance tel un serveur.