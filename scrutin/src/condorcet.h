/**
 * @file condorcet.h
 * @author HERZBERG Dwayne
 * @brief ficher header de condorcet.c
 */

#ifndef CONDOR_H
#define CONDOR_H

#include <stdio.h>
#include "uninominal.h"
#include "graph.h"
#include "utils.h"


int ** generate_duels(t_mat_char_star_dyn *tabmots);

bool is_vainqueur_candidat(int ** matrice_duel, int candidat, int nb_candidats, FILE* logfp);

bool condorcet_vainqueur(t_mat_char_star_dyn *tabmots, int** matrice_duel, FILE* logfp);

struct Graph generer_graph(t_mat_char_star_dyn *tabmots, int** matrice_duel);

int max(int a, int b);

int min(int a, int b);

void schulze(t_mat_char_star_dyn *tabmots, int** matrice_duel, FILE* logfp);

void afficher_resultat_minimax(int* resultat, int nb_candidats, FILE* logfp);

void minimax_margin(t_mat_char_star_dyn *tabmots, int** matrice_duel, FILE* logfp);

void minimax_winning_votes(t_mat_char_star_dyn *tabmots, int** matrice_duel, FILE* logfp);

void condorcet(t_mat_char_star_dyn *tabmots, char* methode, FILE* logfp);

#endif
