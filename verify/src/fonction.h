/**
 * @file fonction.h
 * @author HERZBERG Dwayne
 * @brief ficher header de fonction.c
 */

#ifndef FONCTION_H
#define FONCTION_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "balise.h"
#include "sha256.h"
#include "sha256_utils.h"
#include "utils.h"

/** @brief longueur maximale de la ligne dans le fichier. */
#define MAX 2000

char* equivalent(char elem);

unsigned char* hex_to_binary(unsigned char* resultat, char* hex);

char* hash(char *hashRes, char* name, char* key);

char* generate_key(int size, char* key);

void addhash(char **tab_balises);

void afficher(FILE *file, char* ligne);

void find_in_file(char **tab_balises, char *hashRes);

#endif
