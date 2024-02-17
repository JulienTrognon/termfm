#ifndef PARSE_H
#define PARSE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "parse.h"
#include "arbre.h"



extern int MAX_LINE_SIZE;

/** Traite la chaîne en argument.*/
char **parse(char *restrict line, char *separator);
noeud *get_noeud(noeud *n, char *path);
/** Retourne un code int associé à la chaîne en argument */
int find_cmd(char *restrict word);
/** Renvoie si le chemin est absolue ou non */
bool is_absolute(char *path);
#endif