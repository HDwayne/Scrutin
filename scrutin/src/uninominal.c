/**
 * @file uninominal.c
 * @author HERZBERG Dwayne
 * @brief Ensemble des fonctions pour les scrutins uni1 et uni2.
 */

#include "uninominal.h"
#include "csv.h"


/**
 * @brief Verifie si un vote est valide.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param ligne Entier correspondant à la ligne du vote.
 * @return true, vote valide.
 * @return false, vote invalide.
 */
bool vote_valide(t_mat_char_star_dyn *tabmots, int ligne){
    for (int candidat = tabmots->nbCol-1; candidat >= tabmots->offset; candidat--){
        if (strcmp(tabmots->tab[ligne][candidat],"")==0 || strcmp(tabmots->tab[ligne][candidat],"-999")==0){ // Si un candidat n'est pas classé.
            return false;
        }
        // for (int i = tabmots->offset; i < tabmots->nbCol-1; i++){ // Si deux candidats ont le même classement.
        //     if (strcmp(tabmots->tab[ligne][candidat], tabmots->tab[ligne][i])==0 && i!=candidat){
        //         return false;
        //     }  
        // }
    }
    return true;
}


/**
 * @brief Renvoie le nombre de candidats.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @return int, nombre de candidats.
 */
int get_nb_candidat(t_mat_char_star_dyn *tabmots){
    return tabmots->nbCol-tabmots->offset;
}


/**
 * @brief Affiche le décompte. Nombre personne ayant voté pour chacun des candidats.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param decompte Liste des resultats des candidats.
 * @param nb_candidat Nombre de candidats.
 * @param nb_invalide Nombre de vote invalide.
 * @param logfp Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void afficher_decompte(t_mat_char_star_dyn *tabmots, int *decompte, int nb_candidat, int nb_invalide, FILE* logfp){
    fprintf(logfp, " ┃ Décompte Premier tour (nb personne ayant voté pour les candidats)\n");
    int somme=0;
    for (int i = 0; i < nb_candidat; i++){
        fprintf(logfp, " ┃ ╰ %s -> %d (%.2f%%)\n", tabmots->tab[0][i+tabmots->offset], decompte[i], score(tabmots->nbRows-1, nb_invalide, decompte[i]));
        somme+=decompte[i];
    }
    fprintf(logfp, " ┃ nb vote enregistré: %d (%d attendu)\n ┣╸\n", somme, tabmots->nbRows-1-nb_invalide);
}


/**
 * @brief Génère le décompte.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param decompte Liste des resultats des candidats.
 * @return int, nombre vote invalide.
 */
int generate_decompte(t_mat_char_star_dyn *tabmots, int *decompte){
    int nb_invalide=0;
    for (int ligne = 1; ligne < tabmots->nbRows; ligne++){ // Pour chaque vote
        if (vote_valide(tabmots, ligne)==false){
            nb_invalide++;
        } else { // cherche le plus petit (préféré) dans le ballot. si deux = alors le premier est arbitrairement pris en compte.
            int nbmin=atoi(tabmots->tab[ligne][tabmots->offset]);
            int posmin=tabmots->offset;
            for (int i = tabmots->offset; i < tabmots->nbCol; i++){
                if (atoi(tabmots->tab[ligne][i]) < nbmin){
                    nbmin=atoi(tabmots->tab[ligne][i]);
                    posmin=i;
                }
            }
            decompte[posmin-tabmots->offset]+=1; // Ajoute 1 au candidat préféré du ballot.
        }
    }
    return nb_invalide;
}


/**
 * @brief Calcul le score d'un joueur.
 * 
 * @param nb_vote_tt Nombre de vote total.
 * @param nb_vote_invalide Nombre de vote invalide.
 * @param nb_vote Nombre de vote du candidat.
 * @return float, score du joueur.
 */
float score(int nb_vote_tt, int nb_vote_invalide, int nb_vote){
    return (nb_vote*100)/(float)(nb_vote_tt-nb_vote_invalide);
}


