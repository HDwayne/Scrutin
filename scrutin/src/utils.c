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
 * @brief Générer une liste d'entier.
 * 
 * @param size Taille de la liste.
 * @return int* 
 */
int* generate_list_int(int size){
    int* tab = (int *) malloc(size*sizeof(int));
    if (tab == NULL) erreur_allocation();
    return tab;

}


/**
 * @brief Générer une matrice d'entier.
 * 
 * @param size Taille de la matrice.
 * @return int** 
 */
int** generate_matrice_int(int size){
    int ** matrice = (int **) malloc(size * sizeof(int*));
    if (matrice == NULL) erreur_allocation();
    for (int i = 0; i < size; i++){
        matrice[i]= (int *) malloc(size * sizeof(int));
        if (matrice[i] == NULL) erreur_allocation();
    }
    return matrice;
}


/**
 * @brief Afficher matrice.
 * 
 * @param matrice Matrice int**.
 * @param taille Taille matrice.
 * @param logfp Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void afficher_matrice(int** matrice, int taille, FILE* logfp){
    for (int i = 0; i < taille; i++){
        fprintf(logfp, " ┃ ");
        for (int j = 0; j < taille; j++){
            fprintf(logfp, "%3d", matrice[i][j]);
        }
        fprintf(logfp, "\n");
    }
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