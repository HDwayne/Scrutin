/**
 * @file graph.h
 * @author HERZBERG Dwayne
 * @brief ficher header de graph.c
 */

#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

/** @brief Structure contenant le numéro du candidat vauncu avec une étiquette. */
struct Edge{
	int value; /**< numméro du candidats. */
	int poid; /**< Valeur attribuée. */
};

/** @brief Structure contenant la liste des candidats vaincus et leur nombre. */
struct DestinationList{
	struct Edge *edgelist; /**< Liste de voisins. */
	int nb_voisins; /**< Nombre de voisins. */
};

/** @brief Structure de graph, contenant le nombre de candidats et leurs duel gagnants. */
struct Graph{
	int nb_candidats; /**< Nombre de candidats. */
	struct DestinationList *tab_candidats; /**< liste de DestinationList. */
};

struct Graph new_graph(int nb_candidats);

void add_edge(struct Graph *graphique, int src, int dest, int poid);

void print_graph(struct Graph *graphique, FILE* logfp);

#endif