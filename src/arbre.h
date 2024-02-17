/**
 * Definit la structure manipulee par le programme.
 * Il s'agir d'un arbre dont les noeuds representent
 * les fichiers et dossiers ainsi que leurs liens de parente.
 *
 * Y sont regroupees toutes les fonctions de manipulation interne de
 * cette structure.
 */

#ifndef ARBRE_H
#define ARBRE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

struct noeud;
struct liste_noeud;

struct noeud
{
    bool est_dossier;
    char nom[100];
    struct noeud *pere;
    struct noeud *racine;
    struct liste_noeud *fils;
};

struct liste_noeud
{
    struct noeud *no;
    struct liste_noeud *succ;
};

typedef struct noeud noeud;
typedef struct liste_noeud liste_noeud;

/** Cree un arbre vide, constitue uniquement de la racine */
noeud *init_arbre();
/** Supprimme l'arbre depuis sa racine */
void delete_arbre(noeud *racine);

/** Renvoie la racine de l'arbre donnee en argument */
noeud *get_arbre(noeud *element);

/** Cree un noeud seul */
noeud *new_noeud(char *nom, bool est_dossier, noeud *pere, noeud *racine);

/** Verifie les conditions pour creer un dossier ou fichier de "nom" dans dossier*/
void verification_nom(noeud *dossier, char *nom);

/** Ajoute le noeud comme fils du dossier passe en argument */
void add_fils(noeud *dossier, noeud *n);

/** Copie l'element 'element' dans le dossier 'sous_dossier' */
void cp_noeud(noeud *source, noeud *target,char *nom);
/** Copie recursivement les fils de la liste dans le sous_dossier */
void cpRec(liste_noeud *fils, noeud *sous_doss);

/** Deplace l'element 'element' dans le dossier 'sous_dossier' */
void mv_noeud(noeud *source, noeud *target,char *nom);
/** Deplace recursivement les fils de la liste dans le sous_dossier */
void mv_rec(liste_noeud *fils, noeud *sous_doss);

/** Supprime recursivement la liste fils. */
void rm_fils(liste_noeud *fils);
/** Supprime recursivement le noeud en argument. */
void rm_no(noeud *no);
#endif