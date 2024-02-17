#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "cmd.h"
#include "parse.h"
#include "arbre.h"


void exec(int cmd, char **args, noeud **curr) {
    switch (cmd) {
        case   1: ls(*curr); break;
        case   2: pwd(*curr); break;
        case   3: print(*curr); break;
        case  10: mkdir(*curr, args[0]); break;
        case  11: touch(*curr, args[0]); break;
        case  12: rm(*curr, get_noeud(*curr, args[0])); break;
        case  21:
            cp(*curr,get_noeud(*curr, args[0]), args[1]); break;
        case  22:
            mv(*curr, get_noeud(*curr, args[0]), args[1]); break;
        case 100: cd(curr, args[0]); break;
        default : perror("ERREUR CODE : num commande inconnue.");
    }
}

/** Affiche la liste des fils du dossier courant */
void ls(noeud *curr) {
    // printf("ls    : ");

    if (!curr->est_dossier) {
        printf("La commande ls ne peut être utiliser sur un ficher");
        exit(EXIT_FAILURE);
    }

    liste_noeud *f = curr->fils;
    while (f != NULL) {
        if (f->no->est_dossier) {
            printf("(D)");
        }

        printf("%s ", f->no->nom);
        f = f->succ;
    }
    printf("\n");
}
/** Change le dossier courant à celui indique par Path */
void cd(noeud **curr, char *path) {
    // printf("cd    : ");
    if (path==NULL) {
        *curr = (*curr)->racine;
        //printf("[(D)/%s]\n", (*curr)->nom);
        return;
    } 
    noeud * tmp = get_noeud(*curr, path);
    
    if (tmp->est_dossier == false) {
        printf("Le chemin d'arrive indique un fichier et cd doit être utilise avec un dossier.\n");
        exit(EXIT_FAILURE);
    }

    *curr = tmp;
    // printf("[(D)%s]\n", (*curr)->nom);
}
/** Affiche le chemin absolue du dossier courant */
void pwd(noeud *curr){
    // printf("pwd   : ");

    if (curr == curr->racine) {
        printf("/\n");
        return;
    }

    char **path = malloc(sizeof(char *));
    int len = 0;
    *(path) = malloc(sizeof(char) * strlen(curr->nom)+1);
    strcpy(path[len], curr->nom);

    noeud *tmp = curr->pere;
    for (len = 1; tmp != curr->racine; ++len) {
        path = realloc(path, sizeof(char *) * (len+1));
        path[len] = malloc(sizeof(char)*strlen(tmp->nom)+1);
        strcpy(path[len], tmp->nom);
        tmp = tmp->pere;
    }

    for (int i = len-1; i >= 0; i--) {
        printf("/%s", path[i]);
        free(path[i]);
    }

    printf("\n");

    free(path);
    path = NULL;
}
/** Ajoute le fichier comme fils du dossier */
noeud *touch(noeud *curr, char *nom) {
    // printf("touch : [%s]\n", nom);

    verification_nom(curr, nom);
    return new_noeud(nom, false, curr, curr->racine);
}
/** Ajoute le dossier comme fils du dossier */
noeud *mkdir(noeud *curr, char *nom) {
    // printf("mkdir : [(D)%s]\n", nom);

    verification_nom(curr, nom);
    return new_noeud(nom, true, curr, curr->racine);
}
/** Supprime le fichier passe en argument */
void rm(noeud *curr, noeud *target) { 
    // printf("rm   : [%s] \n", target->nom);

    if (target == target->racine) {
        printf("rm ne peut pas supprimer la racine.\n");
        exit(EXIT_FAILURE);
    }

    noeud *tmp = curr->pere;
    while (tmp != curr->racine) {
        if (tmp->nom == target->nom) {
            printf("rm ne peut pas supprimer un dossier parent au dossier courant.\n");
            exit(EXIT_FAILURE);
        }
        tmp = tmp->pere;
    }
    // free recursivement les fils du dossier que l'on veut rm
    if (target->fils != NULL) {
        //printf("\ntarget->fils != NULL");
        rm_fils(target->fils);
        free(target->fils);
        target->fils = NULL;
    }

    liste_noeud *list =target->pere->fils;
    // Le noeud est le premier des enfants de son parent
    if(list->no==target){
        //printf("cas1");
        target->pere->fils=target->pere->fils->succ;
        free(list->no);
        free(list);
        return;
    }
    liste_noeud *list2=list->succ;

    while(list2->no!= target){
        list=list->succ;
        list2=list->succ;
    }
    // Le noeud est au milieu de la liste des enfants de son parent
    //printf("cas2");
    list->succ=list2->succ;
    free(list2->no);
    free(list2);
    // printf("\n");
}
/** Copie l'element 'element' dans le dossier 'sous_dossier' */
void cp(noeud *curr,noeud *source, char *path) {
    // printf("cp : from %s to %s\n",source->nom,path);
    char *path_cpy = malloc(sizeof(char)*strlen(path));
    strcpy(path_cpy,path);
    
    int *c = malloc(sizeof(int));
    char *name = cpmv_getname(path, c);

    // printf("Nom : %s ",name);
    path_cpy[strlen(path)-*c]='\0';
    //printf("Pathcopy :%s\n",pathCpy);
    noeud *target = cmpv_gettarget(curr, source, path_cpy);
    
    cp_noeud(source,target,name);
    free(c);
    free(name);
}

