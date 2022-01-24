/**
 * @file condorcet.c
 * @author HERZBERG Dwayne
 * @brief Ensemble des fonctions pour le scrutin de condorcet et ses paradoxes.
 */

#include "condorcet.h"


/**
 * @brief Générer une matrice de duel.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @return int**, matrice d'entiers.
 */
int ** generate_duels(t_mat_char_star_dyn *tabmots){
    int nb_candidats = get_nb_candidat(tabmots);

    int ** matrice_duel = generate_matrice_int(nb_candidats);

    if (!is_matrice_duel(tabmots)){ // Si ballot
        for (int i = 0; i < nb_candidats; i++){
            for (int j = 0; j < nb_candidats; j++){
                matrice_duel[i][j]=0; // Initialiser la matrice à 0
            }
        }

        for (int ligne_matrice_char = 1; ligne_matrice_char < tabmots->nbRows; ligne_matrice_char++){ //chaque ligne de vote
            if (vote_valide(tabmots, ligne_matrice_char)==true){
                for (int candidats = tabmots->offset; candidats < tabmots->nbCol; candidats++){ //chaque candidats
                    for (int adversaire =  tabmots->offset; adversaire < tabmots->nbCol; adversaire++){ // chaque adversaire de duel  
                        if (candidats != adversaire && (atoi(tabmots->tab[ligne_matrice_char][candidats]) < atoi(tabmots->tab[ligne_matrice_char][adversaire]))){
                            // gagne le duel. (inférieur préféré)
                            matrice_duel[candidats-tabmots->offset][adversaire-tabmots->offset] +=1;
                        }
                    }       
                }
            }
        }
    } else { // Si déjà matrice duel
        for (int i = 0; i < tabmots->nbRows-1; i++){
            for (int j = 0; j < tabmots->nbCol; j++){
                matrice_duel[i][j]=atoi(tabmots->tab[i+1][j+tabmots->offset]);
            }
        }
    }
    return matrice_duel;
}


/**
 * @brief Vérifie si un candidat est un vainqueur de condorcet.
 * 
 * @param matrice_duel Matrice duel
 * @param candidat Numéro candidat
 * @param nb_candidats Nombre candidats
 * @param logfp Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 * @return true, si candidat est un vainqueur de condorcet.
 * @return false, si candidat perd un duel (n'est pas vainqueur de condorcet).
 */
bool is_vainqueur_candidat(int ** matrice_duel, int candidat, int nb_candidats, FILE* logfp){
    fprintf(logfp," ┃ ╭ Candidat %d\n", candidat);
    for (int adv = 0; adv < nb_candidats; adv++){
        if (candidat != adv && (matrice_duel[candidat][adv] <= matrice_duel[adv][candidat])){
            fprintf(logfp, " ┃ ╰ perd contre %d (%d <= %d)\n", adv, matrice_duel[candidat][adv], matrice_duel[adv][candidat]);
            return false;
        }   
    }
    fprintf(logfp, " ┃ ╰ a gagné tout les combats\n");
    return true;
}


/**
 * @brief Détermine s'il existe un vainqueur de condorcet.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param matrice_duel Matrice duel.
 * @param logfp  Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 * @return true, s'il existe un seul et unique vainquer de condorcet.
 * @return false, sinon.
 */
bool condorcet_vainqueur(t_mat_char_star_dyn *tabmots, int** matrice_duel, FILE* logfp){
    int nb_candidats = get_nb_candidat(tabmots);

    int nbvainqueur=0;
    int vainqueur;

    for (int candidat = 0; candidat < nb_candidats; candidat++){
        if (is_vainqueur_candidat(matrice_duel, candidat, nb_candidats, logfp)==true){
            vainqueur=candidat;
            nbvainqueur+=1;
        }
    }
    fprintf(logfp, " ┃ nombre vainqueur de condorcet: %d\n", nbvainqueur);

    if (nbvainqueur!=1){ // Si le nombre de vainqueur != 1
        fprintf(logfp, " ┃ Unique vainqueur de condorcet n'existe pas.\n ┗╸\n");
        return false;
    } // sinon afficher gagnant
    fprintf(logfp, " ┗╸\n");
    
    fprintf(stdout, "Mode de scrutin : Vainqueur de Condorcet, %d candidats, %d votants, vainqueur de Condorcet = %s\n", nb_candidats, tabmots->nbRows-1, tabmots->tab[0][vainqueur+tabmots->offset]);

    return true;
}

/**
 * @brief Génère un graph depuis la matrice de duel.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param matrice_duel Matrice duel.
 * @return struct Graph.
 */
struct Graph generer_graph(t_mat_char_star_dyn *tabmots, int** matrice_duel){
    int nb_candidats = get_nb_candidat(tabmots);
    struct Graph graphique = new_graph(nb_candidats);

