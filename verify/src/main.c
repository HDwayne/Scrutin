/**
 * @file main.c
 * @author HERZBERG Dwayne
 * @brief Fonction principale du programme.
 */

#include "sha256.h"
#include "sha256_utils.h"
#include "utils.h"
#include "balise.h"
#include "fonction.h"


/**
 * @brief Fonction principal du programme.
 * 
 * @param argc 
 * @param argv 
 * @return int, renvoie 0 si succès sinon 1.
 */
int main(int argc, char **argv){
    char **tableau_balise = generate_matrice_char(NB_BALISES, LONGMOTS);

    getop(tableau_balise, argc, argv);

	if (strcmp(tableau_balise[OPT_NOM_FICHIER_G], "")!=0){ // Si ption g alors générer fichier avec hash
		addhash(tableau_balise);
	} else {
		char *hashRes = generate_list_char(SHA256_BLOCK_SIZE*2+1); // contiendra le hash en hexadécimal
	
		strcpy(hashRes, hash(hashRes, tableau_balise[OPT_NOM_ELECTEUR], tableau_balise[OPT_SECRET_KEY]));
		find_in_file(tableau_balise, hashRes);

		free(hashRes);
	}
	

	for(short i=0; i < NB_BALISES ; ++i ){
        free(tableau_balise[i]);
    }
    free(tableau_balise);

	return 0;
}

// 79X&4zd#$a6BvgsI@A]T:JzX
// 79X&4zd#@A]T:JzX