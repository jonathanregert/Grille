# **LA GRILLE**

### Prérequis
- Un ordinateur sous **Linux/Windows** ;
- **g++** installé et configuré ;

### Instructions de compilation
- Se rendre dans le dossier ***./src*** ;
- Ouvrir la console puis taper :
  - Sous Linux:
`g++ Application.cpp Grille.cpp -o {répertoire}{nom de l'éxécutable}.out`
  - Sous Windows:
`g++ Application.cpp Grille.cpp -o {répertoire}{nom de l'éxécutable}.exe`
*Par défaut le répertoire choisi est le dossier actuel*

### Comment utiliser le programme
- Se rendre dans le dossier où se trouve le fichier éxécutable ;
- Ouvrir la console puis taper le **nom du fichier éxécutable** suivi de son **extension** ;
- Taper dans la console le **chemin du fichier** contenant la grille à résoudre ;
*ex: ../instances/competition_01.txt*
- Le programme renverra un fichier *.txt* qui contient **la grille** obtenue et son **score** ;
- Le fichier renvoyé est situé dans le chemin indiqué par l'utilisateur à l'étape précedente et se nomme : ***resultat_{nom_du_fichier}.txt***  ;
- Différentes informations relatives à la grille seront affichées sur la console: **la grille obtenue**, **le score** et **le temps d'éxécution** ;