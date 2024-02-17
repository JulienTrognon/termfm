#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "parse.h"
#include "arbre.h"

/** Sépare la line et la renvoie en un tableau de string */
char **parse(char *restrict line, char *separator) {
    // 1er mot
    int c = 0;
    char **parse = malloc(sizeof(char *));
    *(parse) = strtok(line, separator);

    if(*parse==NULL){
        return NULL;
    }

    // mots suivants
    for (++c; parse[c-1] != NULL; ++c) {
        parse = realloc(parse, (c+1) * sizeof(char *));
        parse[c] = strtok(NULL, separator);
    }

    // valeur sentinelle
    parse = realloc(parse, (c+1) * sizeof(char *));
    parse[c] = strtok(NULL, separator);

    return parse;
}

/** En fonction d'un noeud et d'un chemin, renvoie le noeud qui lui correspond. */
noeud *get_noeud(noeud *n,char *path){
    //printf("%s",path);
    // On vérifie que le chemin n'est pas la racine
    if (!strcmp(path, "/")) {
        printf("Le chemin \"/\" n'est pas un chemin valide.\n");
        exit(EXIT_FAILURE);
    }
    // Si le chemin est absolue, on rappel la fonction avec 
    // la racine et un chemin relatif à la racine pour 
    // permettre la récursion.
    if (is_absolute(path)) {
        //printf("Chemin racine !\n");
        return get_noeud(n->racine,path+1);
    } 
    // Si le chemin est ".." on renvoie le parent du noeud.
    if(!strcmp(path,"..")){
        //printf("%s",path);
        return n->pere;
    }
    // On copie le path avant de le strtok pour le garder intact
    char *pathRec = malloc(sizeof(char)*strlen(path));
    strcpy(pathRec,path);
    
    // On récupère le premier dossier du chemin
    char *name = strtok(path,"/");
    // printf("\npathRec : %s\n",pathRec+(strlen(name)+1));
    // printf("%s",name);
    // Si le name est null, c'est que le chemin est vide ou ne contient que des "/"
    noeud *tmp=n;
    if(name==NULL){
        printf("Le chemin n'est pas valide_\n");
        exit(EXIT_FAILURE);
    }else if(!strcmp(name, "..")){ 
        // Si le premier dossier est ".." on rappel récursivement avec le parent et en enlevant "../" du path
        tmp =get_noeud(n->pere,pathRec+strlen(name)+1);
        free(pathRec);
        pathRec=NULL;
        return tmp;
    }
    // Si le premier dossier est un nom, on le cherche dans la liste des fils
    liste_noeud *list = tmp->fils;
    while(list!=NULL){
        if(!strcmp(name,list->no->nom)){
            // Si la chaine est vide, alors on a fini le path et on renvoie le fichier
            if(!strcmp(pathRec+strlen(name),"")){
                //printf("trouvé %s\n",name);
                free(pathRec);
                pathRec=NULL;
                return list->no;
            }
            //printf("recursif %s\n",list->no->nom);
            // Si non on refait l'appel récursif sans le dossier vérifié
            tmp =get_noeud(list->no,pathRec+strlen(name)+1);
            free(pathRec);
            pathRec=NULL;
            return tmp;
        }
        list=list->succ;
    }
    // Si on arrive ici, c'est que le dossier/fichier analysé n'as pas été trouvé.
    printf("Le fichier indiqué est introuvable");
    exit(EXIT_FAILURE);
}
/** Renvoie le code associé à la commande */
int find_cmd(char* word) {
    /* La dizaine indique le nombre d'arg requis
     * si >= 100 : cas particulier.*/
    
    if (word == NULL)          return  0;
    if (!strcmp(word,"ls"))    return  1;
    if (!strcmp(word,"pwd"))   return  2;
    if (!strcmp(word,"print")) return  3;
    if (!strcmp(word,"mkdir")) return 10;
    if (!strcmp(word,"touch")) return 11;
    if (!strcmp(word,"rm"))    return 12;
    if (!strcmp(word,"cp"))    return 21;
    if (!strcmp(word,"mv"))    return 22;
    if (!strcmp(word,"cd"))    return 100;
    return -1;
}

bool is_absolute(char *path)
{
    return (*path == '/');
}