/**
 * @file utils.h
 * @author HERZBERG Dwayne
 * @brief ficher header de utils.c
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

/** @brief Message d'erreur, erreur lors de louverture du fichier. */
#define ERROR_OPEN_FILE "Erreur lors de l'ouverture du fichier\n"

char* generate_list_char(int size);

char** generate_matrice_char(int size_x, int size_y);

void check_open_file(FILE* file);

#endif