/**
 * @file csv.c
 * @author HERZBERG Dwayne
 * @brief Ensemble des fonctions permettant la construction de la matrice dynamique de caractère.
 */

#include "csv.h"
#include "uninominal.h"


/**
 * @brief Fonction pour afficher le tableau de données csv.
 * 
 * @param t_tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param logfp Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void affiche_t_mat_char_star_dyn(t_mat_char_star_dyn * t_tabmots, FILE* logfp){
    int nb_invalide = 0;

    fprintf(logfp, " ┏╸ TABLE APRES LECTURE\n");
    for (int i = 0; i < t_tabmots->nbRows; i++){
        fprintf(logfp, " ┣╸tabmots[%d] = {\n", i);
        for (int j = 0; j < t_tabmots->offset; j++){
            fprintf(logfp, " ┃     %s,\n", t_tabmots->tab[i][j]);
        }
        fprintf(logfp, " ┃     ");
        for (int j = t_tabmots->offset; j < t_tabmots->nbCol-1; j++){
            fprintf(logfp, "%s,", t_tabmots->tab[i][j]);
        }
        fprintf(logfp, "%s\n ┃ }", t_tabmots->tab[i][t_tabmots->nbCol-1]);
        if (vote_valide(t_tabmots, i)==false){
            fprintf(logfp, " Vote invalide\n");
            nb_invalide++;
        } else
        {
            fprintf(logfp, "\n");
        }
    }
    fprintf(logfp, " ┣╸\n");
    fprintf(logfp, " ┃ nb votes invalide: %d\n", nb_invalide);
    fprintf(logfp, " ┗╸\n");
}


/**
 * @brief Fonction pour lire le fichier Csv et remplir la matrice dynamique.
 * 
 * @param filename Nom du fichier csv.
 * @param delimiteur Délimiteur du fichier csv.
 * @param t_tabmots Tableau comportant l'ensemble des données du fichier csv.
 */
void read_voting_file(char * filename,char *delimiteur,t_mat_char_star_dyn * t_tabmots){
    FILE *file = fopen(filename, "r"); // Ouverture du fichier.

    if (file == NULL){
        printf(ERROR_OPEN_FILE);
        exit(EXIT_FAILURE);
    }

    char *ligne = malloc(MAX*sizeof(char)); 
    char *token;
    int nb_ligne=0;
    int nb_col=0;

    // Lecture de la première ligne + Detection du nombre de col.
 
    int nb_ligne_init=51; // Estimation du nombre de ligne initiale.
    char ***tab = (char***) malloc(nb_ligne_init * sizeof(char**));

    int nb_col_init=10; // Estimation du nombre de col initiale. 
    tab[nb_ligne] = generate_matrice_char(nb_col_init, LONGMOTS);

    fgets(ligne, MAX, file);
    ligne[strcspn(ligne, "\n")] = 0; // Supprimer le '\n' a la fin de la ligne.
    
    token = strtok(ligne, delimiteur);
    while (token!=NULL){ // Lecture de chaque col tant qu'il y a existance.
        if (nb_col == nb_col_init-1){ // Si le nombre de col est presque atteind (prochain tour) alors realloc (+ STEPCOLONE).
            tab[nb_ligne] = (char **) realloc(tab[nb_ligne], (nb_col_init+STEPCOLONE) * sizeof(char*));
            for (int j = nb_col_init; j < (nb_col_init+STEPCOLONE); j++){
                tab[nb_ligne][j] = generate_list_char(LONGMOTS);
            }
            nb_col_init+=STEPCOLONE;
        }
        strcpy(tab[nb_ligne][nb_col], token);
        nb_col++;      
        token = strtok(NULL, delimiteur);
    }

    if (nb_col < nb_col_init){ // Reduire le nombre de col au minimum.
        tab[nb_ligne] = (char **) realloc(tab[nb_ligne], nb_col * sizeof(char*));
    }

    t_tabmots->nbCol=nb_col;
    nb_ligne++;

    // Lecture du contenu du fichier csv.

    for(int i=1; i < nb_ligne_init ; ++i ){ // Initialisation de toutes les col dans les lignes > 0.
        tab[i]=generate_matrice_char(nb_col, LONGMOTS);
    }


    while (fgets(ligne, MAX, file)){ // Tant qu'il y a des lignes.
        ligne[strcspn(ligne, "\n")] = 0; // supprimer le '\n' a la fin de la ligne.

        nb_col=0;
        nb_ligne++;

        if (nb_ligne == nb_ligne_init-1){ // Si le tableau est presque rempli (prochain tour) alors realloc (+ STEPLIGNE).
            tab = (char***) realloc(tab, (nb_ligne_init+STEPLIGNE) * sizeof(char**));
            for (int i = nb_ligne_init; i < nb_ligne_init+STEPLIGNE; i++){
                tab[i] =generate_matrice_char(t_tabmots->nbCol, LONGMOTS);
            }
            nb_ligne_init+=STEPLIGNE;
        }

        token = strtok(ligne, delimiteur);
        while (token!=NULL){
            strcpy(tab[nb_ligne-1][nb_col], token);
            nb_col++;
            token = strtok(NULL, delimiteur);
        }
    }

    if (nb_ligne < nb_ligne_init){ // reduire la taille du tableau au minimum
        tab = (char ***) realloc(tab, nb_ligne * sizeof(char*));
    }
 
    t_tabmots->nbRows=nb_ligne;
    t_tabmots->tab=tab;
    set_offset(t_tabmots);
}

/**
 * @brief Verifie si t_tabmots est une matrice de duel.
 * 
 * @param t_tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @return true, t_tabmots est une matrice de duel.
 * @return false t_tabmots est un ensemble de ballot.
 */
bool is_matrice_duel(t_mat_char_star_dyn * t_tabmots){
    if (t_tabmots->nbRows-1 == t_tabmots->nbCol && strcmp("Réponse", t_tabmots->tab[0][0])!=0 && strcmp("ID votant", t_tabmots->tab[0][0])!=0){ 
        // Si la matrice est carré (+ ligne tete) et que le premier mot de la première ligne != "Réponse" "ID votant".
        return true;
    }
    return false;    
}


/**
 * @brief Définit l'offset.
 * 
 * @param t_tabmots Tableau comportant l'ensemble des données du fichier csv.
 */
void set_offset(t_mat_char_star_dyn * t_tabmots){
    if (is_matrice_duel(t_tabmots)){ // si c'est une matrice de duel.
        t_tabmots->offset=0;
    } else {
        t_tabmots->offset=OFFSET;
    }
}