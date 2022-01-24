/**
 * @file graph.c
 * @author HERZBERG Dwayne
 * @brief Ensemble des fonction pour la création d'un graph et son affichage.
 */

#include "graph.h"


/**
 * @brief Création et initialisation d'une structure Graph.
 * 
 * @param nb_candidats Nombre candidats.
 * @return struct Graph.
 */
struct Graph new_graph(int nb_candidats){
	struct Graph graphique;

	graphique.nb_candidats = nb_candidats;
	graphique.tab_candidats=(struct DestinationList*) malloc(nb_candidats * sizeof(struct DestinationList));

	for(int i = 0 ; i < graphique.nb_candidats; i++){
		graphique.tab_candidats[i].edgelist= (struct Edge*) malloc(nb_candidats * sizeof(struct Edge));
		graphique.tab_candidats[i].nb_voisins=0;
	}

	return graphique;
}


/**
 * @brief Permet l'ajout d'un lien dans le graph.
 * 
 * @param graphique 
 * @param src Entier représentant la source.
 * @param dest Entier représentant le destinataire.
 * @param poid Entier représentant le poid.
 */
void add_edge(struct Graph *graphique, int src, int dest, int poid){
	graphique->tab_candidats[src].edgelist[graphique->tab_candidats[src].nb_voisins].poid=poid;
	graphique->tab_candidats[src].edgelist[graphique->tab_candidats[src].nb_voisins].value=dest;
	graphique->tab_candidats[src].nb_voisins+=1;
}


/**
 * @brief Affichage d'un graph dans logfp.
 * 
 * @param graphique 
 * @param logfp Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void print_graph(struct Graph *graphique, FILE* logfp){
	fprintf(logfp, " ┏╸graph\n");
	for (int i = 0; i < graphique->nb_candidats; i++){
		fprintf(logfp, " ┃ candidat %d :\n", i);
		for (int j = 0; j < graphique->tab_candidats[i].nb_voisins; j++){
			fprintf(logfp, " ┃ ╰ vers candidat %d (poid %d)\n", graphique->tab_candidats[i].edgelist[j].value, graphique->tab_candidats[i].edgelist[j].poid);
		}
		fprintf(logfp, " ┃\n");
	}
	fprintf(logfp, " ┗╸\n");
}