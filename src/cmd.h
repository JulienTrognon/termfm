#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "parse.h"
#include "arbre.h"

#ifndef CMD_H
#define CMD_H

void exec(int cmd, char **args, noeud **curr);

/** Affiche la liste des fils du dossier courant */
void ls(noeud *curr);
/** Change le dossier courant à celui indiqué par Path */
void cd(noeud **curr, char *path);
/** Affiche le chemin absolue du dossier courant */
void pwd(noeud *curr);
/** Ajoute le fichier comme fils du dossier */
noeud *touch(noeud *curr, char *nom);
/** Ajoute le dossier comme fils du dossier */
noeud *mkdir(noeud *curr, char *nom);
/** Supprime le fichier passé en argument */
void rm(noeud *curr, noeud *fichier);
void rm_fils(liste_noeud *fils);
void rm_no(noeud *no);
/** Copie l'élement 'element' dans le dossier 'sous_dossier' */
void cp(noeud * curr,noeud *source, char *path);
/** Déplace l'élement 'element' dans le dossier 'dossier' */

void mv(noeud *curr,noeud *source, char *path);
char *cpmv_getname(char *path, int *c);
noeud *cmpv_gettarget(noeud *curr, noeud *source, char *path_cpy);

void printRec(noeud *n, int decalage, noeud *courant);
/** Affiche l'arborescence en entier */
void print(noeud *curr);

int calc_nb_args(char **args);
bool right_amount_args(int cmd, char **args);
// renvoie true si le chemin envoie vers un noeud connu depuis le noeud courant
bool path_exists(char **path, noeud *curr);

#endif