/**
 * @brief Cherche l'indice du plus grand nombre d'une liste.
 * 
 * @param decompte Liste des resultats des candidats.
 * @param nb_candidat Nombre de candidats.
 * @return int, indice du gagnant.
 */
int indice_premier(int *decompte, int nb_candidat){
    int max=0;
    int indice_gagnant;
    for (int i = 0; i < nb_candidat; i++){
        if (decompte[i] > max){
            max = decompte[i];
            indice_gagnant=i;
        }
    }
    return indice_gagnant;
}


/**
 * @brief Cherche l'indice du second plus grand nombre d'une liste.
 * 
 * @param decompte Liste des resultats des candidats.
 * @param nb_candidat Nombre de candidats.
 * @param indice_premier Indice du plus grand nombre de la liste.
 * @return int, indice du second.
 */
int indice_second(int *decompte, int nb_candidat, int indice_premier){
    int max=0;
    int indice_gagnant;
    for (int i = 0; i < nb_candidat; i++){
        if (decompte[i] > max && i!=indice_premier){
            max = decompte[i];
            indice_gagnant=i;
        }
    }
    return indice_gagnant;
}


/**
 * @brief Cherche l'indice du plus petit nombre d'une liste.
 * 
 * @param decompte Liste des resultats des candidats.
 * @param nb_candidat Nombre de candidats.
 * @return int, indice du dernier.
 */
int indice_dernier(int *decompte, int nb_candidat){
    int min=decompte[0];
    int indice_gagnant;
    for (int i = 1; i < nb_candidat; i++){
        if (decompte[i] < min){
            min = decompte[i];
            indice_gagnant=i;
        }
    }
    return indice_gagnant;
}


/**
 * @brief Fonction uninominale un tour.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param logfp Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void uninominal_un_tour(t_mat_char_star_dyn *tabmots, FILE* logfp){
    int nb_candidat = get_nb_candidat(tabmots);

    int *decompte;
    decompte = generate_list_int(nb_candidat);
    for (int i = 0; i <= nb_candidat; i++){
        decompte[i]=0;
    }
    
    fprintf(logfp, " ┏╸ uninominal un tour\n");

    int nb_invalide = generate_decompte(tabmots, decompte);

    afficher_decompte(tabmots, decompte, nb_candidat, nb_invalide, logfp);

    int indice_gagnant = indice_premier(decompte, nb_candidat);
    int indice_deuxieme = indice_second(decompte, nb_candidat, indice_gagnant);
    
    fprintf(logfp, " ┃ indice premier: %d\n", indice_gagnant);
    fprintf(logfp, " ┃ indice second: %d\n ┗╸\n", indice_deuxieme);

    float pourcentage = score(tabmots->nbRows-1, nb_invalide, decompte[indice_gagnant]);

    fprintf(stdout, "Mode de scrutin : uninominal à un tour, %d candidats, %d votants, vainqueur = %s, score = %.2f%%\n", nb_candidat, tabmots->nbRows-1, tabmots->tab[0][indice_gagnant+tabmots->offset], pourcentage);
    if (decompte[indice_gagnant] == decompte[indice_deuxieme]){ // S'il y a ex aequo (50% - 50%)
        float pourcentage2 = score(tabmots->nbRows-1, nb_invalide, decompte[indice_deuxieme]);
        fprintf(stdout, "Mode de scrutin : uninominal à un tour, %d candidats, %d votants, vainqueur = %s, score = %.2f%%\n", nb_candidat, tabmots->nbRows-1, tabmots->tab[0][indice_deuxieme+tabmots->offset], pourcentage2);
    }
    
    free(decompte);
}


/**
 * @brief Fonction uninominale deux tours.
 * 
 * @param tabmots Tableau comportant l'ensemble des données du fichier csv.
 * @param logfp Emplacement log. stdout ou fichier spécifié par l'utilisateur.
 */
