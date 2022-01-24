/**
 * @file uninominal.h
 * @author HERZBERG Dwayne
 * @brief ficher header de uninominal.c
 */

#ifndef UNI_H
#define UNI_H

#include <stdio.h>
#include <stdbool.h>
#include "csv.h"
#include "utils.h"


bool vote_valide(t_mat_char_star_dyn *tabmots, int ligne);

int get_nb_candidat(t_mat_char_star_dyn *tabmots);

void afficher_decompte(t_mat_char_star_dyn *tabmots, int *decompte, int nb_candidat, int nb_invalide, FILE* logfp);

int generate_decompte(t_mat_char_star_dyn *tabmots, int *decompte);

float score(int nb_vote_tt, int nb_vote_invalide, int nb_vote);

int indice_premier(int *decompte, int nb_candidat);

int indice_second(int *decompte, int nb_candidat, int indice_premier);

int indice_dernier(int *decompte, int nb_candidat);

void uninominal_un_tour(t_mat_char_star_dyn *tabmots, FILE* logfp);

void uninominal_deux_tours(t_mat_char_star_dyn *tabmots, FILE* logfp);

#endif