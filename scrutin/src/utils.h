/**
 * @file utils.h
 * @author HERZBERG Dwayne
 * @brief ficher header de utils.c
 */

#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

int* generate_list_int(int size);

int** generate_matrice_int(int size);

void afficher_matrice(int** matrice, int taille, FILE* logfp);

char* generate_list_char(int size);

char** generate_matrice_char(int size_x, int size_y);

#endif