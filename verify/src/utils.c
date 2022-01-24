/**
 * @file utils.c
 * @author HERZBERG Dwayne
 * @brief Ensemble des fonctions pour générer les tableaux.
 */

#include "utils.h"


/**
 * @brief Afficher une erreur et exit.
 * 
 */
void erreur_allocation(){
    fprintf(stderr, "Erreur allocation.\n");
    exit(EXIT_FAILURE);
}


/**
 * @brief Générer liste caractère.
 * 
 * @param size Taille de la liste.
 * @return char* 
 */
char* generate_list_char(int size){
    char* tab = (char*) malloc(size * sizeof(char));
    if (tab == NULL) erreur_allocation();
    return tab;
}


/**
 * @brief Générer matrice caractère.
 * 
 * @param size_x Nombre de mots.
 * @param size_y Nombre de caractère dans un mot.
 * @return char** 
 */
char** generate_matrice_char(int size_x, int size_y){
    char **matrice = (char**) malloc(size_x * sizeof(char*));
    if (matrice == NULL) erreur_allocation();

    for(short i=0; i < size_x ; ++i ){
        matrice[i] = (char*) malloc(size_y*sizeof(char));
        if (matrice[i] == NULL) erreur_allocation();
    }
    return matrice;
}


/**
 * @brief Vérifie si le fichier est bien ouvert.
 * 
 * @param file 
 */
void check_open_file(FILE* file){
    if (file == NULL){
        fprintf(stderr, ERROR_OPEN_FILE);
        exit(EXIT_FAILURE);
    }
}