void uninominal_deux_tours(t_mat_char_star_dyn *tabmots, FILE* logfp){
    int nb_candidat = get_nb_candidat(tabmots);

    int *decompte_pr = generate_list_int(nb_candidat);
    for (int i = 0; i <= nb_candidat; i++){
        decompte_pr[i]=0;
    }

    fprintf(logfp, " ┏╸ uninominal deux tours\n");

    //Premier tour

    int nb_invalide = generate_decompte(tabmots, decompte_pr);

    afficher_decompte(tabmots, decompte_pr, nb_candidat, nb_invalide, logfp);

    int indice_pr = indice_premier(decompte_pr, nb_candidat);
    int indice_sd = indice_second(decompte_pr, nb_candidat, indice_pr);

    fprintf(logfp, " ┃ indice premier: %d\n", indice_pr);
    fprintf(logfp, " ┃ indice second: %d\n ┗╸\n", indice_sd);

    float pourcentage_pr = score(tabmots->nbRows-1, nb_invalide, decompte_pr[indice_pr]);

    if (pourcentage_pr > 50){ // Si candidat supérieur à 50%
        fprintf(stdout, "Mode de scrutin : uninominal à deux tours, tour 1, %d candidats, %d votants, vainqueur = %s, score = %.2f%%\n", nb_candidat, tabmots->nbRows-1, tabmots->tab[0][indice_pr+tabmots->offset], pourcentage_pr);
        free(decompte_pr);
    } else {
        float pourcentage_sd = score(tabmots->nbRows-1, nb_invalide, decompte_pr[indice_sd]);

        fprintf(stdout, "Mode de scrutin : uninominal à deux tours, tour 1, %d candidats, %d votants, vainqueur = %s, score = %.2f%%\n", nb_candidat, tabmots->nbRows-1, tabmots->tab[0][indice_pr+tabmots->offset], pourcentage_pr);
        fprintf(stdout, "Mode de scrutin : uninominal à deux tours, tour 1, %d candidats, %d votants, vainqueur = %s, score = %.2f%%\n", nb_candidat, tabmots->nbRows-1, tabmots->tab[0][indice_sd+tabmots->offset], pourcentage_sd);

        free(decompte_pr);

        // Second tour
        
        int *decompte_sd = generate_list_int(2);
        decompte_sd[0]=0;
        decompte_sd[1]=0;

        for (int ligne = 1; ligne < tabmots->nbRows; ligne++){ // Pour chaque vote
            if (vote_valide(tabmots, ligne)==true){
                // Ajouter 1 au candidat le mieux classé. (plus petit préféré)
                if (atoi(tabmots->tab[ligne][indice_pr+tabmots->offset]) < atoi(tabmots->tab[ligne][indice_sd+tabmots->offset])){
                    decompte_sd[0]+=1;
                } else {
                    decompte_sd[1]+=1;
                }            
            }
        }

        fprintf(logfp, " ┏╸\n");
        fprintf(logfp, " ┃ Decompte second tour:\n");
        fprintf(logfp, " ┃ ╰ %s -> %d (%.2f%%)\n", tabmots->tab[0][indice_pr+tabmots->offset], decompte_sd[0], score(tabmots->nbRows-1, nb_invalide, decompte_sd[0]));
        fprintf(logfp, " ┃ ╰ %s -> %d (%.2f%%)\n ┗╸\n", tabmots->tab[0][indice_sd+tabmots->offset], decompte_sd[1], score(tabmots->nbRows-1, nb_invalide, decompte_sd[1]));


        int indice_selectionne, indice_mat_char;
        if (decompte_sd[0] > decompte_sd[1]){
            indice_selectionne=0; // Indice du gagnant
            indice_mat_char=indice_pr+tabmots->offset; // indice correspondant au tableau de données
        } else {
            indice_selectionne=1;
            indice_mat_char=indice_sd+tabmots->offset;
        }

        float pourcentage_gagnant = score(tabmots->nbRows-1, nb_invalide, decompte_sd[indice_selectionne]);
        fprintf(stdout, "Mode de scrutin : uninominal à deux tours, tour 2, 2 candidats, %d votants, vainqueur = %s, score = %.2f%%\n", tabmots->nbRows-1, tabmots->tab[0][indice_mat_char], pourcentage_gagnant);
    
        free(decompte_sd);
    }
}