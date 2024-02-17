INSTALLATION
=====================

1. Ouvrez un terminal (sur Windows : **`powershell`**) au répertoire où se trouve ce fichier (`(...)/termfm/exe`).
2. Copiez-collez les commandes correspondant à votre système d'exploitation et appuyez sur ENTRÉE.

Votre programme se trouve dans le dossier `(...)/termfm/exe`.


UNIX
---------------------
### INSTALLER
```bash
cp MakefileUnix ../src/Makefile;
cd ../src;
make; 
cd ../exe
```

### NETTOYER
```bash
rm termfm;
cd ../src;
make clean;
rm Makefile;
cd ../exe
```

### LANCER LE PROGRAMME
```bash
./termfm <fichier>
```

WINDOWS
---------------------
### INSTALLER
```powershell
copy MakefileWin "..\src\Makefile";
cd ..\src;
make;
cd ..\exe
```

### NETTOYER
```powershell
del termfm.exe;
cd ..\src;
make clean;
del Makefile;
cd ..\exe
```

### LANCER LE PROGRAMME
```powershell
.\termfm.exe <fichier>
```
