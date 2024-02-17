#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "arbre.h"
#include "cmd.h"

/** Cree un arbre vide, constitue uniquement de la racine */
noeud *init_arbre() {
    noeud *racine = malloc(sizeof(noeud));
    racine->est_dossier = true;
    strcpy(racine->nom, ""); // modif temporaire
    racine->pere = racine;
    racine->racine = racine;
    racine->fils = NULL;
    return racine;
}

/** Renvoie la racine de l'arbre donnee en argument */
noeud *get_arbre(noeud *element) {
    return element->racine;
}

/** Cree un noeud seul */
noeud *new_noeud(char *nom, bool est_dossier, noeud *pere, noeud *racine) {
    noeud *n = malloc(sizeof(noeud));

    n->est_dossier = est_dossier;
    n->pere = pere;
    n->racine = racine;
    n->fils = NULL;
    strcpy(n->nom, nom);

    add_fils(pere, n);

    return n;
}

/** Verifie les conditions pour creer un dossier ou fichier de "nom" dans dossier*/
void verification_nom(noeud *dossier, char *nom) {
    /* Verifications du nom et des conflits potentiels
     * Verification de la taille du nom */
    size_t len = strlen(nom);
    if (len > 99 || nom == NULL || len <= 0) {
        printf("Nom invalide lors du touch");
        exit(EXIT_FAILURE); // ERROR
    }

    // Verification des caractères alpha-numerique
    for (size_t i = 0; i < len; i++) {
        if (!isalnum(*(nom + i))) {
            printf("Caractère non alpha-numerique lors du touch");
            exit(EXIT_FAILURE); // ERROR
        }
    }
}

/** Ajoute le noeud comme fils du dossier passe en argument */
void add_fils(noeud *dossier, noeud *n) {
    liste_noeud *list = dossier->fils;

    // Verifie si le pere a des enfants
    if (list == NULL) {
        dossier->fils = malloc(sizeof(liste_noeud));
        dossier->fils->no = n;
        dossier->fils->succ = NULL;
        n->pere = dossier;
    } else {
        // Verifie que le nom soit disponible
        while (list->succ != NULL) {
            if (!strcmp(n->nom, list->no->nom)) {
                // Affiche une erreur si le nom est dejà present
                printf("Le nom du fichier est dejà pris.\n");
                exit(EXIT_FAILURE);
            }
            list = list->succ;
        }
        if (!strcmp(n->nom, list->no->nom)) {
                // Affiche une erreur si le nom est dejà present
                printf("Le nom du fichier est dejà pris.\n");
                exit(EXIT_FAILURE);
        }

        
        liste_noeud *nouveau = malloc(sizeof(liste_noeud));
        nouveau->no = n;
        nouveau->succ = NULL;
        list->succ = nouveau;
        n->pere = dossier;
    }
}

void cp_noeud(noeud *source, noeud *target,char *nom) {
    if (source == NULL || target == NULL) {
        printf("Le fichier donne en argument est null");
        exit(EXIT_FAILURE);
    }

    if (!source->est_dossier) {
        touch(target, nom);
        return;
    }

    noeud *sous_doss = mkdir(target, nom);
    if (source->fils != NULL) {
        cpRec(source->fils, sous_doss);
    }
}
/** Copie recursivement les fils de la liste dans le sous_dossier */
void cpRec(liste_noeud *fils, noeud *sous_doss) {
    // copie recursivement le contenu de la liste fils dans le dossier sous_doss.
    cp_noeud(fils->no, sous_doss,fils->no->nom);
    if (fils->succ != NULL) {
        cpRec(fils->succ, sous_doss);
    }
}

void rm_fils(liste_noeud *fils) {
    //printf("\nrm_fils : [%s] ", fils->no->nom);
    if (fils->succ == NULL) {
        //printf("\nrm_fils : fils->succ == NULL");
        rm_no(fils->no);
        //printf("removed");
        return;
    }
    rm_fils(fils->succ);
    rm_no(fils->no);
}
void rm_no(noeud *no) {
    //printf("\nrm_no : [%s] ", no->nom);
    if (no->fils != NULL) {
        //printf("no->fils!=NULL");
        rm_fils(no->fils);
        free(no->fils);
        no->fils=NULL;
    }
    
    //printf("\n[%s] deleted !", no->nom);

    free(no);
}

void mv_noeud(noeud *source, noeud *target,char *nom) {
    if (source == NULL || target == NULL) {
        printf("Le fichier donné en argument est null");
        exit(EXIT_FAILURE);
    }

    // if (!source->est_dossier) {
    //     add_fils(target,source);
    //     // débranchement + free de source sur son père
    //     return;
    // }

    // noeud *sous_doss = mkdir(target, nom);
    add_fils(target,source);
}
void mv_rec(liste_noeud *fils, noeud *sous_doss) {
    mv_noeud(fils->no, sous_doss,fils->no->nom);
    if (fils->succ != NULL) {
        mv_rec(fils->succ, sous_doss);
    }
}