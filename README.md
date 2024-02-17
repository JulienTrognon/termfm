# Projet_C__MONROUSSEAU_TROGNON
# TERMFM - Terminal File Manager

*TERMFM* est un programme console qui se lance, avec un fichier donné en argument, et qui effectue des opérations sur une arborescence de fichiers interne.

Il est capable d'interpréter les lignes du fichier à la manière d'un bash, pour se déplacer dans l'arborescence, afficher le contenu d'un répertoire, copier coller un élément, le déplacer, le supprimer, afficher le chemin courant, l'arborescence entière, ou encore créer des fichiers et dossiers.

Les fichiers ne sont pas lisibles, ce sont juste des terminaisons, des feuilles de l'arbre.


## INSTALLATION

1. Ouvrez un terminal (sur Windows : **`powershell`**) au répertoire où se trouve ce fichier.
2. Copiez-collez les commandes correspondant à votre système d'exploitation et appuyez sur ENTRÉE.

Votre programme se trouve dans le dossier `src`.


UNIX
---------------------
### INSTALLER
```bash
cp exe/MakefileUnix src/Makefile;
cd src;
make;
```

### NETTOYER
En étant dans le dossier `src` :
```bash
make clean;
```

### LANCER LE PROGRAMME
En étant dans le dossier `src` :
```bash
./termfm <fichier>
```

WINDOWS
---------------------
### INSTALLER
```powershell
copy "exe\MakefileWin" "src\Makefile";
cd src;
make;
```

### NETTOYER
En étant dans le dossier `src` :
```powershell
make clean;
```

### LANCER LE PROGRAMME
En étant dans le dossier `src` :
```powershell
.\termfm.exe <fichier>
```


## BUGS

Nous avons décidé de ne pas réduire `mv` à faire `cp` puis `rm`. En conséquence, la commande `mv` ne fonctionne pas : elle peut déplacer l'élément souhaité seulement si la destination se toruve dans un sous-dossier du dossier courant.