    for (int candidat = 0; candidat < nb_candidats; candidat++){
        for (int adv = 0; adv < nb_candidats; adv++){
            if (candidat != adv && (matrice_duel[candidat][adv] > matrice_duel[adv][candidat])){
                add_edge(&graphique, adv, candidat, matrice_duel[candidat][adv]-matrice_duel[adv][candidat]);
            }
        }
    }
    
    return graphique;
}

/**
 * @brief Retourne le maximum.
 * 
 * @param a entier a.
 * @param b entier b.
 * @return int, max(a,b).
 */
int max(int a, int b){ return a > b ? a : b; }


/**
 * @brief Retourne le minimum.
 * 
 * @param a entier a.
 * @param b entier b.
 * @return int, min(a,b).
 */
int min(int a, int b){ return a < b ? a : b; }


/**
 * @brief Détermine un vainqueur selon la méthode Schulze.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param matrice_duel Matrice duel.
 * @param logfp  Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void schulze(t_mat_char_star_dyn *tabmots, int** matrice_duel, FILE* logfp){
    int nb_candidats = get_nb_candidat(tabmots);

    fprintf(logfp, " ┏╸ schulze\n");

    int ** matrice_path = generate_matrice_int(nb_candidats);

    for (int i = 0; i < nb_candidats; i++){ // Générer matrice duel avec uniquement les gagnants
        for (int j = 0; j < nb_candidats; j++){
            if (i != j){
                if (matrice_duel[i][j] > matrice_duel[j][i]){
                    matrice_path[i][j]=matrice_duel[i][j];
                } else{
                    matrice_path[i][j]=0;
                }
            }
        }
    }

    // Afficher matrice

    fprintf(logfp, " ┃ Matrice with winner only:\n");
    afficher_matrice(matrice_path, nb_candidats, logfp);
    fprintf(logfp, " ┣╸\n");
    
    for (int i = 0; i < nb_candidats; i++){  // Générer matrice de chemin selon la méthode Schulze
        for (int j = 0; j < nb_candidats; j++){
            if (i != j){
                for (int k = 0; k < nb_candidats; k++){
                    if (i!=k && j!=k){
                        matrice_path[j][k]=max(matrice_path[j][k], min(matrice_path[j][i], matrice_path[i][k]));
                    }
                }
            }
        }
    }

    // Afficher matrice

    fprintf(logfp, " ┃ Matrice Path:\n");
    afficher_matrice(matrice_path, nb_candidats, logfp);
    fprintf(logfp, " ┣╸\n");

    // générer une liste pour indiquer les résultats de chaque candidat
    int* resultat = generate_list_int(nb_candidats);
    int nb;
    for (int i = 0; i < nb_candidats; i++){
        nb=0;
        for (int j = 0; j < nb_candidats; j++){
            if (i != j && matrice_path[i][j] > matrice_path[j][i]){
                fprintf(logfp, " ┃ +1 pour candidat %d (%d > %d)\n", i,  matrice_path[i][j], matrice_path[j][i]);
                nb += 1;
            }
        }
        resultat[i]=nb;
    }
    
    fprintf(logfp, " ┃ ╰ resultat: ");
    for (int i = 0; i < nb_candidats; i++){
        fprintf(logfp, "%4d", resultat[i]);
    }
    fprintf(logfp, "\n");

    int gagnant=indice_premier(resultat, nb_candidats); // Chercher le vainqueur selon schulze

    fprintf(logfp, " ┃ gagnant: %d (%s)\n ┗╸\n", gagnant, tabmots->tab[0][gagnant+tabmots->offset]);

    fprintf(stdout, "Mode de scrutin : Condorcet Schulze, %d candidats, %d votants, vainqueur = %s\n", nb_candidats, tabmots->nbRows-1, tabmots->tab[0][gagnant+tabmots->offset]);

    free(resultat);
    free(matrice_path);
}


/**
 * @brief Sous fonction minimax - Afficher resultat dans logfp.
 * 
 * @param resultat Liste d'entier.
 * @param nb_candidats Nombre candidats.
 * @param logfp  Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void afficher_resultat_minimax(int* resultat, int nb_candidats, FILE* logfp){
    fprintf(logfp, " ┃ ╰ resultat: ");
    for (int i = 0; i < nb_candidats; i++){
        fprintf(logfp, "%4d", resultat[i]);
    }
    fprintf(logfp, "\n");
}


/**
 * @brief Détermine un vainqueur selon la méthode minimax (basé sur la marge)
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param matrice_duel Matrice duel.
 * @param logfp  Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void minimax_margin(t_mat_char_star_dyn *tabmots, int** matrice_duel, FILE* logfp){
    int nb_candidats = get_nb_candidat(tabmots);
    int* resultat = generate_list_int(nb_candidats);

    fprintf(logfp, " ┏╸ minimax margin\n");

    int margin;
    for (int candidat = 0; candidat < nb_candidats; candidat++){
        margin=0;
        for (int adv = 0; adv < nb_candidats; adv++){ 
            // Si le duel est perdant pour candidat et que la différence entre les deux scores est > a la précédente alors la sauvegardé.
            if (matrice_duel[candidat][adv] < matrice_duel[adv][candidat] && matrice_duel[adv][candidat]-matrice_duel[candidat][adv] > margin){
                fprintf(logfp, " ┃ candidat %d, %d perd contre %d (margin %d)\n", candidat, matrice_duel[candidat][adv], matrice_duel[adv][candidat], matrice_duel[adv][candidat]-matrice_duel[candidat][adv]);
                margin=matrice_duel[adv][candidat]-matrice_duel[candidat][adv];
            }
        }
        resultat[candidat]=margin;
    }

    afficher_resultat_minimax(resultat, nb_candidats, logfp); // Afficher le résultat

    int gagnant=indice_dernier(resultat, nb_candidats); // Déterminer le gagnant
    free(resultat);
    fprintf(logfp, " ┃ gagnant: %d (%s)\n ┗╸\n", gagnant, tabmots->tab[0][gagnant+tabmots->offset]);

    fprintf(stdout, "Mode de scrutin : Condorcet Minimax (margin), %d candidats, %d votants, vainqueur = %s\n", nb_candidats, tabmots->nbRows-1, tabmots->tab[0][gagnant+tabmots->offset]);
    
}


/**
 * @brief Détermine un vainqueur selon la méthode minimax (basé sur le pourcentage).
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param matrice_duel Matrice duel.
 * @param logfp  Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void minimax_winning_votes(t_mat_char_star_dyn *tabmots, int** matrice_duel, FILE* logfp){
    int nb_candidats = get_nb_candidat(tabmots);
    int* resultat = generate_list_int(nb_candidats);

    fprintf(logfp, " ┏╸ minimax winning votes\n");

    int min;
    for (int candidats = 0; candidats < nb_candidats; candidats++){
        min=999;
        for (int adv = 0; adv < nb_candidats; adv++){
            // Si le duel est perdant pour candidat et que son % < précédent, le sauvegarder.
            if (matrice_duel[candidats][adv] < matrice_duel[adv][candidats] && (matrice_duel[candidats][adv]*100)/(tabmots->nbRows-1) < min){
                fprintf(logfp, " ┃ candidat %d, %d perd contre %d (vote %d)\n", candidats, matrice_duel[candidats][adv], matrice_duel[adv][candidats], (matrice_duel[candidats][adv]*100)/(tabmots->nbRows-1));
                min=(matrice_duel[candidats][adv]*100)/(tabmots->nbRows-1);
            }
        }
        resultat[candidats]=min;
    }

    afficher_resultat_minimax(resultat, nb_candidats, logfp); // Afficher le résultat

    int gagnant=indice_premier(resultat, nb_candidats); // Déterminer le gagnant
    free(resultat);
    fprintf(logfp, " ┃ gagnant: %d (%s)\n ┗╸\n", gagnant, tabmots->tab[0][gagnant+tabmots->offset]);

    fprintf(stdout, "Mode de scrutin : Condorcet Minimax (winning votes), %d candidats, %d votants, vainqueur = %s\n", nb_candidats, tabmots->nbRows-1, tabmots->tab[0][gagnant+tabmots->offset]);
    
}


/**
 * @brief Fonction principale de Condorcet. Détermine si les paradoxes sont nécessaire ou non. Réalise l'ensemble des scrutins de Condorcet.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param methode Méthode de paradoxe souhaité.
 * @param logfp Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void condorcet(t_mat_char_star_dyn *tabmots, char* methode, FILE* logfp){
    int** matrice_duel = generate_duels(tabmots);
    int nb_candidats = get_nb_candidat(tabmots);
    
    struct Graph graphique = generer_graph(tabmots, matrice_duel); // Generer le graph
    print_graph(&graphique, logfp); // afficher le graph

    fprintf(logfp, " ┏╸ vainqueur de condorcet\n");
    fprintf(logfp, " ┃ Matrice Duel:\n");
    afficher_matrice(matrice_duel, nb_candidats, logfp);
    fprintf(logfp, " ┣╸\n");


    if (!condorcet_vainqueur(tabmots, matrice_duel, logfp)){ // S'il n'y a pas de vainquer de condorcet.
        if (strcmp(methode, "all")==0){ // Si minimax et schulze.
            minimax_margin(tabmots, matrice_duel, logfp);
            minimax_winning_votes(tabmots, matrice_duel, logfp);
            schulze(tabmots, matrice_duel, logfp);
        } else if (strcmp(methode, "cs")==0){ // Si uniquement Schulze.
            schulze(tabmots, matrice_duel, logfp);
        } else if (strcmp(methode, "cm")==0){ // Si uniquement minimax.
            minimax_margin(tabmots, matrice_duel, logfp);
            minimax_winning_votes(tabmots, matrice_duel, logfp);
        }
    }
    
    for (int i = 0; i < nb_candidats; i++){
        free(matrice_duel[i]);
    }
    free(matrice_duel);
}