/** Deplace l'element 'element' dans le dossier 'dossier' */
void mv(noeud *curr,noeud *source, char *path) { 
    printf("mv : from %s to %s\n",source->nom,path);

    char *path_cpy = malloc(sizeof(char)*strlen(path));
    strcpy(path_cpy,path);
    
    int *c = malloc(sizeof(int));
    char *name = cpmv_getname(path, c);

    noeud *target;
    if(strlen(path)==strlen(name)+1){
        target = curr->racine;
    }else {
        path_cpy[strlen(path)-*c]='\0';
        target = cmpv_gettarget(curr, source, path_cpy);
    }

    //printf("Nom : %s ",name);
    //printf("Pathcopy :%s\n",path_cpy);
    
    liste_noeud *list = source->pere->fils;
    // Le noeud est le seul enfant de son parent
    // Le noeud est le premier des enfants de son parent
    if(list->no==source){
        //printf("cas1");
        source->pere->fils=source->pere->fils->succ;
        mv_noeud(source,target,name);
        free(c);
        free(name);
        return;
    }
    liste_noeud *list2=list->succ;

    while(list2->no!= source){
        list=list->succ;
        list2=list->succ;
    }
    // Le noeud est au milieu ou a la fin de la liste des enfants de son parent
    //printf("cas2");
    list->succ=list2->succ;

    mv_noeud(source,target,name);
    free(c);
    free(name);
}

char *cpmv_getname(char *path, int *c) {
    *c = 1;
    int haveSlash=1;
    for (size_t i=strlen(path)-1;*(path+i)!='/';i--){
        (*c)++;
        if(i==0){
            *c -=1;
            haveSlash=0;
            break;
        }
        //printf("i:%ld - %c",i,*(path+i));
    }
    //printf("\nC :%d ",c);
    char *name = malloc(sizeof(char)*(*c)+1);
    strcpy(name,path+(strlen(path)-*c+haveSlash));
    if(!strcmp(name,"..")){
        printf("Le changement de nom ne peut pas etre .. ");
        exit(EXIT_FAILURE);
    }

    return name;
}
noeud *cmpv_gettarget(noeud *curr, noeud *source, char *path_cpy) {
    noeud *target;
    if (strcmp(path_cpy,"")!=0) {
        target = get_noeud(curr,path_cpy);
    } else {
        target = curr;
    }

    noeud *tmp = target;
    while(tmp!=curr->racine){
        if(!strcmp(tmp->nom,source->nom)){
            printf("La destination demandee est dans le sous arbre de la source.\n");
            exit(EXIT_FAILURE);
        }
        tmp=tmp->pere;
    }

    return target;
}
/** Affiche l'arborescence en entier */
void printRec(noeud *n, int decalage, noeud *courant) {
    liste_noeud *l = n->fils;
    for (size_t i = 0; i < decalage; i++) {
        printf("   ");
    }

    printf("|=> ");
    if (n->est_dossier) {
        printf("\033[1;36m");
        printf("(D)");
    }

    if (n == courant) {
        printf("\033[4;32m");
        printf("%s", n->nom);

        printf("\033[0;32m");
        printf("\033[1;32m");
        printf(" *\n");
    } else {
        printf("%s\n", n->nom);
    }
    printf("\033[0;34m");

    while (l != NULL) {
        printRec(l->no, decalage + 1, courant);
        l = l->succ;
    }
}

void print(noeud *curr) {
    //printf("print :\n");
    printf("---------------\n");

    liste_noeud *l = curr->racine->fils;
    if (curr == curr->racine) {
        printf("\033[1;36m");
        printf("\033[4;32m");
        printf("Racine");
        printf("\033[1;32m");
        printf(" *\n");

    } 
    printf("\033[0;34m");
    while (l != NULL) {
        printRec(l->no, 0, curr);
        l = l->succ;
    }
    printf("\033[0;0m");
    printf("---------------\n");
}

int calc_nb_args(char **args) {
    int i = 0;
    for (; args[i] != NULL; i++) {}
    return i;
}

bool right_amount_args(int cmd, char **args) {
    int nb_args = calc_nb_args(args);

    if (cmd == 100) {
        return nb_args >= 0 && nb_args <= 1;
    }

    return cmd / 10 == nb_args;
}

bool path_exists(char **path, noeud *curr) {
    return false;
}