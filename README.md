# Projet SINF1252 : Jeu de dames

Description des fichiers constituant le projet :

  - dames.c : Contient les fonctions principales servant au bon fonctionnement du jeu.
  - dames.h : Fichier header de dames.c contenant ses fonctions principales
  - dames_intermediate.h : Fichier header de dames.c contenant ses fonctions intermédiaires
  - programme.c : Interface du jeu de dames
  - test.c : Contient une série de tests CUnit attestant le bon fonctionnement des fonctions principales de dames.c
  - test_intermediate.c : Contient une série de tests CUnit attestant le bon fonctionnement des fonctions intermédiaires de dames.c

> Les tests CUnit principaux et intermédiaires sont séparés pour faciliter l'utilisation de notre fichier test.c sur d'autres codes du projet.

### Version
1.0 : 11/03/2015

### Utilisation

Après avoir compilé le projet à l'aide de la commande :

```sh
$ make
```
Vous pouvez exécuter le jeu à l'aide de la commande :

```sh
$ ./programme
```
Pour exécuter des mouvements sur le jeu, la syntaxe est la suivante (respectivement pour une séquence et pour plusieurs séquences) :
> (x1,y1) to (x2,y2)

> (x1,y1) to (x2,y2) , (x2,y2) to (x3,y3) , ...

Les joueurs jouent chacun à son tour ; les blancs commencent. Exemple de début de partie :
```sh
$ (1,6) to (0,5)
$ (2,3) to (1,4)
$ (0,5) to (2,3)
...
```
Pour annuler n mouvement(s), entrer :
```sh
$ undo n
```
Pour quitter le jeu, entrer :
```sh
$ exit
```
Pour exécuter les tests CUnit (respectivement sur les fonctions principales et auxiliaires) :
```sh
$ ./test
$ ./test_intermediate
```
Auteurs
----

Anonyme

**11/03/2015**

