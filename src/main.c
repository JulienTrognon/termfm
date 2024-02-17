#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "parse.h"
#include "cmd.h"

// nombre arbitraire pas trop grand 
// pour compatibilite des machines.
int MAX_LINE_SIZE = 4096;
#define SEP " \n\t\r"

int main(int argc, char const *argv[])
{
    // (0) check de la commande d'appel
    if (argc < 2) {
        printf("Veuillez indiquer un chemin vers un fichier texte");
        printf("termfm <file_path>");
        return EXIT_FAILURE;
    }
    if (argc > 2) {
        printf("Trop d'arguments.");
        return EXIT_FAILURE;
    }

    // (1) creation de l'arbre
    noeud *racine = init_arbre();
    noeud *courant = racine;

    // (2) chargement du fichier en buffer
    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        printf("%s : fichier inexistant.\n", argv[1]);
        return EXIT_FAILURE;
    }

    char line[MAX_LINE_SIZE];
    char **parsed;
    
    // (3) lecture du fichier ligne par ligne
    while (!feof(file)) {
        fgets(line, MAX_LINE_SIZE, file);
        char *line_ptr = malloc(strlen(line) + 1);
        strcpy(line_ptr, line);
        parsed = parse(line_ptr, SEP);
        if(parsed==NULL){
            continue;
        }
        
        // On recupère le numero de la commande pour verifie son existance
        int cmd = find_cmd(parsed[0]); 
        if (cmd <= 0) {
            if (cmd == -1) {
                printf("%s : commande inconnue.\n", parsed[0]);
                exit(EXIT_FAILURE);
            }
            continue;
        }
        char **args = parsed+1;

        if (!right_amount_args(cmd, args)) {
            printf("%s : nombre d'arguments incorrect.\n", parsed[0]);
            exit(EXIT_FAILURE);
        }

        exec(cmd, args, &courant);
        free(parsed);
        parsed=NULL;
    }
    // printf("Fin du programme.\n");

    // (4) Fermeture des flux
    if(racine->fils!=NULL){
        rm_fils(racine->fils);
        racine->fils=NULL;
    }
    // print(racine);
    free(racine);
    racine = NULL;
    courant = NULL;

    int fc=fclose(file);
    // check si toutes les fermetures de fichiers se sont bien deroulees
    if (fc != 0) {
        printf("Erreur lors de la fermeture du fichier");
        return EXIT_FAILURE;
    }
}