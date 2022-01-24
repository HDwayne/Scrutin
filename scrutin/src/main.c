/**
 * @file main.c
 * @author HERZBERG Dwayne
 * @brief Fonction principale du programme.
 */

#include "balise.h"
#include "uninominal.h"
#include "condorcet.h"
#include "utils.h"

/** @brief Message d'erreur, format du fichier csv invalide. */
#define ERROR_MATRICE_FORMAT "Le format du fichier csv ne correspond pas à la balise indiqué (duel / ballot)\n"
/** @brief Message, traitement d'un fichier de ballots. */
#define TRAITEMENT_BALLOT "Traitement d'un fichier csv de ballots\n"
/** @brief Message, traitement d'un fichier de duels. */
#define TRAITEMENT_DUEL "Traitement d'un fichier csv de matrice de duels\n"


/**
 * @brief Fonction principal du programme.
 * 
 * @param argc 
 * @param argv 
 * @return int, renvoie 0 si succès sinon 1.
 */
int main(int argc, char **argv){
    // Générer tableau_balise.

    char **tableau_balise = generate_matrice_char(NB_BALISES, 80);

    getop(tableau_balise, argc, argv);

    // Redirection logfp si spécifiée par l'utilisateur.

    FILE *logfp = NULL;
    if (strcmp(tableau_balise[OPT_O], "")==0){
        logfp=stdout;
    } else {
        logfp=fopen(tableau_balise[OPT_O], "a+");
    }

    // Générer tableau dynamique de mots (exrait de csv).

    t_mat_char_star_dyn tableau_char_mat;

    if (strcmp(tableau_balise[OPT_D], "")==0){ // si option -i
        read_voting_file(tableau_balise[OPT_I], ",", &tableau_char_mat);
        if (is_matrice_duel(&tableau_char_mat)){
            fprintf(stderr, ERROR_MATRICE_FORMAT);
            exit(EXIT_FAILURE);
        }
        affiche_t_mat_char_star_dyn(&tableau_char_mat, logfp);
        fprintf(logfp, " ┏╸\n ┃ ");
        fprintf(logfp, TRAITEMENT_BALLOT);
    } else { // option -d
        read_voting_file(tableau_balise[OPT_D], ",", &tableau_char_mat);
        if (!is_matrice_duel(&tableau_char_mat)){
            fprintf(stderr, ERROR_MATRICE_FORMAT);
            exit(EXIT_FAILURE);
        }
        affiche_t_mat_char_star_dyn(&tableau_char_mat, logfp);
        fprintf(logfp, " ┏╸\n ┃ ");
        fprintf(logfp, TRAITEMENT_DUEL);
    }

    fprintf(logfp, " ┣╸\n ┃ Nb lignes lues  = %4d\n", tableau_char_mat.nbRows);
    fprintf(logfp, " ┃ Nb colonnes lue = %4d\n", tableau_char_mat.nbCol);
    fprintf(logfp, " ┃ Nb candidats    = %4d\n", get_nb_candidat(&tableau_char_mat));
    fprintf(logfp, " ┃ Nb votants      = %4d\n ┗╸\n", tableau_char_mat.nbRows-1);
    

    // Différentes méthodes de scrutin.

    if (strcmp(tableau_balise[OPT_M], "all") == 0){
        if (strcmp(tableau_balise[OPT_D], "") == 0){
            uninominal_un_tour(&tableau_char_mat, logfp);
            printf("\n");
            uninominal_deux_tours(&tableau_char_mat, logfp);
            printf("\n");
        }
        condorcet(&tableau_char_mat, "all", logfp);
    } else if (strcmp(tableau_balise[OPT_M], "cs") == 0){
        condorcet(&tableau_char_mat, "cs", logfp);
    } else if (strcmp(tableau_balise[OPT_M], "cm") == 0){
        condorcet(&tableau_char_mat, "cm", logfp);
    } else if (strcmp(tableau_balise[OPT_M], "uni2") == 0){
        uninominal_deux_tours(&tableau_char_mat, logfp);
    } else if (strcmp(tableau_balise[OPT_M], "uni1") == 0){
        uninominal_un_tour(&tableau_char_mat, logfp);
    }
    

    for(short i=0; i < NB_BALISES ; ++i ){
        free(tableau_balise[i]);
    }
    free(tableau_balise);
    fclose(logfp);
    
   return 0;
}