/**
 * @file csv.h
 * @author HERZBERG Dwayne
 * @brief ficher header de csv.c
 */

#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "utils.h"

/** @brief longueur maximale de la ligne dans le fichier. */
#define MAX 2000
/** @brief longueur max d'une chaîne de caractères (mot). */
#define LONGMOTS 50

/** @brief Nombre de ligne a réserver en plus dans la mémoire a chaque realloc. */
#define STEPLIGNE 50
/** @brief Nombre de colonne a réserver en plus dans la mémoire a chaque realloc. */
#define STEPCOLONE 10
/** @brief Spécification de la taille de l'offset. */
#define OFFSET 4
/** @brief Message d'erreur, erreur lors de louverture du fichier. */
#define ERROR_OPEN_FILE "Erreur lors de l'ouverture du fichier\n"

/** @brief Structure permettant la représentation du fichier csv. */
typedef struct s_mat_char_dyn{
  char *** tab; /**< Matrice trois dimentions de caractère. */
  int nbRows; /**< Nombre de ligne. */
  int nbCol; /**< Nombre de colonne. */
  int offset;/**< Taille de l'offset. */
} t_mat_char_star_dyn;

void affiche_t_mat_char_star_dyn(t_mat_char_star_dyn * t_tabmots, FILE* logfp);

void read_voting_file(char * filename,char *delimiteur,t_mat_char_star_dyn * t_tabmots);

bool is_matrice_duel(t_mat_char_star_dyn * t_tabmots);

void set_offset(t_mat_char_star_dyn * t_tabmots);

#endif